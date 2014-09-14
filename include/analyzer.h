/* * -----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Toon Schoenmakers <nighteyes1993@gmail.com>
 * wrote this file. As long as you retain this notice you can do whatever you
 * want with this stuff. If we meet some day, and you think this stuff is worth
 * it, you can buy us a beer in return.
 * -----------------------------------------------------------------------------
 */

#pragma once

#include <map>
#include <memory>
#include <smarttpl.h>

#define ANALYZEMODULE(classname) extern "C" Scanic::Analyzer* loadAnalyzer(void *handle) { return new classname(handle); }

namespace Scanic {

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
  Analyzer(const Analyzer& that) = delete;
  Analyzer* operator=(const Analyzer& that) = delete;
  virtual ~Analyzer();
  /**
   *  Return the name of your analyzer
   */
  virtual const std::string name() const = 0;

  /**
   *  Override this method to actually analyze the data
   */
  virtual std::shared_ptr<SmartTpl::Value> analyze(const Generator &generator) = 0;

  /**
   * Return true from this method if your analyzer requires sorted data, if not
   * a single analyzer requires sorted data we simply skip the sorting step
   */
  virtual bool requiresSorting() const { return false; };
};

};
