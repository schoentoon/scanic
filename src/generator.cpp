/* *
 * -----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Toon Schoenmakers <nighteyes1993@gmail.com>
 * wrote this file. As long as you retain this notice you can do whatever you
 * want with this stuff. If we meet some day, and you think this stuff is worth
 * it, you can buy us a beer in return.
 * -----------------------------------------------------------------------------
 */

#include "generator.h"

#include <future>
#include <queue>

#include <dlfcn.h>

#include "input.h"

namespace Scanic {

Generator::Generator()
    : _messages(), _joins(), _quits(), _parts(), _kicks(), _nick_changes(),
      _lock(), _analyzers(){};

std::chrono::duration<double> Generator::sort() {
  std::chrono::time_point<std::chrono::system_clock> start(
      std::chrono::system_clock::now());
  _messages.sort(Event::compare);
  _joins.sort(Event::compare);
  _quits.sort(Event::compare);
  _parts.sort(Event::compare);
  _kicks.sort(Event::compare);
  _nick_changes.sort(Event::compare);
  return std::chrono::system_clock::now() - start;
};

SmartTpl::Data Generator::analyze(bool no_threads) {

  struct AnalyzeData {
    AnalyzeData(Analyzer *analyzer, Generator &generator)
        : _start(std::chrono::system_clock::now()), _name(analyzer->name()),
          _data(analyzer->analyze(generator)),
          _duration(std::chrono::duration<double>(
                        std::chrono::system_clock::now() - _start).count()){};
    // This property is purely so we can measure the entire lifetime of the
    // constructor
    std::chrono::time_point<std::chrono::system_clock> _start;

    const std::string &_name;
    SmartTpl::VariantValue _data;
    double _duration;
  };

  std::queue<std::future<AnalyzeData>> futures;
  SmartTpl::Data output;
  for (auto &analyzer : _analyzers) {
    auto func = [&analyzer, this]() {
      AnalyzeData output(analyzer.get(), *this);
      return output;
    };
    if (no_threads) {
      std::packaged_task<AnalyzeData()> task(func);
      futures.push(std::move(task.get_future()));
      task();
    } else
      futures.emplace(std::async(std::launch::async, func));
  };
  std::map<std::string, SmartTpl::VariantValue> timings;
  while (!futures.empty()) {
    auto &future = futures.front();
    auto ret = future.get();
    output.assign(ret._name, ret._data);
    timings.insert(std::make_pair(ret._name, ret._duration));
    futures.pop();
  };
  output.assign("timing", timings);
  return output;
};

void Generator::insertInput(Input *input) {
  std::lock_guard<std::mutex> locked(_lock);
  _messages.splice(_messages.begin(), std::move(input->_messages));
  _joins.splice(_joins.begin(), std::move(input->_joins));
  _quits.splice(_quits.begin(), std::move(input->_quits));
  _parts.splice(_parts.begin(), std::move(input->_parts));
  _kicks.splice(_kicks.begin(), std::move(input->_kicks));
  _nick_changes.splice(_nick_changes.begin(), std::move(input->_nick_changes));
};

void Generator::loadAnalyzer(Analyzer *analyzer) {
  _analyzers.emplace_back(analyzer, [](Analyzer *analyzer) {
    void *handle = analyzer->_handle;
    delete analyzer;
    dlclose(handle);
  });
};
};
