#include "analyzer.h"

#include <dlfcn.h>

Analyzer::Analyzer(void* handle)
: _handle(handle) {
};

Analyzer::~Analyzer() {
  // We probably SHOULD be closing this, but if we do we for some reason get a segfault :(
  // I think we are not allowed to close the shared library that we are in ourself
  //if (_handle) dlclose(_handle);
};