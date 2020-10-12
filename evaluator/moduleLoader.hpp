#ifndef SOVIET_MODULELOADER_HPP
#define SOVIET_MODULELOADER_HPP

#include <dlfcn.h>
#include "Scope.hpp"
#include "../tokenizer/FileReadError.hpp"

auto loadModule(const char* path) -> soviet::Scope* {
    auto handle = dlopen(path, RTLD_LAZY);
    if (!handle)
        throw soviet::FileReadError("Unknown module");

    auto create = reinterpret_cast<soviet::Scope* (*)()>(
        dlsym(handle, "createObject")
    );
    return create();
}

#endif //SOVIET_MODULELOADER_HPP
