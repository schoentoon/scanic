#include "input_znc.h"

#include <iostream>

Message ZncInput::process(const std::string& input) const {
  // For now we'll just print the input
  std::cerr << input << std::endl;
  return Message();
}