#include "input.h"

#include <iostream>

Input::Input(const std::shared_ptr<Generator> &generator)
: _generator(generator) {
};

void Input::publishMessage(Message &&message) {
  _generator->publishMessage(std::move(message));
};

void Input::publishJoin(JoinEvent &&join) {
  _generator->publishJoin(std::move(join));
};

void Input::publishQuit(QuitEvent &&quit) {
  _generator->publishQuit(std::move(quit));
};