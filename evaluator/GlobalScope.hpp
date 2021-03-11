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
#include "modules/String.hpp"

namespace soviet {
    class GlobalScope : public Scope {
    public:
        GlobalScope() {
            modules.insert({"Array", new ArrayModule});
            modules.insert({"Map", new MapModule});
            modules.insert({"IO", new IOModule});
            modules.insert({"Misc", new MiscModule});
            modules.insert({"String", new StringModule});
        }
    };
}

#endif //SOVIET_GLOBALSCOPE_HPP
