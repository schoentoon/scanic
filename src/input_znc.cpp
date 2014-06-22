#include "input_znc.h"

#include <iostream>
#include <iomanip>
#include <stdexcept>

void ZncInput::process(const std::string& input, const std::string &path) {
  std::string date_string = path.substr(path.find_last_of('_'));
  std::tm time;
  if (strptime(date_string.c_str(), "_%Y%m%d.log", &time) == nullptr) throw std::runtime_error("Failed to parse date");
  if (strptime(input.c_str(), "[%H:%M:%S]", &time) == nullptr) throw std::runtime_error("Failed to parse time");

  Message output;
  output.time(time);

  std::size_t open_user = input.find_first_of('<');
  std::size_t end_user = input.find_first_of('>');
  if (open_user == std::string::npos || end_user == std::string::npos) {
    // For now we simply skip any form of actions
    if (input.find("***") != std::string::npos) {
      open_user = input.find("*** Joins: ");
      if (open_user != std::string::npos) {
        open_user += 11; // Increase by length of the literal string above "*** Joins: "
        end_user = input.find_first_of(' ', open_user);
        if (end_user != std::string::npos) {
          publishJoin(JoinEvent(std::move(time), input.substr(open_user, end_user - open_user)));
          return;
        };
      } else {
        open_user = input.find("*** Quits: ");
        end_user = input.find("*** Parts: "); // Yes we're abusing end_user for something entirely different here..
        if (open_user != std::string::npos || end_user != std::string::npos) {
          bool part = end_user != std::string::npos;
          open_user = end_user;
          open_user += 11; // Increase by length of the literal string above "*** Quits: "
          end_user = input.find_first_of(' ', open_user);
          if (end_user != std::string::npos) {
            std::size_t open_msg = input.find_first_of('(', open_user);
            if (open_msg == std::string::npos) throw std::runtime_error("Failed to parse quit message.");
            open_msg = input.find_first_of('(', open_msg + 1);
            if (open_msg == std::string::npos) throw std::runtime_error("Failed to parse quit message.");
            std::string msg = input.substr(open_msg + 1, input.size() - open_msg - 2);

            if (part) publishPart(PartEvent(std::move(time), input.substr(open_user, end_user - open_user), std::move(msg)));
            else publishQuit(QuitEvent(std::move(time), input.substr(open_user, end_user - open_user), std::move(msg)));
            return;
          };
        };
      };
    };

    // Search for the "* " which indicates an action
    open_user = input.find("* ");
    if (open_user == std::string::npos) throw std::runtime_error("Couldn't find username..");
    open_user += 2; // Increase it by 2 so we're at the first character of the username

    end_user = input.find_first_of(' ', open_user);
    output.action(true) // mark the message as an action
          .author(input.substr(open_user, end_user - open_user));
  } else
    output.author(input.substr(open_user + 1, end_user - open_user - 1)); // +1 to the open to skip the < and - 1 to the end to skip the >

  // + 2 to skip the > of the username and the space just after it
  output.message(input.substr(end_user + 2));

  publishMessage(std::move(output));
}