/* * -----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Toon Schoenmakers <nighteyes1993@gmail.com>
 * wrote this file. As long as you retain this notice you can do whatever you
 * want with this stuff. If we meet some day, and you think this stuff is worth
 * it, you can buy us a beer in return.
 * -----------------------------------------------------------------------------
 */

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