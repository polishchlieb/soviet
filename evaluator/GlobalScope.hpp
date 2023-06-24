#pragma once
#include "Scope.hpp"
#include "modules/Module.hpp"
#include "modules/Array.hpp"
#include "modules/IO.hpp"
#include "modules/Map.hpp"
#include "modules/String.hpp"
#include "modules/Math.hpp"
#include "modules/ThreadModule.hpp"

namespace soviet {
    class GlobalScope : public Scope {
    public:
        GlobalScope(Evaluator& evaluator);
    };
}
