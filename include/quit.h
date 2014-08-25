/* * -----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Toon Schoenmakers <nighteyes1993@gmail.com>
 * wrote this file. As long as you retain this notice you can do whatever you
 * want with this stuff. If we meet some day, and you think this stuff is worth
 * it, you can buy us a beer in return.
 * -----------------------------------------------------------------------------
 */

#pragma once

#include "event.h"

namespace Scanic {

class QuitEvent : public Event {
private:
  /**
   * The username of the joined user
   */
  std::string _username;

  /**
   * The message the user quit with
   */
  std::string _message;
public:
  QuitEvent() : Event() {};
  QuitEvent(std::tm &&when, std::string &&username, std::string &&msg = "")
  : Event(std::move(when))
  , _username(std::move(username))
  , _message(std::move(msg)) {
  };

  const std::string username() const { return _username; };
  QuitEvent& username(const std::string &username) {
    _username = username;
    return *this;
  };
  QuitEvent& username(std::string &&username) {
    _username = std::move(username);
    return *this;
  };

  const std::string message() const { return _message; };
  QuitEvent& message(const std::string &message) {
    _message = message;
    return *this;
  };
  QuitEvent& message(std::string &&message) {
    _message = std::move(message);
    return *this;
  };
};

};