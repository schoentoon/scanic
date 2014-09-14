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
  Message(const std::string &message)
  : Event()
  , _message(message)
  , _author() {
  };
  Message(std::string &&message)
  : Event()
  , _message(std::move(message))
  , _author() {
  };
  Message(const std::string &message, const std::string &author)
  : Event()
  , _message(message)
  , _author(author) {
  };
  Message(std::string &&message, std::string &&author)
  : Event()
  , _message(std::move(message))
  , _author(std::move(author)) {
  };
  Message()
  : Event()
  , _message()
  , _author() {
  };

  virtual ~Message() {};

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

};
