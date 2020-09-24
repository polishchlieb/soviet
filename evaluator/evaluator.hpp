#ifndef SOVIET_EVALUATOR_HPP
#define SOVIET_EVALUATOR_HPP

#include <memory>
#include "../parser/nodes/nodes.hpp"
#include "../parser/node_cast.hpp"
#include "values/values.hpp"
#include "EvaluateError.hpp"
#include "../parser/dumpNode.hpp"
#include <unordered_map>
#include "dumpValue.hpp"

namespace soviet {
    class Evaluator {
    public:
        Evaluator() {
            variables.insert({
                "print",
                std::make_shared<FunctionValue>(
                    [](const std::vector<std::shared_ptr<Value>>& args) {
                        for (const auto& arg : args)
                            std::cout << dumpValue(arg) << " ";
                        std::cout << std::endl;

                        return std::make_shared<Value>(ValueType::UndefinedValue);
                    }
                )
            });
        }

        std::shared_ptr<Value> evaluate(const std::shared_ptr<Node>& node) {
            switch (node->type) {
                case NodeType::NumberNode:
                    return evaluateNumberNode(node);
                case NodeType::NameNode:
                    return evaluateNameNode(node);
                case NodeType::AddOpNode:
                    return evaluateAddOpNode(node);
                case NodeType::SubOpNode:
                    return evaluateSubOpNode(node);
                case NodeType::MulOpNode:
                    return evaluateMulOpNode(node);
                case NodeType::DivOpNode:
                    return evaluateDivOpNode(node);
                case NodeType::EqualsOpNode:
                    return evaluateEqualsOpNode(node);
                case NodeType::DoubleEqualsOpNode:
                    return evaluateDoubleEqualsOpNode(node);
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
                default:
                    throw EvaluateError("Unexpected node");
            }
        }

    private:
        std::unordered_map<std::string, std::shared_ptr<Value>> variables;

        std::shared_ptr<Value> evaluateNumberNode(const std::shared_ptr<Node>& node) {
            const auto& n = node_cast<NumberNode>(node);
            return std::make_shared<NumberValue>(n->value);
        }

        std::shared_ptr<Value> evaluateNameNode(const std::shared_ptr<Node>& node) {
            const auto& n = node_cast<NameNode>(node);
            if (!variables.contains(n->value))
                throw EvaluateError("unknown name: " + n->value);
            return variables[n->value];
        }

        std::shared_ptr<Value> evaluateStringNode(const std::shared_ptr<Node>& node) {
            const auto& n = node_cast<StringNode>(node);
            return std::make_shared<StringValue>(std::move(n->value));
        }

        std::shared_ptr<Value> evaluateAddOpNode(const std::shared_ptr<Node>& node) {
            const auto& n = node_cast<AddOpNode>(node);
            const auto left = value_cast<NumberValue>(evaluate(n->left));
            const auto right = value_cast<NumberValue>(evaluate(n->right));
            return std::make_shared<NumberValue>(left->value + right->value);
        }

        std::shared_ptr<Value> evaluateIfNode(const std::shared_ptr<Node>& node) {
            const auto n = node_cast<IfNode>(node);
            const auto condition = value_cast<BooleanValue>(evaluate(n->condition));
            if (condition->value)
                return evaluate(n->body);
            else if (n->elseBody)
                return evaluate(n->elseBody);
            return std::make_shared<Value>(ValueType::UndefinedValue);
        }

        std::shared_ptr<Value> evaluateSubOpNode(const std::shared_ptr<Node>& node) {
            const auto& n = node_cast<SubOpNode>(node);
            const auto left = value_cast<NumberValue>(evaluate(n->left));
            const auto right = value_cast<NumberValue>(evaluate(n->right));
            return std::make_shared<NumberValue>(left->value - right->value);
        }

        std::shared_ptr<Value> evaluateMulOpNode(const std::shared_ptr<Node>& node) {
            const auto& n = node_cast<DivOpNode>(node);
            const auto left = value_cast<NumberValue>(evaluate(n->left));
            const auto right = value_cast<NumberValue>(evaluate(n->right));
            return std::make_shared<NumberValue>(left->value * right->value);
        }

        std::shared_ptr<Value> evaluateDivOpNode(const std::shared_ptr<Node>& node) {
            const auto& n = node_cast<DivOpNode>(node);
            const auto left = value_cast<NumberValue>(evaluate(n->left));
            const auto right = value_cast<NumberValue>(evaluate(n->right));
            return std::make_shared<NumberValue>(left->value / right->value);
        }

        std::shared_ptr<Value> evaluateEqualsOpNode(const std::shared_ptr<Node>& node) {
            const auto& n = node_cast<EqualsOpNode>(node);
            const auto name = node_cast<NameNode>(n->left);
            const auto value = value_cast<NumberValue>(evaluate(n->right));
            variables[std::move(name->value)] = value;
            return value;
        }

        std::shared_ptr<Value> evaluateDoubleEqualsOpNode(const std::shared_ptr<Node>& node) {
            const auto& n = node_cast<DoubleEqualsOpNode>(node);
            const auto left = evaluate(n->left);
            const auto right = evaluate(n->right);
            if (left->type != right->type)
                return std::make_shared<BooleanValue>(false);

            switch (left->type) {
                case ValueType::NumberValue: {
                    const auto leftNum = value_cast<NumberValue>(left);
                    const auto rightNum = value_cast<NumberValue>(right);
                    return std::make_shared<BooleanValue>(
                        leftNum->value == rightNum->value
                    );
                }
                case ValueType::BooleanValue: {
                    const auto leftBool = value_cast<BooleanValue>(left);
                    const auto rightBool = value_cast<BooleanValue>(right);
                    return std::make_shared<BooleanValue>(
                        leftBool->value == rightBool->value
                    );
                }
            }
        }

        std::shared_ptr<Value> evaluateFuncCallNode(const std::shared_ptr<Node>& node) {
            const auto& n = node_cast<FuncCallNode>(node);

            const auto function = value_cast<FunctionValue>(evaluate(n->name));

            std::vector<std::shared_ptr<Value>> arguments;
            std::transform(
                n->arguments.begin(),
                n->arguments.end(),
                std::back_inserter(arguments),
                [this](const std::shared_ptr<Node>& argNode) {
                    return evaluate(argNode);
                }
            );

            return function->run(arguments);
        }

        std::shared_ptr<Value> evaluatePrototypeNode(const std::shared_ptr<Node>& node) {
            const auto n = node_cast<PrototypeNode>(node);

            return std::make_shared<FunctionValue>(
                [this, n](const std::vector<std::shared_ptr<Value>>& args) {
                    return evaluate(n->returnValue);
                }
            );
        }

        std::shared_ptr<Value> evaluateBlockNode(const std::shared_ptr<Node>& node) {
            const auto n = node_cast<BlockNode>(node);


        }
    };
}

#endif //SOVIET_EVALUATOR_HPP
