#include "String.hpp"

namespace soviet {
    std::shared_ptr<Value> StringModule::split(Evaluator& evaluator, std::vector<std::shared_ptr<Value>>& args) {
        const auto& str = valueCast<StringValue>(args[0])->value;
        const auto& delimiter = valueCast<StringValue>(args[1])->value;

        auto result = std::make_shared<ArrayValue>();
        size_t start = 0;
        auto end = str.find(delimiter);
        while (end != std::string::npos) {
            std::string element = str.substr(start, end - start);
            result->add(std::make_shared<StringValue>(element));
            start = end + delimiter.length();
            end = str.find(delimiter, start);
        }
        std::string element = str.substr(start, end);
        result->add(std::make_shared<StringValue>(element));

        return result;
    }

    std::shared_ptr<Value> StringModule::at(Evaluator& evaluator, std::vector<std::shared_ptr<Value>>& args) {
        const auto& str = valueCast<StringValue>(args[0])->value;
        const auto index = static_cast<size_t>(valueCast<NumberValue>(args[1])->value);

        const auto result = std::string(1, str[index]);
        return std::make_shared<StringValue>(result);
    }

    std::shared_ptr<Value> StringModule::slice(Evaluator& evaluator, std::vector<std::shared_ptr<Value>>& args) {
        const auto& str = valueCast<StringValue>(args[0])->value;
        const auto start = static_cast<size_t>(valueCast<NumberValue>(args[1])->value);
        const auto end = args.size() >= 3 ? static_cast<size_t>(valueCast<NumberValue>(args[2])->value) : str.length();

        return std::make_shared<StringValue>(str.substr(start, end));
    }

    std::shared_ptr<Value> StringModule::length(Evaluator& evaluator, std::vector<std::shared_ptr<Value>>& args) {
        const auto str = valueCast<StringValue>(args[0]);
        return std::make_shared<NumberValue>((float) str->length());
    }

    std::shared_ptr<Value> StringModule::starts_with(Evaluator& evaluator, std::vector<std::shared_ptr<Value>>& args) {
        const auto& str = valueCast<StringValue>(args[0])->value;
        const auto& substr = valueCast<StringValue>(args[1])->value;

        for (unsigned int i = 0; i < substr.length(); ++i) {
            if (str[i] != substr[i])
                return std::make_shared<BooleanValue>(false);
        }

        return std::make_shared<BooleanValue>(true);
    }

    std::shared_ptr<Value> StringModule::ends_with(Evaluator& evaluator, std::vector<std::shared_ptr<Value>>& args) {
        const auto& str = valueCast<StringValue>(args[0])->value;
        const auto& substr = valueCast<StringValue>(args[1])->value;

        for (int i = (int) str.length() - 1, j = (int) substr.length() - 1; j >= 0; --i, --j) {
            if (str[i] != substr[j])
                return std::make_shared<BooleanValue>(false);
        }

        return std::make_shared<BooleanValue>(true);
    }

    std::shared_ptr<Value> StringModule::contains(Evaluator& evaluator, std::vector<std::shared_ptr<Value>>& args) {
        const auto& str = valueCast<StringValue>(args[0])->value;
        const auto& substr = valueCast<StringValue>(args[1])->value;
        return std::make_shared<BooleanValue>(str.find(substr) != std::string::npos);
    }

    std::shared_ptr<Value> StringModule::replace_all(Evaluator& evaluator, std::vector<std::shared_ptr<Value>>& args) {
        const auto str = valueCast<StringValue>(args[0]->clone());
        const auto& substr = valueCast<StringValue>(args[1])->value;
        const auto& replaceWith = valueCast<StringValue>(args[2])->value;

        str->replaceAll(substr, replaceWith);
        return str;
    }

    std::shared_ptr<Value> StringModule::from_int(Evaluator& evaluator, std::vector<std::shared_ptr<Value>>& args) {
        const auto value = static_cast<int>(valueCast<NumberValue>(args[0])->value);
        return std::make_shared<StringValue>(std::to_string(value));
    }

    std::shared_ptr<Value> StringModule::index_of(Evaluator& evaluator, std::vector<std::shared_ptr<Value>>& args) {
        const auto& str = valueCast<StringValue>(args[0])->value;
        const auto& substr = valueCast<StringValue>(args[1])->value;
        return std::make_shared<NumberValue>((float) str.find(substr));
    }

    std::shared_ptr<Value> StringModule::to_upper(Evaluator& evaluator, std::vector<std::shared_ptr<Value>>& args) {
        const auto str = valueCast<StringValue>(args[0]->clone());
        str->toUpperCase();
        return str;
    }

    std::shared_ptr<Value> StringModule::to_lower(Evaluator& evaluator, std::vector<std::shared_ptr<Value>>& args) {
        const auto str = valueCast<StringValue>(args[0]->clone());
        str->toLowerCase();
        return str;
    }

    std::shared_ptr<Value> StringModule::characters(Evaluator& evaluator, std::vector<std::shared_ptr<Value>>& args) {
        const auto str = valueCast<StringValue>(args[0]);

        auto result = std::make_shared<ArrayValue>();
        for (const char c : str->value) {
            result->add(std::make_shared<StringValue>(std::string(1, c)));
        }

        return result;
    }

    std::shared_ptr<Value> StringModule::to_string(Evaluator& evaluator, std::vector<std::shared_ptr<Value>>& args) {
        return std::make_shared<StringValue>(args[0]->dump());
    }
}