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

class KickEvent : public Event {
private:
  /**
   * The username of the kicked user
   */
  std::string _username;

  /**
   * The username of the kicker
   */
  std::string _kicker;

  /**
   * The kick message
   */
  std::string _message;
public:
  KickEvent()
  : Event()
  , _username()
  , _kicker()
  , _message() {
  };
  KickEvent(std::tm &&when, std::string &&username, std::string &&kicker, std::string &&msg = "")
  : Event(std::move(when))
  , _username(std::move(username))
  , _kicker(std::move(kicker))
  , _message(std::move(msg)) {
  };

  virtual ~KickEvent() {};

  const std::string& username() const { return _username; };
  KickEvent& username(const std::string &username) {
    _username = username;
    return *this;
  };
  KickEvent& username(std::string &&username) {
    _username = std::move(username);
    return *this;
  };

  const std::string& kicker() const { return _kicker; };
  KickEvent& kicker(const std::string &kicker) {
    _kicker = kicker;
    return *this;
  };
  KickEvent& kicker(std::string &&kicker) {
    _kicker = std::move(kicker);
    return *this;
  };

  const std::string& message() const { return _message; };
  KickEvent& message(const std::string &message) {
    _message = message;
    return *this;
  };
  KickEvent& message(std::string &&message) {
    _message = std::move(message);
    return *this;
  };
};

};
