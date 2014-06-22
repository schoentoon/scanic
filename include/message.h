#pragma once

#include <ctime>
#include <string>

class Message {
private:
  /**
   * The timestamp of the message
   */
  std::time_t _timestamp;

  /**
   * The actual message
   */
  std::string _message;

  /**
   * The sender of the message
   */
  std::string _author;
public:
  const std::time_t timestamp() const { return _timestamp; };
  Message& timestamp(std::time_t timestamp) {
    _timestamp = timestamp;
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
};