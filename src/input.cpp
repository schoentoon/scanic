#include "input.h"

#include <iostream>

Input::Input(const std::shared_ptr<Generator> &generator)
: _generator(generator) {
};

Input::~Input() {
  _generator->insertInput(this);
};