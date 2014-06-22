#pragma once

#include <string>
#include <ctime>

class QuitEvent {
private:
  /**
   * When did this user join the channel?
   */
  std::tm _when;

  /**
   * The username of the joined user
   */
  std::string _username;

  /**
   * The message the user quit with
   */
  std::string _message;
public:
  QuitEvent() {};
  QuitEvent(std::tm &&when, std::string &&username, std::string &&msg = "")
  : _when(std::move(when))
  , _username(std::move(username)) {
  };

  const std::tm when() const { return _when; };
  QuitEvent& when(const std::tm &when) {
    _when = when;
    return *this;
  };

  const std::string username() const { return _username; };
  QuitEvent& username(const std::string &username) {
    _username = username;
    return *this;
  };

  const std::string message() const { return _message; };
  QuitEvent& message(const std::string &message) {
    _message = message;
    return *this;
  };
};