#pragma once
#include <memory>
#include "Value.hpp"

namespace soviet {
    class ExplicitReturnValue : public Value {
    public:
        std::shared_ptr<Value> value;

        explicit ExplicitReturnValue(std::shared_ptr<Value> value);

        bool equals(const std::shared_ptr<Value>& other) override;
        std::shared_ptr<Value> clone() override;
        std::string dump() const override;
    };
}
