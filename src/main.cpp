#include <iostream>
#include <fstream>
#include <memory>
#include <chrono>
#include <list>

#include <dirent.h>

#include "input_znc.h"

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "We'll need at least a directory to scan for log files." << std::endl;
    return 1;
  };

  std::chrono::time_point<std::chrono::system_clock> start(std::chrono::system_clock::now());

  // Create our input analyzer, make this dynamic later on of course
  std::unique_ptr<Input> input(new ZncInput());

  uint64_t processed_lines = 0;
  std::list<Message> messages;
  // Yuck we can only loop through a directory using C :(
  DIR *dir = nullptr;
  struct dirent *ent;
  if ((dir = opendir(argv[1])) != nullptr) {
    while ((ent = readdir (dir)) != nullptr) {
      // Skip hidden files and . and ..
      if (ent->d_name[0] == '.') continue;

      // Build our filename
      std::string logfilename(argv[1]);
      logfilename.append("/").append(ent->d_name);

      // Open the file, loop through it and pass every line to our input analyzer
      std::ifstream logfile(logfilename);
      for (std::string line; std::getline(logfile, line); ++processed_lines) {
        try {
          messages.push_back(std::move(input->process(line, logfilename)));
        } catch (const std::exception &error) {
          std::cerr << error.what() << std::endl;
        };
      }
    }
    closedir(dir);
  };

  std::chrono::duration<double> elapsed_seconds(std::chrono::system_clock::now() - start);
  std::cerr << "We got a total of " << messages.size() << " messages." << std::endl;
  std::cerr << "Processed " << processed_lines << " lines in " << elapsed_seconds.count() << " seconds. " << std::endl;
  return 0;
}
