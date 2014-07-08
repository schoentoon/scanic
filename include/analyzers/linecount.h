#pragma once

#include "analyzer.h"

class LineCountAnalyzer : public Analyzer {
public:
  LineCountAnalyzer() : Analyzer() {};
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