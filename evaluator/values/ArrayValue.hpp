#ifndef SOVIET_ARRAYVALUE_HPP
#define SOVIET_ARRAYVALUE_HPP

#include <vector>
#include "Value.hpp"
#include <memory>

namespace soviet {
    class ArrayValue : public Value {
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

        size_t size() const {
            return data.size();
        }

        bool equals(const std::shared_ptr<Value>& other) override {
            if (other->type != this->type)
                return false;
            const auto otherValue = valueCast<ArrayValue>(other);
            if (this->size() != otherValue->size())
                return false;
            for (unsigned int i = 0; i < this->size(); ++i) {
                if (!this->at(i)->equals(otherValue->at(i)))
                    return false;
            }
            return true;
        }

        std::shared_ptr<Value> clone() override {
            auto result = std::make_shared<ArrayValue>();
            for (const auto& element : data)
                result->add(element->clone());
            return result;
        }

        void removeAt(unsigned int index) {
            data.erase(data.begin() + index);
        }

        void remove(const std::shared_ptr<Value>& element) {
            const auto iterator = std::find_if(
                data.begin(), data.end(),
                [&element](const std::shared_ptr<Value>& curr) {
                    return curr->equals(element);
                }
            );
            if (iterator != data.end())
                data.erase(iterator);
        }
    private:
        Data data;
    };
}

#endif //SOVIET_ARRAYVALUE_HPP