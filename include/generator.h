#pragma once

#include <list>
#include <string>
#include <chrono>
#include <mutex>
#include <iostream>

#include "analyzer.h"

#include "message.h"
#include "join.h"
#include "quit.h"
#include "part.h"
#include "kick.h"
#include "nickchange.h"

/**
 * Forward declaration
 */
class Input;

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

  /**
   * This will contain all the nick name changes
   */
  std::list<NickChangeEvent> _nick_changes;

  /**
   * Lock for when we're mass inserting events
   */
  std::mutex _lock;

  /**
   * All the analyzers to use to generate our end result
   */
  std::list<std::unique_ptr<Analyzer, void(*)(Analyzer*)>> _analyzers;
public:
  Generator();
  virtual ~Generator() {};

  std::chrono::duration<double> sort();

  const std::list<Message> &messages() const { return _messages; };

  const std::list<JoinEvent> &joins() const { return _joins; };

  const std::list<QuitEvent> &quits() const { return _quits; };

  const std::list<PartEvent> &parts() const { return _parts; };

  const std::list<KickEvent> &kicks() const { return _kicks; };

  const std::list<NickChangeEvent> &nickChanges() const { return _nick_changes; };

  void insertInput(Input *input);

  /**
   *  Load an analyzer, we will take ownership!
   */
  void loadAnalyzer(Analyzer *analyzer);

  Variant::Value analyze();
};