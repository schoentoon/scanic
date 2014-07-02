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
  void analyze(const Generator &generator) {
    std::map<std::string, int64_t> lines;
    for (auto &msg : generator.messages()) ++lines[msg.author()];

    for (auto &l : lines) _result[l.first] = l.second;
  };
};