#include "input_znc.h"

#include <iostream>

Message ZncInput::process(const std::string& input, const std::string &path) const {
  // For now we'll just print the input
  std::cerr << input << std::endl;
  return Message();
}