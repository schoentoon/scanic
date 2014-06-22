#pragma once

#include <ctime>
#include <string>

class Message {
private:
  /**
   * The timestamp of the message
   */
  std::tm _time;

  /**
   * The actual message
   */
  std::string _message;

  /**
   * The sender of the message
   */
  std::string _author;

  /**
   * Is this message actually an action?
   */
  bool _action = false;
public:
  const std::tm time() const { return _time; };
  Message& time(std::tm time) {
    _time = time;
    return *this;
  };

  const std::string message() const { return _message; };
  Message& message(std::string message) {
    _message = message;
    return *this;
  };

  const std::string author() const { return _author; };
  Message& author(std::string author) {
    _author = author;
    return *this;
  };

  const bool action() const { return _action; };
  Message& action(bool action) {
    _action = action;
    return *this;
  };
};