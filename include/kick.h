#pragma once

#include <string>
#include <ctime>

class KickEvent {
private:
  /**
   * When did this user get kicked?
   */
  std::tm _when;

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
  KickEvent() {};
  KickEvent(std::tm &&when, std::string &&username, std::string &&kicker, std::string &&msg = "")
  : _when(std::move(when))
  , _username(std::move(username))
  , _kicker(std::move(kicker))
  , _message(std::move(msg)) {
  };

  const std::tm when() const { return _when; };
  KickEvent& when(const std::tm &when) {
    _when = when;
    return *this;
  };

  const std::string username() const { return _username; };
  KickEvent& username(const std::string &username) {
    _username = username;
    return *this;
  };

  const std::string kicker() const { return _kicker; };
  KickEvent& kicker(const std::string &kicker) {
    _kicker = kicker;
    return *this;
  };

  const std::string message() const { return _message; };
  KickEvent& message(const std::string &message) {
    _message = message;
    return *this;
  };
};