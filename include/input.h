#pragma once

#include "message.h"

class Input {
public:
  virtual ~Input() {};

  /**
   * Implement this method to add support for your own logfiles
   */
  virtual Message process(const std::string &input, const std::string &path) const = 0;
};