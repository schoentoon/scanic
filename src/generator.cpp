#include "generator.h"

#include "input.h"

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

Variant::Value Generator::analyze() {
  std::map<std::string, Variant::Value> output;
  for (auto &analyzer : _analyzers) {
    std::chrono::time_point<std::chrono::system_clock> start(std::chrono::system_clock::now());
    analyzer->analyze(*this);
    output[analyzer->name()] = analyzer->analyze(*this);
    std::chrono::duration<double> elapsed_seconds(std::chrono::system_clock::now() - start);
    output["timing"][analyzer->name()] = elapsed_seconds.count();
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