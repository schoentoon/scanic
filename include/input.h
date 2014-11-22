/* * -----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Toon Schoenmakers <nighteyes1993@gmail.com>
 * wrote this file. As long as you retain this notice you can do whatever you
 * want with this stuff. If we meet some day, and you think this stuff is worth
 * it, you can buy us a beer in return.
 * -----------------------------------------------------------------------------
 */

#pragma once

#include <memory>

#include "message.h"
#include "generator.h"

#define INPUTMODULE(classname) extern "C" Scanic::Input* loadInput(const std::shared_ptr<Scanic::Generator> &generator) { return new classname(generator); }

namespace Scanic {

class Input {
private:
  /**
   * The generator we actually publish our events to
   */
  std::shared_ptr<Generator> _generator;

  /**
   * Generator is our friend so he can touch our privates
   */
  friend class Generator;

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
protected:
  /**
   * Call this to publish messages to the generator
   */
  void publishMessage(Message &&message) { _messages.push_back(std::move(message)); };

  /**
   * Call this to publish a joined user
   */
  void publishJoin(JoinEvent &&join) { _joins.push_back(std::move(join)); };

  /**
   * Call this to publish a quit user
   */
  void publishQuit(QuitEvent &&quit) { _quits.push_back(std::move(quit)); };

  /**
   * Call this to publish a parted user
   */
  void publishPart(PartEvent &&part) { _parts.push_back(std::move(part)); };

  /**
   * Call this to publish a kicked user
   */
  void publishKick(KickEvent &&kick) { _kicks.push_back(std::move(kick)); };

  /**
   * Call this to publish a change nickname event
   */
  void publishNickChange(NickChangeEvent &&nickchange) { _nick_changes.push_back(std::move(nickchange)); };
public:
  Input(const std::shared_ptr<Generator> &generator);

  virtual ~Input();

  /**
   * Implement this method to add support for your own logfiles
   */
  virtual void process(const std::string &input, const std::string &path) = 0;
};

};
