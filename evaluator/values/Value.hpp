#pragma once
#include <memory>
#include "ValueType.hpp"

namespace soviet {
    class Value {
    public:
        ValueType type;

        explicit Value(ValueType type) : type(type) {}

        virtual bool equals(const std::shared_ptr<Value>&) = 0;
        virtual std::shared_ptr<Value> clone() = 0;
        virtual std::string dump() const = 0;
    };

    class UndefinedValue : public Value {
    public:
        UndefinedValue() : Value{ValueType::UndefinedValue} {}

        bool equals(const std::shared_ptr<Value>& other) {
            return other->type == type;
        }

        std::shared_ptr<Value> clone() override {
            return std::make_shared<UndefinedValue>();
        }

        std::string dump() const override {
            return "<undefined>";
        }
    };

	class NullValue : public Value {
	public:
		NullValue() : Value{ValueType::NullValue} {}

		bool equals(const std::shared_ptr<Value>& other) {
			return other->type == type;
		}

		std::shared_ptr<Value> clone() override {
			return std::make_shared<NullValue>();
		}

		std::string dump() const override {
			return "<null>";
		}
	};
}