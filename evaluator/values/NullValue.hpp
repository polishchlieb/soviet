#pragma once
#include "Value.hpp"

namespace soviet {
    class NullValue : public Value {
    public:
        explicit NullValue();

        bool equals(const std::shared_ptr<Value>& other) override;
        std::shared_ptr<Value> clone() override;
        std::string dump() const override;
    };
}