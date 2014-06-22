#include "input_znc.h"

#include <iostream>
#include <iomanip>
#include <stdexcept>

Message ZncInput::process(const std::string& input, const std::string &path) const {
  std::string date_string = path.substr(path.find_last_of('_'));
  std::tm time;
  if (strptime(date_string.c_str(), "_%Y%m%d.log", &time) == nullptr) throw std::runtime_error("Failed to parse date");
  if (strptime(input.c_str(), "[%H:%M:%S]", &time) == nullptr) throw std::runtime_error("Failed to parse time");

  Message output;
  output.time(time);

  std::size_t open_user = input.find_first_of('<');
  std::size_t end_user = input.find_first_of('>');
  if (open_user == std::string::npos || end_user == std::string::npos) throw std::runtime_error("Couldn't find username..");
  output.author(input.substr(open_user + 1, end_user - open_user - 1)); // +1 to the open to skip the < and - 1 to the end to skip the >

  // + 2 to skip the > of the username and the space just after it
  output.message(input.substr(end_user + 2));

  return Message();
}