#pragma once

#include <list>
#include <string>
#include <chrono>
#include <iostream>

#include "message.h"
#include "join.h"
#include "quit.h"
#include "part.h"
#include "kick.h"

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

  /**
   * This will contain all the quit users and the time they quit
   */
  std::list<QuitEvent> _quits;

  /**
   * This will contain all the parted users and the time they parted
   */
  std::list<PartEvent> _parts;

  /**
   * This will contain all the kicked users and the times they were kicked
   */
  std::list<KickEvent> _kicks;
public:
  virtual ~Generator() {};

  void publishMessage(Message &&message) { _messages.push_back(std::move(message)); };

  void publishJoin(JoinEvent &&join) { _joins.push_back(std::move(join)); };

  void publishQuit(QuitEvent &&quit) { _quits.push_back(std::move(quit)); };

  void publishPart(PartEvent &&part) { _parts.push_back(std::move(part)); };

  void publishKick(KickEvent &&kick) { _kicks.push_back(std::move(kick)); };

  std::chrono::duration<double> sort();
};