#pragma once

#include "event.h"

class Message : public Event {
private:

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
  const std::string message() const { return _message; };
  Message& message(const std::string &message) {
    _message = message;
    return *this;
  };
  Message& message(std::string &&message) {
    _message = std::move(message);
    return *this;
  };

  const std::string author() const { return _author; };
  Message& author(const std::string &author) {
    _author = author;
    return *this;
  };
  Message& author(std::string &&author) {
    _author = std::move(author);
    return *this;
  };

  const bool action() const { return _action; };
  Message& action(bool action) {
    _action = action;
    return *this;
  };
};