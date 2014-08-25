/* * -----------------------------------------------------------------------------
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

Generator::Generator() {
};

std::chrono::duration<double> Generator::sort() {
  std::chrono::time_point<std::chrono::system_clock> start(std::chrono::system_clock::now());
  _messages.sort(Event::compare);
  _joins.sort(Event::compare);
  _quits.sort(Event::compare);
  _parts.sort(Event::compare);
  _kicks.sort(Event::compare);
  _nick_changes.sort(Event::compare);
  return std::chrono::system_clock::now() - start;
};

Variant::Value Generator::analyze(bool no_threads) {
  using AnalyzeData = std::tuple<std::string, Variant::Value, double>;
  std::queue<std::future<AnalyzeData>> futures;
  std::map<std::string, Variant::Value> output;
  for (auto &analyzer : _analyzers) {
    auto func = [&analyzer, this]() {
      AnalyzeData output;
      std::chrono::time_point<std::chrono::system_clock> start(std::chrono::system_clock::now());
      std::get<0>(output) = analyzer->name();
      std::get<1>(output) = analyzer->analyze(*this);
      std::chrono::duration<double> elapsed_seconds(std::chrono::system_clock::now() - start);
      std::get<2>(output) = elapsed_seconds.count();
      return output;
    };
    if (no_threads) {
      std::packaged_task<AnalyzeData()> task(func);
      futures.push(std::move(task.get_future()));
      task();
    } else futures.emplace(std::async(std::launch::async, func));
  };
  while (!futures.empty()) {
    auto &future = futures.front();
    auto ret = future.get();
    output[std::get<0>(ret)] = std::get<1>(ret);
    output["timing"][std::get<0>(ret)] = std::get<2>(ret);
    futures.pop();
  };
  return output;
};

void Generator::insertInput(Input* input) {
  std::lock_guard<std::mutex> locked(_lock);
  _messages.splice(_messages.begin(), std::move(input->_messages));
  _joins.splice(_joins.begin(), std::move(input->_joins));
  _quits.splice(_quits.begin(), std::move(input->_quits));
  _parts.splice(_parts.begin(), std::move(input->_parts));
  _kicks.splice(_kicks.begin(), std::move(input->_kicks));
  _nick_changes.splice(_nick_changes.begin(), std::move(input->_nick_changes));
};

void Generator::loadAnalyzer(Analyzer* analyzer) {
  _analyzers.emplace_back(analyzer, [](Analyzer *analyzer) {
    void *handle = analyzer->_handle;
    delete analyzer;
    dlclose(handle);
  });
};

};