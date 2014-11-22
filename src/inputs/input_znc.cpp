/* *
 * -----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Toon Schoenmakers <nighteyes1993@gmail.com>
 * wrote this file. As long as you retain this notice you can do whatever you
 * want with this stuff. If we meet some day, and you think this stuff is worth
 * it, you can buy us a beer in return.
 * -----------------------------------------------------------------------------
 */

#include "input.h"

#include <iostream>
#include <iomanip>
#include <stdexcept>

namespace Scanic {

class ZncInput : public Input {
public:
  ZncInput(const std::shared_ptr<Generator> &generator)
      : Input(std::move(generator)) {};

  virtual ~ZncInput() {};

  void process(const std::string &input, const std::string &path) override {
    std::string date_string = path.substr(path.find_last_of('_'));
    std::tm time;

    if (strptime(date_string.c_str(), "_%Y%m%d.log", &time) == nullptr)
      throw std::runtime_error("Failed to parse date");

    if (strptime(input.c_str(), "[%H:%M:%S] *** ", &time) !=
        nullptr) { // Event block
      if (std::size_t open_user =
              input.find("*** Joins: ") != std::string::npos) { // Join block
        open_user +=
            11; // Increase by length of the literal string above "*** Joins: "
        std::size_t end_user = input.find_first_of(' ', open_user);

        if (end_user != std::string::npos)
          publishJoin(JoinEvent(std::move(time),
                                input.substr(open_user, end_user - open_user)));

      } else if (std::size_t open_user =
                     input.find(" is now known as ") !=
                     std::string::npos) { // Name change block
        // Time to check for name changes..
        open_user = input.find("*** ") + 4;
        NickChangeEvent nickchange;
        nickchange.originalUsername(
                       input.substr(open_user,
                                    input.find(" ", open_user) - open_user))
            .time(std::move(time));
        open_user = input.find(" is now known as ") +
                    17; // 17 is the length of " is now known as "
        nickchange.newUsername(
            input.substr(open_user, input.size() - open_user));
        publishNickChange(std::move(nickchange));
      } else if (input.find(" was kicked by ") != std::string::npos) {
        std::size_t open_user =
            input.find("*** ") +
            4; // Increase by 4 so we're at the start of the username
        KickEvent kick;
        kick.username(
                 input.substr(open_user, input.find_first_of(' ', open_user) -
                                             open_user)).time(std::move(time));
        open_user = input.find(" was kicked by ") +
                    15; // 15 is the length of " was kicked by "
        kick.kicker(input.substr(
            open_user, input.find_first_of(' ', open_user) - open_user));
        open_user = input.find("(", open_user); // We're abusing open_user here
                                                // to store the start of the
                                                // kick message

        if (open_user != std::string::npos)
          kick.message(
              input.substr(open_user + 1, input.size() - open_user - 2));

        publishKick(std::move(kick));
      } else if (bool part = input.find("*** Parts: ") != std::string::npos ||
                             input.find("*** Quits: ") != std::string::npos) {
        // Here we're going to check for quits and parts
        open_user = input.find("*** ");
        open_user = input.find(": ", open_user) +
                    2; // Increase by 2 to be just after the space
        std::size_t end_user = input.find_first_of(' ', open_user);
        if (end_user != std::string::npos) {
          std::size_t open_msg = input.find_first_of('(', open_user);

          if (open_msg == std::string::npos)
            throw std::runtime_error("Failed to parse quit message.");

          open_msg = input.find_first_of('(', open_msg + 1);

          if (open_msg == std::string::npos)
            throw std::runtime_error("Failed to parse quit message.");

          std::string msg =
              input.substr(open_msg + 1, input.size() - open_msg - 2);

          if (part)
            publishPart(PartEvent(std::move(time),
                                  input.substr(open_user, end_user - open_user),
                                  std::move(msg)));
          else
            publishQuit(QuitEvent(std::move(time),
                                  input.substr(open_user, end_user - open_user),
                                  std::move(msg)));
        };
      };

      return;
    } else if (strptime(input.c_str(), "[%H:%M:%S] * ", &time) !=
               nullptr) { // Action block
      Message output;
      output.action(true).time(std::move(time));

      std::size_t open_user = input.find("] * ");

      if (open_user == std::string::npos)
        throw std::runtime_error("Couldn't find username..");

      open_user +=
          4; // Increase it by 4 so we're at the first character of the username
      std::size_t end_user = input.find_first_of(' ', open_user);

      output.author(input.substr(open_user, end_user - open_user)).message(
          input.substr(end_user + 2)); // + 2 to skip the > of the username and
                                       // the space just after it

      publishMessage(std::move(output));

      return;
    } else if (strptime(input.c_str(), "[%H:%M:%S]", &time) == nullptr)
      throw std::runtime_error("Failed to parse time");

    std::size_t open_user = input.find_first_of('<');
    std::size_t end_user = input.find_first_of('>');
    if (open_user == std::string::npos || end_user == std::string::npos)
      throw std::runtime_error("Uhm what is this?");

    Message output;
    output.author(input.substr(open_user + 1,
                               end_user - open_user -
                                   1)) // +1 to the open to skip the < and - 1
                                       // to the end to skip the >
                                           .message(input.substr(end_user + 2))
                                           .time(std::move(time)); // + 2 to
                                                                   // skip the >
                                                                   // of the
                                                                   // username
                                                                   // and the
                                                                   // space just
                                                                   // after it

    publishMessage(std::move(output));
  };
};
};

INPUTMODULE(Scanic::ZncInput);
