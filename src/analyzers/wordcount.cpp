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

class SortedByWordCountIterator : public SmartTpl::Iterator {
private:
  std::vector<std::pair<std::string, int64_t>>::const_iterator _iter;

  std::vector<std::pair<std::string, int64_t>>::const_iterator _end;

public:
  SortedByWordCountIterator(
      const std::vector<std::pair<std::string, int64_t>>::const_iterator &iter,
      const std::vector<std::pair<std::string, int64_t>>::const_iterator &end)
      : _iter(iter), _end(end) {}

  virtual ~SortedByWordCountIterator(){};

  bool valid() const override { return _iter != _end; };

  void next() override { ++_iter; };

  SmartTpl::VariantValue value() const override { return _iter->second; };

  SmartTpl::VariantValue key() const override { return _iter->first; };
};

class SortedByWordCountValue : public SmartTpl::Value {
private:
  std::vector<std::pair<std::string, int64_t>> _value;

public:
  SortedByWordCountValue(const std::map<std::string, int64_t> &input) : _value() {
    for (auto &line : input)
      _value.push_back(line);

    std::sort(
        _value.begin(), _value.end(),
        [](std::pair<std::string, int64_t> a,
           std::pair<std::string, int64_t> b) { return a.second > b.second; });
  };

  virtual ~SortedByWordCountValue(){};

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
    return new SortedByWordCountIterator(_value.begin(), _value.end());
  };
};

class WordCountAnalyzer : public Analyzer {
public:
  WordCountAnalyzer(void *handle, const char *name) : Analyzer(handle, name){};
  virtual ~WordCountAnalyzer(){};

  /**
   *  Override this method to actually analyze the data
   */
  SmartTpl::VariantValue analyze(const Generator &generator) {
    std::map<std::string, int64_t> words;
    for (auto &msg : generator.messages()) {
      bool inWord = false;
      for (char c : msg.message()) {
        if (!inWord) {
          ++words[msg.author()];
          inWord = true;
        } else {
          if (!std::isalnum(c)) inWord = false;
        }
      }
    }

    return SmartTpl::VariantValue(std::make_shared<SortedByWordCountValue>(words));
  };
};
};

ANALYZEMODULE(Scanic::WordCountAnalyzer, "wordcount");
