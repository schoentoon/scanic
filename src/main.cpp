#include <iostream>
#include <fstream>
#include <future>
#include <memory>
#include <chrono>
#include <queue>
#include <list>

#include <dirent.h>
#include <getopt.h>

#include "input_znc.h"
#include "linecount.h"

static const struct option g_LongOpts[] = {
  { "help",       no_argument,       0, 'h' },
  { "input",      required_argument, 0, 'i' },
  { 0, 0, 0, 0 }
};

static int usage(const char *prog) {
  std::cerr << "USAGE: " << prog << " [options]" << std::endl
            << "-h, --help      Show this help message" << std::endl
            << "-i, --input     Input folder, repeat for multiple folders" << std::endl;
  return 1;
};

int main(int argc, char **argv) {
  std::vector<std::string> input;
  int arg, optindex;
  while ((arg = getopt_long(argc, argv, "hi:", g_LongOpts, &optindex)) != -1) {
    switch (arg) {
    case 'h':
      return usage(argv[0]);
    case 'i':
      input.emplace_back(optarg);
      break;
    };
  };

  if (input.empty()) {
    std::cerr << "No input folders, can't continue." << std::endl;
    return 1;
  };

  std::chrono::time_point<std::chrono::system_clock> start(std::chrono::system_clock::now());

  // Create our input analyzer, make this dynamic later on of course
  std::shared_ptr<Generator> generator(new Generator());

  // Yuck we can only loop through a directory using C :(
  DIR *dir = nullptr;
  struct dirent *ent;
  std::queue<std::future<uint64_t>> futures;

  for (const std::string &in : input) {
    if ((dir = opendir(in.c_str())) != nullptr) {
      while ((ent = readdir (dir)) != nullptr) {
        // Skip hidden files and . and ..
        if (ent->d_name[0] == '.') continue;

        // Build our filename
        std::string logfilename(in.c_str());
        logfilename.push_back('/');
        logfilename.append(ent->d_name);

        futures.emplace(std::async(std::launch::async, [generator, logfilename]() -> uint64_t {
          uint64_t processed_lines = 0;
          std::unique_ptr<Input> input(new ZncInput(generator));
          // Open the file, loop through it and pass every line to our input analyzer
          std::ifstream logfile(logfilename);
          for (std::string line; std::getline(logfile, line); ++processed_lines) {
            try {
              input->process(line, logfilename);
            } catch (const std::exception &error) {
              std::cerr << error.what() << std::endl;
            };
          }
          return processed_lines;
        }));
      }
      closedir(dir);
    };
  };

  uint64_t processed_lines = 0;

  while (!futures.empty()) {
    auto &future = futures.front();
    processed_lines += future.get();
    futures.pop();
  };

  auto sorttime = generator->sort();
  std::cerr << "Sorting took about " << sorttime.count() << " seconds." << std::endl;

  generator->loadAnalyzer(new LineCountAnalyzer());

  auto output = generator->analyze();

  std::cerr << output.toJsonString() << std::endl;

  std::chrono::duration<double> elapsed_seconds(std::chrono::system_clock::now() - start);
  std::cerr << "Processed " << processed_lines << " lines in " << elapsed_seconds.count() << " seconds. " << std::endl;
  return 0;
}
