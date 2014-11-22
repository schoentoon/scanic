/* *
 * -----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Toon Schoenmakers <nighteyes1993@gmail.com>
 * wrote this file. As long as you retain this notice you can do whatever you
 * want with this stuff. If we meet some day, and you think this stuff is worth
 * it, you can buy us a beer in return.
 * -----------------------------------------------------------------------------
 */

#include <iostream>
#include <fstream>
#include <future>
#include <memory>
#include <chrono>
#include <queue>
#include <list>

#include <smarttpl.h>

#include <sys/stat.h>
#include <string.h>
#include <dirent.h>
#include <getopt.h>
#include <dlfcn.h>

#include "input.h"
#include "analyzer.h"

static const struct option g_LongOpts[] = {
  { "help", no_argument, 0, 'h' },
  { "input-module", required_argument, 0, 'I' },
  { "input", required_argument, 0, 'i' },
  { "analyzer", required_argument, 0, 'a' },
  { "template", required_argument, 0, 't' },
  { "output", required_argument, 0, 'o' },
  { "no-threads", no_argument, 0, 'T' },
  { 0, 0, 0, 0 }
};

static int usage(const char *prog) {
  std::cerr << "USAGE: " << prog << " [options]" << std::endl
            << "-h, --help         Show this help message" << std::endl
            << "-I, --input-module The input module to use" << std::endl
            << "-i, --input        Input file/folder, repeat for multiple "
               "files/folders" << std::endl
            << "-t, --template     Use this template to generate our output"
            << std::endl
            << "-o, --output       Output file for the generated output"
            << std::endl
            << "-a, --analyzer     Load this analyzer and use it for our output"
            << std::endl << "-T, --no-threads   Launch with less/no threads."
            << std::endl;
  return 1;
};

using namespace Scanic;

int main(int argc, char **argv) {
  std::list<std::string> input;
  std::unique_ptr<SmartTpl::Source> tplsource;
  std::string outputfile;

  void *input_handle = nullptr;
  typedef Input *(InputCreator)(const std::shared_ptr<Generator> & generator);
  InputCreator *input_creator = nullptr;
  bool no_threads = false;

  std::shared_ptr<Generator> generator(new Generator());

  int arg, optindex;
  while ((arg = getopt_long(argc, argv, "hI:i:a:t:o:T", g_LongOpts,
                            &optindex)) != -1) {
    switch (arg) {
    case 'h':
      return usage(argv[0]);
    case 'I':
      if (input_handle != nullptr) {
        std::cerr << "You already picked a input module, can only pick one!"
                  << std::endl;
        break;
      };
      input_handle = dlopen(optarg, RTLD_NOW);
      if (input_handle == nullptr) {
        std::cerr << dlerror() << std::endl;
        return 1;
      };
      input_creator = (InputCreator *)dlsym(input_handle, "loadInput");
      if (input_creator == nullptr) {
        std::cerr << dlerror() << std::endl;
        return 1;
      };
      break;
    case 'i':
      input.emplace_back(optarg);
      break;
    case 'a': {
      struct stat st_buf;
      if (stat(optarg, &st_buf) != 0) {
        std::cerr << strerror(errno) << std::endl;
        return 1;
      };

      if (S_ISREG(st_buf.st_mode)) {
        void *handle = dlopen(optarg, RTLD_NOW);
        if (handle == nullptr) {
          std::cerr << dlerror() << std::endl;
          return 1;
        };
        typedef Analyzer *(AnalyzerCreator)(void * handle);
        AnalyzerCreator *func =
            (AnalyzerCreator *)dlsym(handle, "loadAnalyzer");
        if (func == nullptr) {
          std::cerr << dlerror() << std::endl;
          return 1;
        };
        generator->loadAnalyzer(func(handle));
      } else if (S_ISDIR(st_buf.st_mode)) {
        struct dirent *ent;
        DIR *dir = nullptr;
        if ((dir = opendir(optarg)) != nullptr) {
          while ((ent = readdir(dir)) != nullptr) {
            // Skip hidden files and . and ..
            if (ent->d_name[0] == '.')
              continue;

            std::string file(optarg);
            file.push_back('/');
            file.append(ent->d_name);
            void *handle = dlopen(file.c_str(), RTLD_NOW);
            if (handle == nullptr) {
              std::cerr << dlerror() << std::endl;
              return 1;
            };
            typedef Analyzer *(AnalyzerCreator)(void * handle);
            AnalyzerCreator *func =
                (AnalyzerCreator *)dlsym(handle, "loadAnalyzer");
            if (func == nullptr) {
              std::cerr << dlerror() << std::endl;
              return 1;
            };
            generator->loadAnalyzer(func(handle));
          };
        };
      };
      break;
    };
    case 't':
      tplsource.reset(new SmartTpl::File(optarg));
      break;
    case 'o':
      outputfile = optarg;
      break;
    case 'T':
      no_threads = true;
      break;
    };
  };

  if (input.empty()) {
    std::cerr << "No input files/folders, can't continue." << std::endl
              << std::endl;
    return usage(argv[0]);
  };

  if (input_handle == nullptr || input_creator == nullptr) {
    std::cerr << "No input module found, can't continue." << std::endl
              << std::endl;
    return usage(argv[0]);
  };

  if (!tplsource) {
    std::cerr << "No template specified" << std::endl << std::endl;
    return usage(argv[0]);
  };

  if (outputfile.empty()) {
    std::cerr << "No output file specified" << std::endl << std::endl;
    return usage(argv[0]);
  };

  std::chrono::time_point<std::chrono::system_clock> start(
      std::chrono::system_clock::now());

  std::queue<std::future<uint64_t> > futures;

  for (const std::string &in : input) {
    struct stat st_buf;
    if (stat(in.c_str(), &st_buf) != 0) {
      std::cerr << strerror(errno) << std::endl;
      continue;
    };

    if (S_ISREG(st_buf.st_mode)) {
      auto func = [input_creator, generator, in]()->uint64_t {
        uint64_t processed_lines = 0;
        std::unique_ptr<Input> input(input_creator(generator));
        // Open the file, loop through it and pass every line to our input
        // analyzer
        std::ifstream logfile(in);
        for (std::string line; std::getline(logfile, line); ++processed_lines) {
          try {
            input->process(line, in);
          }
          catch (const std::exception &error) {
            std::cerr << error.what() << std::endl;
          };
        }
        return processed_lines;
      };
      if (no_threads) {
        std::packaged_task<uint64_t()> task(func);
        futures.push(std::move(task.get_future()));
        task();
      } else
        futures.emplace(std::async(std::launch::async, func));
    } else if (S_ISDIR(st_buf.st_mode)) {
      DIR *dir = nullptr;
      struct dirent *ent;
      if ((dir = opendir(in.c_str())) != nullptr) {
        while ((ent = readdir(dir)) != nullptr) {
          // Skip hidden files and . and ..
          if (ent->d_name[0] == '.')
            continue;

          // Build our filename
          std::string logfilename(in.c_str());
          logfilename.push_back('/');
          logfilename.append(ent->d_name);

          if (stat(in.c_str(), &st_buf) == 0) {
            if (S_ISREG(st_buf.st_mode)) {
              auto func = [input_creator, generator, logfilename]()->uint64_t {
                uint64_t processed_lines = 0;
                std::unique_ptr<Input> input(input_creator(generator));
                // Open the file, loop through it and pass every line to our
                // input analyzer
                std::ifstream logfile(logfilename);
                for (std::string line; std::getline(logfile, line);
                     ++processed_lines) {
                  try {
                    input->process(line, logfilename);
                  }
                  catch (const std::exception &error) {
                    std::cerr << error.what() << std::endl;
                  };
                }
                return processed_lines;
              };
              if (no_threads) {
                std::packaged_task<uint64_t()> task(func);
                futures.push(std::move(task.get_future()));
                task();
              } else
                futures.emplace(std::async(std::launch::async, func));
            } else if (S_ISDIR(st_buf.st_mode))
              input.push_back(logfilename);
          };
        }
        closedir(dir);
      };
    } else
      std::cerr << in << " is not a file or a directory." << std::endl;
  };

  uint64_t processed_lines = 0;

  while (!futures.empty()) {
    auto &future = futures.front();
    processed_lines += future.get();
    futures.pop();
  };

  // We no longer need the input module at this point so let's close it and all
  dlclose(input_handle);
  input_handle = nullptr;
  input_creator = nullptr;

  if (generator->requiresSorting()) {
    auto sorttime = generator->sort();
    std::cerr << "Sorting took about " << sorttime.count() << " seconds."
              << std::endl;
  }

  auto output = generator->analyze(no_threads);

  std::chrono::duration<double> elapsed_seconds(
      std::chrono::system_clock::now() - start);
  std::cerr << "Processed " << processed_lines << " lines in "
            << elapsed_seconds.count() << " seconds. " << std::endl;

  SmartTpl::Template tpl(*tplsource);

  if (outputfile == "-") {
    std::cout << tpl.process(output);
  } else {
    std::ofstream outstream(outputfile);

    if (!outstream.is_open()) {
      std::cerr << strerror(errno) << std::endl;
      return 1;
    };

    outstream << tpl.process(output);
    outstream.close();
  }

  return 0;
}
