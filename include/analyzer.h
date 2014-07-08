#pragma once

#include <variant.h>

class Generator;

class Analyzer {
public:
  Analyzer() {};
  virtual ~Analyzer() {};
  /**
   *  Return the name of your analyzer
   */
  virtual const std::string name() const = 0;

  /**
   *  Override this method to actually analyze the data
   */
  virtual std::map<std::string, Variant::Value> analyze(const Generator &generator) = 0;
};