#pragma once

#include <string>
#include <ctime>

class JoinEvent {
private:
  /**
   * When did this user join the channel?
   */
  std::tm _when;

  /**
   * The username of the joined user
   */
  std::string _username;
public:
  JoinEvent() {};
  JoinEvent(std::tm &&when, std::string &&username)
  : _when(std::move(when))
  , _username(std::move(username)) {
  };

  const std::tm when() const { return _when; };
  JoinEvent& when(const std::tm &when) {
    _when = when;
    return *this;
  };

  const std::string username() const { return _username; };
  JoinEvent& username(const std::string &username) {
    _username = username;
    return *this;
  };
};