#pragma once

#include <memory>

#include "message.h"
#include "generator.h"

class Input {
private:
  std::shared_ptr<Generator> _generator;
protected:
  /**
   * Call this to publish messages to the generator
   */
  void publishMessage(Message &&message);

  /**
   * Call this to publish a joined user
   */
  void publishJoin(JoinEvent &&join);

  /**
   * Call this to publish a quit user
   */
  void publishQuit(QuitEvent &&quit);

  /**
   * Call this to publish a parted user
   */
  void publishPart(PartEvent &&part);
public:
  Input(const std::shared_ptr<Generator> &generator);

  virtual ~Input() {};

  /**
   * Implement this method to add support for your own logfiles
   */
  virtual void process(const std::string &input, const std::string &path) = 0;
};