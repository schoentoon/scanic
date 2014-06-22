#include "input.h"

#include <iostream>

Input::Input(const std::shared_ptr<Generator> &generator)
: _generator(generator) {
};

void Input::publishMessage(Message &&message) {
  _generator->publishMessage(std::move(message));
};

void Input::publishJoin(std::tm &&when, std::string &&username) {
  _generator->publishJoin(std::move(when), std::move(username));
};