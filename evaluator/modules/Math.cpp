#include "Math.hpp"
#include "../EvaluateError.hpp"
#include <random>

namespace soviet {
    MathModule::MathModule(Evaluator& evaluator) : Module{evaluator} {
        variables["sin"] = std::make_shared<FunctionValue>(&sin);
        variables["cos"] = std::make_shared<FunctionValue>(&cos);
        variables["sqrt"] = std::make_shared<FunctionValue>(&sqrt);
        variables["pow"] = std::make_shared<FunctionValue>(&pow);
        variables["max"] = std::make_shared<FunctionValue>(&max);
        variables["random"] = std::make_shared<FunctionValue>(&random);
        variables["mod"] = std::make_shared<FunctionValue>(&mod);

        variables["pi"] = std::make_shared<NumberValue>(3.14159265358979323846f);
        variables["e"] = std::make_shared<NumberValue>(2.71828182845904523536f);
    }

    std::shared_ptr<Value> MathModule::sin(Evaluator&, std::vector<std::shared_ptr<Value>>& args) {
        const auto arg = valueCast<NumberValue>(args[0])->value;
        return std::make_shared<NumberValue>(sinf(arg));
    }

    std::shared_ptr<Value> MathModule::cos(Evaluator&, std::vector<std::shared_ptr<Value>>& args) {
        const auto arg = valueCast<NumberValue>(args[0])->value;
        return std::make_shared<NumberValue>(cosf(arg));
    }

    std::shared_ptr<Value> MathModule::sqrt(Evaluator&, std::vector<std::shared_ptr<Value>>& args) {
        const auto arg = valueCast<NumberValue>(args[0])->value;
        return std::make_shared<NumberValue>(sqrtf(arg));
    }

    std::shared_ptr<Value> MathModule::pow(Evaluator&, std::vector<std::shared_ptr<Value>>& args) {
        const auto base = valueCast<NumberValue>(args[0])->value;
        const auto index = valueCast<NumberValue>(args[1])->value;
        return std::make_shared<NumberValue>(powf(base, index));
    }

    std::shared_ptr<Value> MathModule::max(Evaluator&, std::vector<std::shared_ptr<Value>>& args) {
        const auto values = valueCast<ArrayValue>(args[0]);
        if (values->size() == 0) {
            return std::make_shared<UndefinedValue>();
        }

        float max = valueCast<NumberValue>(values->at(0))->value;
        for (size_t i = 1; i < values->size(); ++i) {
            float value = valueCast<NumberValue>(values->at(i))->value;
            if (value > max)
                max = value;
        }

        return std::make_shared<NumberValue>(max);
    }

    std::shared_ptr<Value> MathModule::random(Evaluator&, std::vector<std::shared_ptr<Value>>& args) {
        std::random_device dev;
        std::mt19937 rng(dev());

        if (args.size() == 0) {
            std::uniform_real_distribution<> dist(0.0, 1.0);
            return std::make_shared<NumberValue>((float) dist(rng));
        } else if (args.size() == 2) {
            if (args[0]->type != ValueType::NumberValue || args[1]->type != ValueType::NumberValue)
                throw EvaluateError("Math.random accepts either 0 or 2 number arguments");

            float min = valueCast<NumberValue>(args[0])->value;
            float max = valueCast<NumberValue>(args[1])->value;
            std::uniform_real_distribution<> dist(min, max);
            return std::make_shared<NumberValue>((float) dist(rng));
        }

        throw EvaluateError("Math.random accepts either 0 or 2 number arguments");
    }

	std::shared_ptr<Value> MathModule::mod(Evaluator&, std::vector<std::shared_ptr<Value>>& args) {
		const auto left = valueCast<NumberValue>(args[0])->value;
		const auto right = valueCast<NumberValue>(args[1])->value;
		return std::make_shared<NumberValue>(std::fmod(left, right));
	}
}