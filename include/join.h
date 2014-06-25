#pragma once

#include "event.h"

class JoinEvent : public Event {
private:
  /**
   * The username of the joined user
   */
  std::string _username;
public:
  JoinEvent() : Event() {};
  JoinEvent(std::tm &&when, std::string &&username)
  : Event(std::move(when))
  , _username(std::move(username)) {
  };

  const std::string username() const { return _username; };
  JoinEvent& username(const std::string &username) {
    _username = username;
    return *this;
  };
};