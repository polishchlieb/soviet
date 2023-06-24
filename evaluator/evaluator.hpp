#pragma once
#include <cmath>
#include <memory>
#include "../parser/nodes.hpp"
#include "../parser/nodeCast.hpp"
#include "values/values.hpp"
#include "EvaluateError.hpp"
#include "../parser/dumpNode.hpp"
#include <unordered_map>
#include <cfloat>
#include "Scope.hpp"
#include "GlobalScope.hpp"
#include <fstream>
#include "../file.hpp"

namespace soviet {
	class Thread;

	class Evaluator {
	public:
		Evaluator(Thread& thread);

		std::shared_ptr<Value> evaluate(const std::shared_ptr<Node>& node);
		std::shared_ptr<Value> callFunction(const std::shared_ptr<FunctionValue>& function, std::vector<std::shared_ptr<Value>>& arguments);

		Thread& thread;
	private:
		std::vector<std::shared_ptr<Scope>> currentContext;

		template<typename T>
		auto getNumberValues(const std::shared_ptr<T>& node)
			-> std::tuple<float, float> {
			const auto n = nodeCast<OperatorNode>(node);

			const auto left = evaluate(n->left);
			const auto right = evaluate(n->right);

			if (left->type != ValueType::NumberValue || right->type != ValueType::NumberValue)
				throw EvaluateError("Unknown operands");

			return {
				valueCast<NumberValue>(left)->value,
				valueCast<NumberValue>(right)->value
			};
		}

		std::shared_ptr<ArrayValue> destructure(const std::shared_ptr<ArrayNode>& left, const std::shared_ptr<ArrayValue>& right);
		std::shared_ptr<Value> setVariable(const std::string& name, std::shared_ptr<Value> value);

		std::shared_ptr<Value> evaluateBinOpNode(const std::shared_ptr<Node>& node);
		std::shared_ptr<Value> evaluateImportNode(const std::shared_ptr<Node>& node);
		std::shared_ptr<Value> evaluateLessThanOpNode(const std::shared_ptr<BinOpNode>& node);
		std::shared_ptr<Value> evaluateArrayNode(const std::shared_ptr<Node>& node);
		std::shared_ptr<Value> evaluateGreaterThanOrEqualOpNode(const std::shared_ptr<BinOpNode>& node);
		std::shared_ptr<Value> evaluateLessThanOrEqualOpNode(const std::shared_ptr<BinOpNode>& node);
		std::shared_ptr<Value> evaluateNegationNode(const std::shared_ptr<Node>& node);
		std::shared_ptr<Value> evaluateGreaterThanOpNode(const std::shared_ptr<BinOpNode>& node);
		std::shared_ptr<Value> evaluateNumberNode(const std::shared_ptr<Node>& node);
		std::shared_ptr<Value> evaluateReturnNode(const std::shared_ptr<Node>& node);
		std::shared_ptr<Value> evaluateNameNode(const std::shared_ptr<Node>& node);
		std::shared_ptr<Value> evaluateStringNode(const std::shared_ptr<Node>& node);
		std::shared_ptr<Value> evaluateAddOpNode(const std::shared_ptr<BinOpNode>& node);
		std::shared_ptr<Value> evaluateIfNode(const std::shared_ptr<Node>& node);
		std::shared_ptr<Value> evaluateWhileLoopNode(const std::shared_ptr<Node>& node);
		std::shared_ptr<Value> evaluateSubOpNode(const std::shared_ptr<BinOpNode>& node);
		std::shared_ptr<Value> evaluateMulOpNode(const std::shared_ptr<BinOpNode>& node);
		std::shared_ptr<Value> evaluateDivOpNode(const std::shared_ptr<BinOpNode>& node);
		std::shared_ptr<Value> evaluateEqualsOpNode(const std::shared_ptr<BinOpNode>& node);
		std::shared_ptr<Value> evaluateDoubleEqualsOpNode(const std::shared_ptr<BinOpNode>& node);
		std::shared_ptr<Value> evaluateNotEqualsOpNode(const std::shared_ptr<BinOpNode>& node);
		std::shared_ptr<Value> evaluateFuncCallNode(const std::shared_ptr<Node>& node);
		std::shared_ptr<Value> evaluatePrototypeNode(const std::shared_ptr<Node>& node);
		std::shared_ptr<Value> evaluateBlockNode(const std::shared_ptr<Node>& node);
		std::shared_ptr<Value> evaluateBooleanNode(const std::shared_ptr<Node>& node);
		std::shared_ptr<Value> evaluateDotOpNode(const std::shared_ptr<BinOpNode>& node);
		std::shared_ptr<Value> evaluateModuleNode(const std::shared_ptr<Node>& node);
		std::shared_ptr<Value> evaluatePipeOpNode(const std::shared_ptr<Node>& node);
		std::shared_ptr<Value> evaluateForLoopNode(const std::shared_ptr<Node>& node);
	};
}
