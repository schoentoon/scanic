#pragma once

#include "input.h"

class ZncInput : public Input {
public:
  ZncInput(const std::shared_ptr<Generator> &generator)
  : Input(std::move(generator)) {
  };

  virtual ~ZncInput() {};

  void process(const std::string &input, const std::string &path) override;
};