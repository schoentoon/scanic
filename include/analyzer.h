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
#include <libconfig.h++>

#define ANALYZEMODULE(classname, human_name) extern "C" Scanic::Analyzer* loadAnalyzer(void *handle) { return new classname(handle, human_name); }

namespace Scanic {

class Generator;

class Analyzer {
private:
  /**
   * Handle to the shared library that should be closed upon destruction
   */
  void* _handle;

  /**
   * The name of the actual analyzer
   */
  std::string _name;

  /**
   * Generator is our friend so he can unload us before deleting us
   */
  friend class Generator;

public:
  Analyzer(void *handle, const char *name);
  Analyzer(const Analyzer& that) = delete;
  Analyzer* operator=(const Analyzer& that) = delete;
  virtual ~Analyzer();
  /**
   *  Return the name of your analyzer
   */
  const std::string &name() const { return _name; };

  /**
   *  Overload this method to load analyzers specify settings
   *  @return  false in case you don't agree with the config that is coming in
   */
  virtual bool onConfig(const libconfig::Setting &setting) { return true; };

  /**
   *  Override this method to actually analyze the data
   */
  virtual SmartTpl::VariantValue analyze(const Generator &generator) = 0;

  /**
   * Return true from this method if your analyzer requires sorted data, if not
   * a single analyzer requires sorted data we simply skip the sorting step
   */
  virtual bool requiresSorting() const { return false; };
};

};
