#pragma once

#include "input.h"

class ZncInput : public Input {
public:
  virtual ~ZncInput() {};

  Message process(const std::string &input) const override;
};