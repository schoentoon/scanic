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

class SortedByCountIterator : public SmartTpl::Iterator {
private:
  std::vector<std::pair<std::string, int64_t>>::const_iterator _iter;

  const std::vector<std::pair<std::string, int64_t>>::const_iterator _end;

public:
  SortedByCountIterator(
      const std::vector<std::pair<std::string, int64_t>>::const_iterator &iter,
      const std::vector<std::pair<std::string, int64_t>>::const_iterator &end)
      : _iter(iter), _end(end) {}

  virtual ~SortedByCountIterator(){};

  bool valid() const override { return _iter != _end; };

  void next() override { ++_iter; };

  SmartTpl::VariantValue value() const override { return _iter->second; };

  SmartTpl::VariantValue key() const override { return _iter->first; };
};

class SortedByCountValue : public SmartTpl::Value {
private:
  std::vector<std::pair<std::string, int64_t>> _value;

public:
  SortedByCountValue(const std::map<std::string, int64_t> &input) : _value() {
    for (auto &smiley : input) {
      _value.push_back(smiley);
    }

    std::sort(
        _value.begin(), _value.end(),
        [](std::pair<std::string, int64_t> a,
           std::pair<std::string, int64_t> b) { return a.second > b.second; });
  };

  virtual ~SortedByCountValue(){};

  std::string toString() const override { return ""; };

  numeric_t toNumeric() const override { return 0; };

  bool toBoolean() const override { return false; };

  double toDouble() const override { return 0.0; };

  SmartTpl::VariantValue member(const char *name, size_t size) const override {
    return nullptr;
  }

  size_t memberCount() const override { return _value.size(); }

  SmartTpl::VariantValue member(size_t position) const override {
    return nullptr;
  }

  SmartTpl::Iterator *iterator() const override {
    return new SortedByCountIterator(_value.begin(), _value.end());
  };
};

class CapsCountAnalyzer : public Analyzer {
private:
  /**
   *  The minimum amounts of caps we want to see
   */
  std::size_t min_characters = 3;

public:
  CapsCountAnalyzer(void *handle, const char *name) : Analyzer(handle, name){};
  virtual ~CapsCountAnalyzer(){};

  /**
   *  Override this method to actually analyze the data
   */
  SmartTpl::VariantValue analyze(const Generator &generator) {
    std::map<std::string, int64_t> output;

    for (auto &msg : generator.messages()) {
      std::size_t caps = 0;
      for (char c : msg.message()) {
        if (std::isalpha(c) && std::isupper(c))
          ++caps;
      }

      if (caps >= min_characters)
        ++output[msg.author()];
    }

    return SmartTpl::VariantValue(std::make_shared<SortedByCountValue>(output));
  };
};
};

ANALYZEMODULE(Scanic::CapsCountAnalyzer, "caps");
