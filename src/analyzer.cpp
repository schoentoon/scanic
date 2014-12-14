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

#include <dlfcn.h>

namespace Scanic {

Analyzer::Analyzer(void *handle, const char *name)
    : _handle(handle), _name(name){};

Analyzer::~Analyzer(){
    // We probably SHOULD be closing this, but if we do we
    // for some reason get a segfault :(
    // I think we are not allowed to close the shared
    // library that we are in ourself
    // if (_handle) dlclose(_handle);
};
};
