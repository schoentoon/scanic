/* * -----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Toon Schoenmakers <nighteyes1993@gmail.com>
 * wrote this file. As long as you retain this notice you can do whatever you
 * want with this stuff. If we meet some day, and you think this stuff is worth
 * it, you can buy us a beer in return.
 * -----------------------------------------------------------------------------
 */

#include "analyzer.h"

#include "generator.h"

class LineCountAnalyzer : public Analyzer {
public:
  LineCountAnalyzer(void *handle) : Analyzer(handle) {};
  virtual ~LineCountAnalyzer() {};
  /**
   *  Return the name of your analyzer
   */
  const std::string name() const { return "linecount"; };

  /**
   *  Override this method to actually analyze the data
   */
  std::map<std::string, Variant::Value> analyze(const Generator &generator) {
    std::map<std::string, int64_t> lines;
    for (auto &msg : generator.messages()) ++lines[msg.author()];

    std::map<std::string, Variant::Value> output;
    for (auto &l : lines) output[l.first] = l.second;

    return output;
  };
};

ANALYZEMODULE(LineCountAnalyzer);