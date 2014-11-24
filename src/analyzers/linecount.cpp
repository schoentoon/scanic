/* *
 * -----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Toon Schoenmakers <nighteyes1993@gmail.com>
 * wrote this file. As long as you retain this notice you can do whatever you
 * want with this stuff. If we meet some day, and you think this stuff is worth
 * it, you can buy us a beer in return.
 * -----------------------------------------------------------------------------
 */

#include "analyzer.h"

#include "generator.h"

namespace Scanic {

class SortedByLinesIterator : public SmartTpl::Iterator {
private:
  std::vector<std::pair<std::string, int64_t> >::const_iterator _iter;

  std::vector<std::pair<std::string, int64_t> >::const_iterator _end;

public:
  SortedByLinesIterator(
      const std::vector<std::pair<std::string, int64_t> >::const_iterator &iter,
      const std::vector<std::pair<std::string, int64_t> >::const_iterator &end)
      : _iter(iter), _end(end) {}

  virtual ~SortedByLinesIterator() {};

  bool valid() const override {
    return _iter != _end;
  };

  void next() override {
    ++_iter;
  };

  SmartTpl::VariantValue value() const override {
    return _iter->second;
  };

  SmartTpl::VariantValue key() const override {
    return _iter->first;
  };
};

class SortedByLinesValue : public SmartTpl::Value {
private:
  std::vector<std::pair<std::string, int64_t> > _value;

public:
  SortedByLinesValue(const std::map<std::string, int64_t> &input) : _value() {
    for (auto &line : input)
      _value.push_back(line);

    std::sort(
        _value.begin(), _value.end(),
        [](std::pair<std::string, int64_t> a,
           std::pair<std::string, int64_t> b) { return a.second > b.second; });
  };

  virtual ~SortedByLinesValue() {};

  std::string toString() const override {
    return "";
  };

  numeric_t toNumeric() const override {
    return 0;
  };

  bool toBoolean() const override {
    return false;
  };

  double toDouble() const override {
    return 0.0;
  };

  SmartTpl::VariantValue member(const char *name, size_t size) const override {
    return nullptr;
  }

  size_t memberCount() const override { return _value.size(); }

  SmartTpl::VariantValue member(size_t position) const override {
    return nullptr;
  }

  SmartTpl::Iterator *iterator() const override {
    return new SortedByLinesIterator(_value.begin(), _value.end());
  };
};

class LineCountAnalyzer : public Analyzer {
public:
  LineCountAnalyzer(void *handle, const char *name) : Analyzer(handle, name) {};
  virtual ~LineCountAnalyzer() {};

  /**
   *  Override this method to actually analyze the data
   */
  std::shared_ptr<SmartTpl::Value> analyze(const Generator &generator) {
    std::map<std::string, int64_t> lines;
    for (auto &msg : generator.messages())
      ++lines[msg.author()];

    return std::shared_ptr<SmartTpl::Value>(new SortedByLinesValue(lines));
  };
};
};

ANALYZEMODULE(Scanic::LineCountAnalyzer, "linecount");
