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

class JoinEvent : public Event {
private:
  /**
   * The username of the joined user
   */
  std::string _username;
public:
  JoinEvent()
  : Event()
  , _username() {
  };
  JoinEvent(std::tm &&when, std::string &&username)
  : Event(std::move(when))
  , _username(std::move(username)) {
  };

  virtual ~JoinEvent() {};

  const std::string& username() const { return _username; };
  JoinEvent& username(const std::string &username) {
    _username = username;
    return *this;
  };
  JoinEvent& username(std::string &&username) {
    _username = std::move(username);
    return *this;
  };
};

};
