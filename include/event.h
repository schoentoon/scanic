/* * -----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Toon Schoenmakers <nighteyes1993@gmail.com>
 * wrote this file. As long as you retain this notice you can do whatever you
 * want with this stuff. If we meet some day, and you think this stuff is worth
 * it, you can buy us a beer in return.
 * -----------------------------------------------------------------------------
 */

#pragma once

#include <ctime>
#include <string>
#include <functional>

namespace Scanic {

class Event {
public:
  /**
   *  Simple compare function
   */
  static bool compare(const Event &lhs, const Event &rhs) { return lhs._time < rhs._time; };
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

  virtual ~Event() {};

  const std::time_t time() const { return _time; };
  void time(std::tm time) {
    _time = std::mktime(&time);
  };
  void time(std::time_t time) {
    _time = time;
  };
};

};
