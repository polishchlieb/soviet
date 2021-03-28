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
	class Evaluator {
	public:
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
				default:
					throw EvaluateError("Unexpected node");
			}
		}
	private:
		std::vector<Scope> currentContext = {GlobalScope{}};

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
			}

			std::cout << "bin op node" << std::endl;
			return std::make_shared<NumberValue>(1.0f);
		}

		auto evaluateLessThanOpNode(const std::shared_ptr<BinOpNode>& node)
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

			for (auto i = currentContext.rbegin(); i != currentContext.rend(); ++i) {
				if (i->variables.contains(n->value))
					return i->variables[n->value];
			}

			throw EvaluateError("unknow name: " + n->value);
		}

		static auto evaluateStringNode(const std::shared_ptr<Node>& node)
		  -> std::shared_ptr<Value> {
			const auto n = nodeCast<StringNode>(node);
			return std::make_shared<StringValue>(n->value);
		}

		auto evaluateAddOpNode(const std::shared_ptr<BinOpNode>& node)
		  -> std::shared_ptr<Value>{
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
			} else if (left->type == ValueType::StringValue) {
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
				} else if (right->type == ValueType::StringValue) {
					return std::make_shared<StringValue>(times(
						valueCast<StringValue>(right)->value,
						static_cast<unsigned int>(leftValue)
					));
				}
			} else if (left->type == ValueType::StringValue) {
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
			if (std::fabs(right) < DBL_EPSILON)
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

		auto evaluateEqualsOpNode(const std::shared_ptr<BinOpNode>& node)
		  -> std::shared_ptr<Value> {
			switch (node->left->type) {
				case NodeType::NameNode: {
					const auto name = nodeCast<NameNode>(node->left)->value;
					auto value = evaluate(node->right);

					// resolve name
					for (auto i = currentContext.rbegin(); i != currentContext.rend(); ++i) {
						if (i->variables.contains(name))
							return i->variables[name] = value;
					}
					currentContext[currentContext.size() - 1].variables.insert({
						name, value
					});
					return value;
				}
				case NodeType::ArrayNode: {
					if (node->right->type != NodeType::ArrayNode)
						throw EvaluateError("value destructured to an array must be an array");
					
					const auto left = nodeCast<ArrayNode>(node->left);
					const auto right = nodeCast<ArrayNode>(node->right);
					if (left->elements.size() != right->elements.size())
						throw EvaluateError("arrays have to be the same length when destructuring");

					std::vector<std::shared_ptr<Value>> result;
					for (size_t i = 0; i < left->elements.size(); ++i) {
						// assign resolve(left) = evaluate(right)
					}

					return std::make_shared<ArrayValue>(result);
				}
				// add objects and their destructuring
				default:
					throw EvaluateError("Unknown operands");
			}

			throw EvaluateError("Unexpected node");
		}

		auto evaluateDoubleEqualsOpNode(const std::shared_ptr<BinOpNode>& node)
		  -> std::shared_ptr<Value> {
			const auto left = evaluate(node->left);
			const auto right = evaluate(node->right);

			if (left->type != right->type)
				return std::make_shared<BooleanValue>(false);

			switch (left->type) {
				case ValueType::NumberValue: {
					const auto leftNum = valueCast<NumberValue>(left);
					const auto rightNum = valueCast<NumberValue>(right);
					return std::make_shared<BooleanValue>(
						// leftNum->value == rightNum->value
						leftNum->equals(rightNum)
					);
				}
				case ValueType::BooleanValue: {
					const auto leftBool = valueCast<BooleanValue>(left);
					const auto rightBool = valueCast<BooleanValue>(right);
					return std::make_shared<BooleanValue>(
						leftBool->value == rightBool->value
					);
				}
				case ValueType::StringValue: {
					const auto leftString = valueCast<StringValue>(left);
					const auto rightString = valueCast<StringValue>(right);
					return std::make_shared<BooleanValue>(
						leftString->value == rightString->value
					);
				}
				case ValueType::UndefinedValue:
					return std::make_shared<BooleanValue>(true);
				case ValueType::FunctionValue:
				/* case ValueType::ObjectValue:
					return std::make_shared<BooleanValue>(left == right); */
				case ValueType::ExplicitReturnValue:
					throw EvaluateError("what the fuck is explicit return value");
				default:
					throw EvaluateError("Unknown error");
			}
		}

		auto evaluateFuncCallNode(const std::shared_ptr<Node>& node)
		  -> std::shared_ptr<Value> {
			const auto n = nodeCast<FuncCallNode>(node);

			const auto function = evaluate(n->name);

			if (function->type == ValueType::NumberValue) {
				auto num = valueCast<NumberValue>(function)->value;
				if (std::fabs(num - 3.14f) < DBL_EPSILON)
					return std::make_shared<StringValue>("rip eddie");
			}

			if (function->type != ValueType::FunctionValue)
				throw EvaluateError("You can only call a function");

			std::vector<std::shared_ptr<Value>> arguments;
			for (const auto& argument : n->arguments)
				arguments.push_back(evaluate(argument));

			return valueCast<FunctionValue>(function)->run(arguments);
		}

		auto evaluatePrototypeNode(const std::shared_ptr<Node>& node)
		  -> std::shared_ptr<Value> {
			auto n = nodeCast<PrototypeNode>(node);

			return std::make_shared<FunctionValue>(
				[this, n](const std::vector<std::shared_ptr<Value>>& args) {
					Scope functionScope;
					for (unsigned int i = 0; i < n->args.size(); ++i)
						functionScope.variables.insert({
							nodeCast<NameNode>(n->args[i])->value,
							args[i]
						});
					currentContext.push_back(functionScope);

					auto value = evaluate(n->returnValue);
					if (value->type == ValueType::ExplicitReturnValue)
						value = valueCast<ExplicitReturnValue>(value)->value;

					currentContext.pop_back();

					return value;
				}
			);
		}

		auto evaluateBlockNode(const std::shared_ptr<Node>& node)
		  -> std::shared_ptr<Value> {
			const auto n = nodeCast<BlockNode>(node);

			currentContext.push_back(Scope{});
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
			for (auto i = currentContext.rbegin(); i != currentContext.rend(); ++i) {
				if (i->modules.contains(left)) {
					auto m = i->modules[left];

					const auto right = nodeCast<NameNode>(node->right)->value;
					if (m->variables.contains(right))
						return m->variables[right];
				}
			}

			throw EvaluateError("unknow object: " + left);
		}
	};
}

#endif //SOVIET_EVALUATOR_HPP
