#ifndef SOVIET_ARRAYVALUE_HPP
#define SOVIET_ARRAYVALUE_HPP

#include <vector>
#include "Value.hpp"
#include <memory>

namespace soviet {
    struct ArrayValue : Value {
    private:
        typedef std::vector<std::shared_ptr<Value>> Data;
    public:
        explicit ArrayValue(Data data)
            : Value{ValueType::ArrayValue}, data(std::move(data))
        {}

        ArrayValue() : Value{ValueType::ArrayValue} {}

        auto add(std::shared_ptr<Value> value) {
            data.push_back(std::move(value));
        }

        auto& at(const unsigned int index) {
            return data[index];
        }

        auto& getData() {
            return data;
        }
    private:
        Data data;
    };
}

#endif //SOVIET_ARRAYVALUE_HPP