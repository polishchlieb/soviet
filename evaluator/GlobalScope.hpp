#pragma once
#include "Scope.hpp"

namespace soviet {
    class GlobalScope : public Scope {
    public:
        GlobalScope(Evaluator& evaluator);
    };
}
