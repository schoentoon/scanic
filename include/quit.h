#pragma once

#include "event.h"

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

  const std::string message() const { return _message; };
  QuitEvent& message(const std::string &message) {
    _message = message;
    return *this;
  };
};