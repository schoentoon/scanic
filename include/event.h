#pragma once

#include <ctime>
#include <string>

class Event {
private:
  /**
   * The timestamp of the message
   */
  std::tm _time;
public:
  Event(std::tm &&time)
  : _time(std::move(time)) {
  };

  Event() {};

  const std::tm time() const { return _time; };
  void time(std::tm time) {
    _time = time;
  };
};