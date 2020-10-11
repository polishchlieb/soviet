#ifndef SOVIET_MODULELOADER_HPP
#define SOVIET_MODULELOADER_HPP

#include <dlfcn.h>
#include "Scope.hpp"

auto loadModule(const char* path) -> soviet::Scope* {
    auto handle = dlopen(path, RTLD_LAZY);
    auto create = reinterpret_cast<soviet::Scope* (*)()>(
        dlsym(handle, "createObject")
    );
    return create();
}

#endif //SOVIET_MODULELOADER_HPP
