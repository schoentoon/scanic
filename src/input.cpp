/* * -----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Toon Schoenmakers <nighteyes1993@gmail.com>
 * wrote this file. As long as you retain this notice you can do whatever you
 * want with this stuff. If we meet some day, and you think this stuff is worth
 * it, you can buy us a beer in return.
 * -----------------------------------------------------------------------------
 */

#include "input.h"

#include <iostream>

namespace Scanic {

Input::Input(const std::shared_ptr<Generator> &generator)
: _generator(generator) {
};

Input::~Input() {
  _generator->insertInput(this);
};

};