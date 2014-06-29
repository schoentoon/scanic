#pragma once

#pragma once

#include "event.h"

class NickChangeEvent : public Event {
private:
  /**
   * The original username of the user
   */
  std::string _original_username;

  /**
   * The new username of the user
   */
  std::string _new_username;
public:
  NickChangeEvent() : Event() {};
  NickChangeEvent(std::tm &&when, std::string &&original_username, std::string &&new_username)
  : Event(std::move(when))
  , _original_username(std::move(original_username))
  , _new_username(std::move(new_username)) {
  };

  const std::string originalUsername() const { return _original_username; };
  NickChangeEvent& originalUsername(const std::string &original_username) {
    _original_username = original_username;
    return *this;
  };
  NickChangeEvent& originalUsername(std::string &&original_username) {
    _original_username = std::move(original_username);
    return *this;
  };

  const std::string newUsername() const { return _new_username; };
  NickChangeEvent& newUsername(const std::string &new_username) {
    _new_username = new_username;
    return *this;
  };
  NickChangeEvent& newUsername(std::string &&new_username) {
    _new_username = std::move(new_username);
    return *this;
  };
};