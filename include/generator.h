#pragma once

#include <list>
#include <string>
#include <iostream>

#include "message.h"
#include "join.h"

class Generator {
private:
  /**
   * This will contain all the messages
   */
  std::list<Message> _messages;

  /**
   * This will contain all the joined users and the time they joined
   */
  std::list<JoinEvent> _joins;
public:
  virtual ~Generator() {};

  void publishMessage(Message &&message) { _messages.push_back(std::move(message)); };

  void publishJoin(JoinEvent &&join) { _joins.push_back(std::move(join)); };
};