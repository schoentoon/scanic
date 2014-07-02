#pragma once

#include <list>
#include <string>
#include <chrono>
#include <iostream>

#include "analyzer.h"

#include "message.h"
#include "join.h"
#include "quit.h"
#include "part.h"
#include "kick.h"
#include "nickchange.h"

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
   * All the analyzers to use to generate our end result
   */
  std::list<std::unique_ptr<Analyzer>> _analyzers;
public:
  virtual ~Generator() {};

  void publishMessage(Message &&message) { _messages.push_back(std::move(message)); };

  void publishJoin(JoinEvent &&join) { _joins.push_back(std::move(join)); };

  void publishQuit(QuitEvent &&quit) { _quits.push_back(std::move(quit)); };

  void publishPart(PartEvent &&part) { _parts.push_back(std::move(part)); };

  void publishKick(KickEvent &&kick) { _kicks.push_back(std::move(kick)); };

  void publishNickChange(NickChangeEvent &&nickchange) { _nick_changes.push_back(std::move(nickchange)); };

  std::chrono::duration<double> sort();

  const std::list<Message> &messages() const { return _messages; };

  const std::list<JoinEvent> &joins() const { return _joins; };

  const std::list<QuitEvent> &quits() const { return _quits; };

  const std::list<PartEvent> &parts() const { return _parts; };

  const std::list<KickEvent> &kicks() const { return _kicks; };

  const std::list<NickChangeEvent> &nickChanges() const { return _nick_changes; };

  /**
   *  Load an analyzer, we will take ownership!
   */
  void loadAnalyzer(Analyzer *analyzer) { _analyzers.emplace_back(analyzer); };

  Variant::Value analyze();
};