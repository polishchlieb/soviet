#pragma once
#include <vector>
#include "Value.hpp"
#include <memory>

namespace soviet {
    class ArrayValue : public Value {
    private:
        typedef std::vector<std::shared_ptr<Value>> Data;
    public:
        explicit ArrayValue(Data data);
        ArrayValue();

		bool equals(const std::shared_ptr<Value>& other) override;
		std::shared_ptr<Value> clone() override;
        std::string dump() const override;

        void add(std::shared_ptr<Value> value);
        inline std::shared_ptr<Value>& at(size_t index);
        inline Data& getData();
        size_t size() const;
        void removeAt(size_t index);
        void remove(const std::shared_ptr<Value>& element);
        void concat(const std::shared_ptr<ArrayValue>& other);
        void reverse();
    private:
        Data data;
    };
}