#pragma once

#include <list>
#include <string>
#include <iostream>

#include "message.h"

class Generator {
private:
  /**
   * This will contain all the messages
   */
  std::list<Message> _messages;

  /**
   * This will contain all the joined users and the time they joined
   */
  std::list<std::pair<std::tm, std::string>> _joins;
public:
  virtual ~Generator() {};

  void publishMessage(Message &&message) { _messages.push_back(std::move(message)); };

  void publishJoin(std::tm &&when, std::string &&username) { _joins.emplace_back(std::move(when), std::move(username)); };
};