#ifndef SOVIET_EVALUATOR_HPP
#define SOVIET_EVALUATOR_HPP

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

namespace soviet {
	class Thread;

	class Evaluator {
	public:
		Evaluator(Thread& thread) : thread(thread) {
			currentContext.push_back(std::make_shared<GlobalScope>(*this));
		}

		auto evaluate(const std::shared_ptr<Node>& node) {
			switch (node->type) {
			case NodeType::NumberNode:
				return evaluateNumberNode(node);
			case NodeType::NameNode:
				return evaluateNameNode(node);
			case NodeType::StringNode:
				return evaluateStringNode(node);
			case NodeType::FuncCallNode:
				return evaluateFuncCallNode(node);
			case NodeType::IfNode:
				return evaluateIfNode(node);
			case NodeType::PrototypeNode:
				return evaluatePrototypeNode(node);
			case NodeType::BlockNode:
				return evaluateBlockNode(node);
			case NodeType::ReturnNode:
				return evaluateReturnNode(node);
			case NodeType::NegationNode:
				return evaluateNegationNode(node);
			case NodeType::BooleanNode:
				return evaluateBooleanNode(node);
			case NodeType::ArrayNode:
				return evaluateArrayNode(node);
			case NodeType::WhileLoopNode:
				return evaluateWhileLoopNode(node);
			case NodeType::BinOpNode:
				return evaluateBinOpNode(node);
			case NodeType::ModuleNode:
				return evaluateModuleNode(node);
			case NodeType::ForLoopNode:
				return evaluateForLoopNode(node);
			case NodeType::PipeOpNode:
				return evaluatePipeOpNode(node);
			default:
				throw EvaluateError("Unexpected node");
			}
		}

		auto callFunction(const std::shared_ptr<FunctionValue>& function, const std::vector<std::shared_ptr<Node>>& arguments) {
			if (function->isNative) {
				std::vector<std::shared_ptr<Value>> args;
				for (const auto& arg : arguments)
					args.push_back(evaluate(arg));
				return function->run(*this, args);
			}

			if (arguments.size() != function->prototype->args.size()) {
				const auto expectedArgs = std::to_string(function->prototype->args.size());
				const auto gotArgs = std::to_string(arguments.size());
				throw EvaluateError(
					"function expects " + expectedArgs + " arguments but got " + gotArgs
				);
			}

			currentContext.reserve(function->declarationScope.size());
			currentContext.insert(
				currentContext.end(),
				function->declarationScope.begin(),
				function->declarationScope.end()
			);

			auto functionScope = std::make_shared<Scope>(*this);
			for (int i = 0; i < function->prototype->args.size(); ++i) {
				const auto& argName = nodeCast<NameNode>(function->prototype->args[i])->value;
				const auto& argValue = evaluate(arguments[i]);
				functionScope->variables[argName] = argValue;
			}
			currentContext.push_back(std::move(functionScope));

			auto functionOutput = evaluate(function->prototype->returnValue);
			if (functionOutput->type == ValueType::ExplicitReturnValue)
				functionOutput = valueCast<ExplicitReturnValue>(functionOutput)->value;

			// remove function declaration scope and function scope from context
			currentContext.resize(currentContext.size() - (function->declarationScope.size() + 1));

			return functionOutput;
		}

		Thread& thread;
	private:
		std::vector<std::shared_ptr<Scope>> currentContext;

		auto evaluateBinOpNode(const std::shared_ptr<Node>& node)
			-> std::shared_ptr<Value> {
			const auto n = nodeCast<BinOpNode>(node);

			switch (n->binOpType) {
			case BinOpType::Add:
				return evaluateAddOpNode(n);
			case BinOpType::Subtract:
				return evaluateSubOpNode(n);
			case BinOpType::Multiply:
				return evaluateMulOpNode(n);
			case BinOpType::Divide:
				return evaluateDivOpNode(n);
			case BinOpType::Equals:
				return evaluateEqualsOpNode(n);
			case BinOpType::DoubleEquals:
				return evaluateDoubleEqualsOpNode(n);
			case BinOpType::Dot:
				return evaluateDotOpNode(n);
			case BinOpType::GreaterThan:
				return evaluateGreaterThanOpNode(n);
			case BinOpType::GreaterThanOrEqual:
				return evaluateGreaterThanOrEqualOpNode(n);
			case BinOpType::LessThan:
				return evaluateLessThanOpNode(n);
			case BinOpType::LessThanOrEqual:
				return evaluateLessThanOrEqualOpNode(n);
			case BinOpType::NotEquals:
				return evaluateNotEqualsOpNode(n);
			}

#if DEBUG
			throw EvaluateError("unknown binary operator");
#endif
		}

		auto evaluateLessThanOpNode(const std::shared_ptr<BinOpNode>& node)
			-> std::shared_ptr<Value> {
			const auto left = evaluate(node->left);
			const auto right = evaluate(node->right);
			if (left->type != ValueType::NumberValue || right->type != ValueType::NumberValue) {
				throw EvaluateError("unknown operands");
			}

			const auto leftValue = valueCast<NumberValue>(left)->value;
			const auto rightValue = valueCast<NumberValue>(right)->value;
			return std::make_shared<BooleanValue>(
				leftValue < rightValue
			);
		}

		auto evaluateArrayNode(const std::shared_ptr<Node>& node)
			-> std::shared_ptr<Value> {
			const auto n = nodeCast<ArrayNode>(node);

			std::vector<std::shared_ptr<Value>> elements;
			for (const auto& elementNode : n->elements)
				elements.push_back(evaluate(elementNode));

			return std::make_shared<ArrayValue>(
				std::move(elements)
			);
		}

		auto evaluateGreaterThanOrEqualOpNode(const std::shared_ptr<BinOpNode>& node)
			-> std::shared_ptr<Value> {
			const auto left = evaluate(node->left);
			const auto right = evaluate(node->right);
			if (left->type != ValueType::NumberValue
				|| right->type != left->type) {
				throw EvaluateError("unknown operands");
			}

			const auto leftValue = valueCast<NumberValue>(left)->value;
			const auto rightValue = valueCast<NumberValue>(right)->value;
			return std::make_shared<BooleanValue>(
				leftValue >= rightValue
			);
		}

		auto evaluateLessThanOrEqualOpNode(const std::shared_ptr<BinOpNode>& node)
			-> std::shared_ptr<Value> {
			const auto left = evaluate(node->left);
			const auto right = evaluate(node->right);
			if (left->type != ValueType::NumberValue
				|| right->type != left->type) {
				throw EvaluateError("unknown operands");
			}

			const auto leftValue = valueCast<NumberValue>(left)->value;
			const auto rightValue = valueCast<NumberValue>(right)->value;
			return std::make_shared<BooleanValue>(
				leftValue <= rightValue
			);
		}

		auto evaluateNegationNode(const std::shared_ptr<Node>& node)
			-> std::shared_ptr<Value> {
			const auto n = nodeCast<NegationNode>(node);

			const auto expression = evaluate(n->expression);
			if (expression->type != ValueType::BooleanValue)
				throw EvaluateError("jakas dziwna liczba mi sie tu dzieje");

			const auto v = valueCast<BooleanValue>(expression);
			return std::make_shared<BooleanValue>(!v->value);
		}

		auto evaluateGreaterThanOpNode(const std::shared_ptr<BinOpNode>& node)
			-> std::shared_ptr<Value> {
			const auto left = evaluate(node->left);
			const auto right = evaluate(node->right);
			if (left->type != ValueType::NumberValue
				|| right->type != left->type) {
				throw EvaluateError("unknown operands");
			}

			const auto leftValue = valueCast<NumberValue>(left)->value;
			const auto rightValue = valueCast<NumberValue>(right)->value;
			return std::make_shared<BooleanValue>(
				leftValue > rightValue
			);
		}

		static auto evaluateNumberNode(const std::shared_ptr<Node>& node)
			-> std::shared_ptr<Value> {
			const auto n = nodeCast<NumberNode>(node);
			return std::make_shared<NumberValue>(n->value);
		}

		auto evaluateReturnNode(const std::shared_ptr<Node>& node)
			-> std::shared_ptr<Value> {
			const auto n = nodeCast<ReturnNode>(node);
			return std::make_shared<ExplicitReturnValue>(
				evaluate(n->returnValue)
			);
		}

		auto evaluateNameNode(const std::shared_ptr<Node>& node)
			-> std::shared_ptr<Value> {
			const auto n = nodeCast<NameNode>(node);

			for (auto& scope : currentContext) {
				if (scope->variables.contains(n->value))
					return scope->variables[n->value];
			}

			throw EvaluateError("unknow name: " + n->value);
		}

		static auto evaluateStringNode(const std::shared_ptr<Node>& node)
			-> std::shared_ptr<Value> {
			const auto n = nodeCast<StringNode>(node);
			return std::make_shared<StringValue>(n->value);
		}

		auto evaluateAddOpNode(const std::shared_ptr<BinOpNode>& node)
			-> std::shared_ptr<Value> {
			const auto left = evaluate(node->left);
			if (left->type == ValueType::NumberValue) {
				const auto leftValue = valueCast<NumberValue>(left)->value;
				const auto right = evaluate(node->right);
				if (right->type == ValueType::NumberValue) {
					const auto rightValue = valueCast<NumberValue>(right)->value;
					return std::make_shared<NumberValue>(leftValue + rightValue);
				} else if (right->type == ValueType::StringValue) {
					const auto rightValue = valueCast<StringValue>(right)->value;
					return std::make_shared<StringValue>(
						std::to_string(leftValue) + rightValue
					);
				}
			}
			else if (left->type == ValueType::StringValue) {
				const auto leftValue = valueCast<StringValue>(left)->value;
				const auto right = evaluate(node->right);
				if (right->type == ValueType::NumberValue) {
					const auto rightValue = valueCast<NumberValue>(right)->value;
					return std::make_shared<StringValue>(
						leftValue + std::to_string(rightValue)
					);
				} else if (right->type == ValueType::StringValue) {
					const auto rightValue = valueCast<StringValue>(right)->value;
					return std::make_shared<StringValue>(leftValue + rightValue);
				}
			}

			throw EvaluateError("Unknown operands");
		}

		auto evaluateIfNode(const std::shared_ptr<Node>& node)
			-> std::shared_ptr<Value> {
			const auto n = nodeCast<IfNode>(node);

			const auto condition = valueCast<BooleanValue>(evaluate(n->condition));
			if (condition->value)
				return evaluate(n->body);
			else if (n->elseBody)
				return evaluate(n->elseBody);

			return std::make_shared<UndefinedValue>();
		}

		auto evaluateWhileLoopNode(const std::shared_ptr<Node>& node)
			-> std::shared_ptr<Value> {
			const auto n = nodeCast<WhileLoopNode>(node);

			while (true) {
				const auto condition = evaluate(n->condition);
				if (condition->type != ValueType::BooleanValue)
					throw EvaluateError("while loop condition has to be a boolean");
				const auto value = valueCast<BooleanValue>(condition)->value;
				if (!value)
					break;
				evaluate(n->body);
			}

			return std::make_shared<UndefinedValue>();
		}

		auto evaluateSubOpNode(const std::shared_ptr<BinOpNode>& node)
			-> std::shared_ptr<Value> {
			const auto [left, right] = getNumberValues(node);
			return std::make_shared<NumberValue>(left - right);
		}

		auto evaluateMulOpNode(const std::shared_ptr<BinOpNode>& node)
			-> std::shared_ptr<Value> {
			const auto left = evaluate(node->left);
			if (left->type == ValueType::NumberValue) {
				const auto leftValue = valueCast<NumberValue>(left)->value;
				const auto right = evaluate(node->right);
				if (right->type == ValueType::NumberValue) {
					return std::make_shared<NumberValue>(
						leftValue * valueCast<NumberValue>(right)->value
					);
				}
				else if (right->type == ValueType::StringValue) {
					return std::make_shared<StringValue>(times(
						valueCast<StringValue>(right)->value,
						static_cast<unsigned int>(leftValue)
					));
				}
			}
			else if (left->type == ValueType::StringValue) {
				const auto right = evaluate(node->right);
				if (right->type == ValueType::NumberValue) {
					return std::make_shared<StringValue>(times(
						valueCast<StringValue>(left)->value,
						static_cast<unsigned int>(
							valueCast<NumberValue>(right)->value
						)
					));
				}
			}

			throw EvaluateError("Unknown operands");
		}

		auto evaluateDivOpNode(const std::shared_ptr<BinOpNode>& node)
			-> std::shared_ptr<Value> {
			const auto [left, right] = getNumberValues(node);
			if (std::fabs(right) < DBL_EPSILON) // == 0
				return std::make_shared<StringValue>(":)");
			return std::make_shared<NumberValue>(left / right);
		}

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

		std::shared_ptr<Value> setVariable(const std::string& name, std::shared_ptr<Value> value) {
			for (auto& scope : currentContext) {
				if (scope->variables.contains(name))
					return scope->variables[name] = value;
			}

			currentContext[currentContext.size() - 1]->variables.insert({
				name, value
			});
			return value;
		}

		std::shared_ptr<ArrayValue> destructure(const std::shared_ptr<ArrayNode>& left, const std::shared_ptr<ArrayValue>& right) {
			std::vector<std::shared_ptr<Value>> result;
			for (size_t i = 0; i < left->elements.size(); ++i) {
				const auto name = left->elements[i];
				const auto value = right->at(i);

				switch (name->type) {
					case NodeType::NameNode: {
						setVariable(
							nodeCast<NameNode>(name)->value,
							value
						);
						break;
					}
					case NodeType::ArrayNode: {
						if (value->type != ValueType::ArrayValue)
							throw EvaluateError("value destructured to an array must be an array");
						destructure(
							nodeCast<ArrayNode>(name),
							valueCast<ArrayValue>(value)
						);
						break;
					}
					default: {
						const auto leftValue = evaluate(name);
						if (!leftValue->equals(value))
							throw EvaluateError{ "matching failed" };
						break;
					}
				}

				result.push_back(value);
			}

			return std::make_shared<ArrayValue>(result);
		}

		auto evaluateEqualsOpNode(const std::shared_ptr<BinOpNode>& node)
		  -> std::shared_ptr<Value> {
			switch (node->left->type) {
				case NodeType::NameNode: {
					const auto name = nodeCast<NameNode>(node->left)->value;
					auto value = evaluate(node->right);

					return setVariable(name, value);
				}
				case NodeType::ArrayNode: {
					const auto left = nodeCast<ArrayNode>(node->left);
					const auto _right = evaluate(node->right);
					if (_right->type != ValueType::ArrayValue)
						throw EvaluateError("value destructured to an array must be an array");

					const auto right = valueCast<ArrayValue>(_right);
					if (left->elements.size() != right->size())
						throw EvaluateError("arrays have to be the same length when destructuring");

					return destructure(left, right);
				}
				default:
					throw EvaluateError("Unknown operands");
			}

			throw EvaluateError("Unexpected node");
		}

		bool equals(const std::shared_ptr<Value>& left, const std::shared_ptr<Value>& right) {
			if (left->type != right->type)
				return false;

			switch (left->type) {
			case ValueType::NumberValue: {
				const auto leftNum = valueCast<NumberValue>(left);
				const auto rightNum = valueCast<NumberValue>(right);
				return leftNum->equals(rightNum);
			}
			case ValueType::BooleanValue: {
				const auto leftBool = valueCast<BooleanValue>(left);
				const auto rightBool = valueCast<BooleanValue>(right);
				return leftBool->value == rightBool->value;
			}
			case ValueType::StringValue: {
				const auto leftString = valueCast<StringValue>(left);
				const auto rightString = valueCast<StringValue>(right);
				return leftString->value == rightString->value;
			}
			case ValueType::UndefinedValue:
				return true;
			case ValueType::FunctionValue:
				return left == right;
			case ValueType::ExplicitReturnValue:
				throw EvaluateError("what the fuck is explicit return value");
			default:
				throw EvaluateError("Unknown error");
			}
		}

		auto evaluateDoubleEqualsOpNode(const std::shared_ptr<BinOpNode>& node)
		  -> std::shared_ptr<Value> {
			const auto left = evaluate(node->left);
			const auto right = evaluate(node->right);

			return std::make_shared<BooleanValue>(equals(left, right));
		}

		auto evaluateNotEqualsOpNode(const std::shared_ptr<BinOpNode>& node)
		  -> std::shared_ptr<Value> {
			const auto left = evaluate(node->left);
			const auto right = evaluate(node->right);

			return std::make_shared<BooleanValue>(!equals(left, right));
		}

		auto evaluateFuncCallNode(const std::shared_ptr<Node>& node)
		  -> std::shared_ptr<Value> {
			const auto n = nodeCast<FuncCallNode>(node);
			const auto functionValue = evaluate(n->name);

			if (functionValue->type != ValueType::FunctionValue)
				throw EvaluateError("You can only call a function");

			auto function = valueCast<FunctionValue>(functionValue);
			return callFunction(function, n->arguments);
		}

		auto evaluatePrototypeNode(const std::shared_ptr<Node>& node)
		  -> std::shared_ptr<Value> {
			auto n = nodeCast<PrototypeNode>(node);

			/*return std::make_shared<FunctionValue>(
				[this, n](Evaluator& evaluator, const std::vector<std::shared_ptr<Value>>& args) {
					auto functionScope = std::make_shared<Scope>(evaluator);
					for (unsigned int i = 0; i < n->args.size(); ++i)
						functionScope->variables.insert({
							nodeCast<NameNode>(n->args[i])->value,
							args[i]
						});
					evaluator.currentContext.push_back(functionScope);

					auto value = evaluator.evaluate(n->returnValue);
					if (value->type == ValueType::ExplicitReturnValue)
						value = valueCast<ExplicitReturnValue>(value)->value;

					evaluator.currentContext.pop_back();

					return value;
				},
				currentContext
			); */
			return std::make_shared<FunctionValue>(n, currentContext);
		}

		auto evaluateBlockNode(const std::shared_ptr<Node>& node)
		  -> std::shared_ptr<Value> {
			const auto n = nodeCast<BlockNode>(node);

			currentContext.push_back(std::make_shared<Scope>(*this));
			for (const auto& expr : n->nodes) {
				auto value = evaluate(expr);
				if (value->type == ValueType::ExplicitReturnValue) {
					currentContext.pop_back();
					return value;
				}
			}
			currentContext.pop_back();

			return std::make_shared<UndefinedValue>();
		}

		static auto evaluateBooleanNode(const std::shared_ptr<Node>& node)
		  -> std::shared_ptr<Value> {
			const auto n = nodeCast<BooleanNode>(node);
			return std::make_shared<BooleanValue>(n->value);
		}

		auto evaluateDotOpNode(const std::shared_ptr<BinOpNode>& node)
		  -> std::shared_ptr<Value> {
			if (node->left->type != NodeType::NameNode)
				throw EvaluateError("tf?!?!");

			const auto left = nodeCast<NameNode>(node->left)->value;
			const auto right = nodeCast<NameNode>(node->right)->value;

			bool objectExists = false;
			for (auto& scope : currentContext) {
				if (scope->modules.contains(left)) {
					objectExists = true;

					auto m = scope->modules[left];
					if (m->variables.contains(right))
						return m->variables[right];
				}
			}
			
			if (objectExists)
				throw EvaluateError("object " + left + " has no property: " + right);
			throw EvaluateError("unknow object: " + left);
		}

		std::shared_ptr<Value> evaluateModuleNode(const std::shared_ptr<Node>& node) {
			auto moduleNode = nodeCast<ModuleNode>(node);

			auto* m = new Module{*this};
			m->name = std::move(moduleNode->name);
			for (const auto& [name, value] : moduleNode->members)
				m->variables.emplace(name, evaluate(value));

			currentContext[0]->modules.emplace(m->name, m);

			return std::make_shared<UndefinedValue>();
		}

		std::shared_ptr<Value> evaluatePipeOpNode(const std::shared_ptr<Node>& node) {
			const auto pipeOpNode = nodeCast<PipeOpNode>(node);

			if (pipeOpNode->function->type == NodeType::FuncCallNode) {
				const auto funcCallNode = nodeCast<FuncCallNode>(pipeOpNode->function);
				funcCallNode->arguments.insert(funcCallNode->arguments.begin(), pipeOpNode->value);
				return evaluate(funcCallNode);
			} else {
				auto value = evaluate(pipeOpNode->value);

				const auto function = evaluate(pipeOpNode->function);
				if (function->type != ValueType::FunctionValue)
					throw EvaluateError("pipe argument is not a function");

				const auto f = valueCast<FunctionValue>(function);

				std::vector<std::shared_ptr<Value>> args{std::move(value)};
				// return f->run(*this, args);
				throw EvaluateError("not implemented");
			}
		}

		std::shared_ptr<Value> evaluateForLoopNode(const std::shared_ptr<Node>& node) {
			auto forLoopNode = nodeCast<ForLoopNode>(node);

			if (forLoopNode->iterator->type != NodeType::NameNode)
				throw EvaluateError("no");

			auto iteratorName = nodeCast<NameNode>(forLoopNode->iterator);

			auto iterableArg = evaluate(forLoopNode->iterable);
			
			std::shared_ptr<Value> previousReturnValue;

			if (iterableArg->type == ValueType::ArrayValue) {
				auto iterable = valueCast<ArrayValue>(iterableArg);
				for (const auto& element : iterable->getData()) {
					setVariable(iteratorName->value, element);
					previousReturnValue = evaluate(forLoopNode->body);
				}
			} else if (iterableArg->type == ValueType::RangeValue) {
				auto iterable = valueCast<RangeValue>(iterableArg);
				for (size_t i = iterable->from; i < iterable->to; ++i) {
					setVariable(iteratorName->value, std::make_shared<NumberValue>(i));
					previousReturnValue = evaluate(forLoopNode->body);
				}
			} else {
				throw EvaluateError{"not iterable"};
			}

			return previousReturnValue;
		}
	};
}

#endif //SOVIET_EVALUATOR_HPP
