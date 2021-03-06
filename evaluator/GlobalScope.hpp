#ifndef SOVIET_GLOBALSCOPE_HPP
#define SOVIET_GLOBALSCOPE_HPP

#include "Scope.hpp"
#include "values/FunctionValue.hpp"
#include "dumpValue.hpp"
#include "EvaluateError.hpp"

#include <array>

#include "modules/Module.hpp"
#include "modules/Array.hpp"
#include "modules/IO.hpp"
#include "modules/Map.hpp"
#include "modules/Misc.hpp"

namespace soviet {
    class GlobalScope : public Scope {
    public:
        GlobalScope() {
            const std::array<Module*, 4> modules =
                {new ArrayModule, new IOModule, new MapModule, new MiscModule};
            for (const auto& m : modules)
                this->merge(m);
        }
    };
}

#endif //SOVIET_GLOBALSCOPE_HPP
