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
  std::vector<std::pair<std::string, int64_t> >::const_iterator _iter;

  std::vector<std::pair<std::string, int64_t> >::const_iterator _end;

public:
  SortedByCountIterator(
      const std::vector<std::pair<std::string, int64_t> >::const_iterator &iter,
      const std::vector<std::pair<std::string, int64_t> >::const_iterator &end)
      : _iter(iter), _end(end) {}

  virtual ~SortedByCountIterator() {};

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

class SortedByCountValue : public SmartTpl::Value {
private:
  std::vector<std::pair<std::string, int64_t> > _value;

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

  virtual ~SortedByCountValue() {};

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
    return new SortedByCountIterator(_value.begin(), _value.end());
  };
};

class SmileyCountAnalyzer : public Analyzer {
private:
  std::vector<std::string> _smilies;

public:
  SmileyCountAnalyzer(void *handle, const char *name)
      : Analyzer(handle, name),
        _smilies({ ":D", ":)", ":(", ";)", "\\o/", "\\o", "o/", "^_^" }) {};
  virtual ~SmileyCountAnalyzer() {};

  /**
  *  Overload this method to load analyzers specify settings
  *  @return  false in case you don't agree with the config that is coming in
  */
  bool onConfig(const libconfig::Setting &setting) override {
    std::cerr << "onConfig()" << std::endl;
    return true;
  };

  /**
   *  Override this method to actually analyze the data
   */
  SmartTpl::VariantValue analyze(const Generator &generator) {
    std::map<std::string, int64_t> output;

    for (auto &msg : generator.messages()) {
      for (auto &smiley : _smilies) {
        const std::string &msgstr = msg.message();
        std::string::size_type pos = msgstr.find(smiley);

        while (pos != std::string::npos) {
          ++output[smiley];
          pos = msgstr.find(smiley, pos + 1);
        }
      }
    }

    return SmartTpl::VariantValue(std::make_shared<SortedByCountValue>(output));
  };
};
};

ANALYZEMODULE(Scanic::SmileyCountAnalyzer, "smileycount");
