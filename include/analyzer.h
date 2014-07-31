#pragma once

#include <variant.h>

#define ANALYZEMODULE(classname) extern "C" Analyzer* loadAnalyzer(void *handle) { return new classname(handle); }

class Generator;

class Analyzer {
private:
  /**
   * Handle to the shared library that should be closed upon destruction
   */
  void* _handle;

  /**
   * Generator is our friend so he can unload us before deleting us
   */
  friend class Generator;

public:
  Analyzer(void *handle);
  virtual ~Analyzer();
  /**
   *  Return the name of your analyzer
   */
  virtual const std::string name() const = 0;

  /**
   *  Override this method to actually analyze the data
   */
  virtual std::map<std::string, Variant::Value> analyze(const Generator &generator) = 0;
};