#pragma once

#include <variant.h>

class Generator;

class Analyzer {
protected:
  /**
   *  Store your actual results in here.
   */
  std::map<std::string, Variant::Value> _result;
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
  virtual void analyze(const Generator &generator) = 0;

  const std::map<std::string, Variant::Value> &result() const { return _result; };
};