#pragma once

#include <ctime>
#include <string>

class Event {
private:
  /**
   * The timestamp of the message
   */
  std::time_t _time;
public:
  Event(std::tm &&time)
  : _time(std::mktime(&time)) {
  };

  Event(std::time_t &&time)
  : _time(std::move(time)) {
  };

  Event() : _time(0) {};

  const std::time_t time() const { return _time; };
  void time(std::tm time) {
    _time = std::mktime(&time);
  };
  void time(std::time_t time) {
    _time = time;
  };
};