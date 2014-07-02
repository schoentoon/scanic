#include "generator.h"

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
    analyzer->analyze(*this);
    output[analyzer->name()] = analyzer->result();
  };
  return output;
};