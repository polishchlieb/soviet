#ifndef SOVIET_EVALUATOR_HPP
#define SOVIET_EVALUATOR_HPP

#include <cmath>
#include <memory>
#include "../parser/nodes/nodes.hpp"
#include "../parser/node_cast.hpp"
#include "values/values.hpp"
#include "EvaluateError.hpp"
#include "../parser/dumpNode.hpp"
#include <unordered_map>
#include <cmath>
#include <cfloat>
#include "dumpValue.hpp"
#include "Scope.hpp"
#include "GlobalScope.hpp"

namespace soviet {
    class Evaluator {
    public:
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
                case NodeType::DotOpNode:
                    return evaluateDotOpNode(node);
                case NodeType::ReturnNode:
                    return evaluateReturnNode(node);
                default:
                    throw EvaluateError("Unexpected node");
            }
        }
    private:
        std::vector<Scope> currentContext = {GlobalScope{}};

        std::shared_ptr<Value> evaluateNumberNode(const std::shared_ptr<Node>& node) {
            const auto n = node_cast<NumberNode>(node);
            return std::make_shared<NumberValue>(n->value);
        }

        std::shared_ptr<Value> evaluateReturnNode(const std::shared_ptr<Node>& node) {
            const auto n = node_cast<ReturnNode>(node);
            return std::make_shared<ExplicitReturnValue>(
                evaluate(n->returnValue)
            );
        }

        std::shared_ptr<Value> evaluateNameNode(
            const std::shared_ptr<Node>& node,
            bool require = true
        ) {
            const auto n = node_cast<NameNode>(node);

            for (auto i = currentContext.rbegin(); i != currentContext.rend(); ++i) {
                if (i->variables.contains(n->value))
                    return i->variables[n->value];
            }

            if (require) throw EvaluateError("unknown name: " + n->value);
            return nullptr;
        }

        std::shared_ptr<Value> evaluateStringNode(const std::shared_ptr<Node>& node) {
            const auto n = node_cast<StringNode>(node);
            return std::make_shared<StringValue>(n->value);
        }

        std::shared_ptr<Value> evaluateAddOpNode(const std::shared_ptr<Node>& node) {
            const auto n = node_cast<AddOpNode>(node);

            const auto left = evaluate(n->left);
            if (left->type == ValueType::NumberValue) {
                const auto leftValue = value_cast<NumberValue>(left)->value;
                const auto right = evaluate(n->right);
                if (right->type == ValueType::NumberValue) {
                    const auto rightValue = value_cast<NumberValue>(right)->value;
                    return std::make_shared<NumberValue>(leftValue + rightValue);
                } else if (right->type == ValueType::StringValue) {
                    const auto rightValue = value_cast<StringValue>(right)->value;
                    return std::make_shared<StringValue>(
                        std::to_string(leftValue) + rightValue
                    );
                }
            } else if (left->type == ValueType::StringValue) {
                const auto leftValue = value_cast<StringValue>(left)->value;
                const auto right = evaluate(n->right);
                if (right->type == ValueType::NumberValue) {
                    const auto rightValue = value_cast<NumberValue>(right)->value;
                    return std::make_shared<StringValue>(
                        leftValue + std::to_string(rightValue)
                    );
                } else if (right->type == ValueType::StringValue) {
                    const auto rightValue = value_cast<StringValue>(right)->value;
                    return std::make_shared<StringValue>(leftValue + rightValue);
                }
            }

            throw EvaluateError("Unknown operands");
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
            const auto [left, right] = getNumberValues(node);
            return std::make_shared<NumberValue>(left - right);
        }

        std::shared_ptr<Value> evaluateMulOpNode(const std::shared_ptr<Node>& node) {
            const auto n = node_cast<DivOpNode>(node);

            const auto left = evaluate(n->left);
            if (left->type == ValueType::NumberValue) {
                const auto leftValue = value_cast<NumberValue>(left)->value;
                const auto right = evaluate(n->right);
                if (right->type == ValueType::NumberValue) {
                    return std::make_shared<NumberValue>(
                        leftValue * value_cast<NumberValue>(right)->value
                    );
                } else if (right->type == ValueType::StringValue) {
                    return std::make_shared<StringValue>(times(
                        value_cast<StringValue>(right)->value,
                        static_cast<unsigned int>(leftValue)
                    ));
                }
            } else if (left->type == ValueType::StringValue) {
                const auto right = evaluate(n->right);
                if (right->type == ValueType::NumberValue) {
                    return std::make_shared<StringValue>(times(
                        value_cast<StringValue>(left)->value,
                        static_cast<unsigned int>(
                            value_cast<NumberValue>(right)->value
                        )
                    ));
                }
            }

            throw EvaluateError("Unknown operands");
        }

        std::shared_ptr<Value> evaluateDivOpNode(const std::shared_ptr<Node>& node) {
            const auto [left, right] = getNumberValues(node);
            return std::make_shared<NumberValue>(left / right);
        }

        template<typename T>
        auto getNumberValues(const std::shared_ptr<T>& node) -> std::tuple<float, float> {
            const auto n = node_cast<OperatorNode>(node);

            const auto left = evaluate(n->left);
            const auto right = evaluate(n->right);

            if (left->type != ValueType::NumberValue || right->type != ValueType::NumberValue)
                throw EvaluateError("Unknown operands");

            return {
                value_cast<NumberValue>(left)->value,
                value_cast<NumberValue>(right)->value
            };
        }

        std::shared_ptr<Value> evaluateEqualsOpNode(const std::shared_ptr<Node>& node) {
            const auto n = node_cast<EqualsOpNode>(node);

            switch (n->left->type) {
                case NodeType::NameNode: {
                    const auto& name = node_cast<NameNode>(n->left)->value;
                    auto value = evaluate(n->right);

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
                case NodeType::DotOpNode: {
                    const auto op = node_cast<DotOpNode>(n->left);
                    const auto left = node_cast<NameNode>(op->left)->value;
                    const auto right = node_cast<NameNode>(op->right)->value;

                    auto value = evaluate(n->right);

                    // resolve name
                    for (auto i = currentContext.rbegin(); i != currentContext.rend(); ++i) {
                        if (i->variables.contains(left)) {
                            value_cast<ObjectValue>(i->variables[left])
                                ->set(right, value);
                            return value;
                        }
                    }
                    break;
                }
                default:
                    throw EvaluateError("Unknown operands");
            }

            throw EvaluateError("Unexpected node");
        }

        std::shared_ptr<Value> evaluateDoubleEqualsOpNode(const std::shared_ptr<Node>& node) {
            const auto n = node_cast<DoubleEqualsOpNode>(node);

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
                case ValueType::StringValue: {
                    const auto leftString = value_cast<StringValue>(left);
                    const auto rightString = value_cast<StringValue>(right);
                    return std::make_shared<BooleanValue>(
                        leftString->value == rightString->value
                    );
                }
                case ValueType::UndefinedValue:
                    return std::make_shared<BooleanValue>(true);
                case ValueType::FunctionValue:
                case ValueType::ObjectValue:
                    return std::make_shared<BooleanValue>(left == right);
                case ValueType::ExplicitReturnValue:
                    throw EvaluateError("what the fuck is explicit return value");
                default:
                    throw EvaluateError("Unknown error");
            }
        }

        std::shared_ptr<Value> evaluateFuncCallNode(const std::shared_ptr<Node>& node) {
            const auto n = node_cast<FuncCallNode>(node);

            const auto function = evaluate(n->name);

            if (function->type == ValueType::NumberValue) {
                auto num = value_cast<NumberValue>(function)->value;
                if (std::fabs(num - 3.14f) < DBL_EPSILON) {
                    return std::make_shared<StringValue>("rip eddie");
                }
            }

            if (function->type != ValueType::FunctionValue)
                throw EvaluateError("You can only call a function");

            std::vector<std::shared_ptr<Value>> arguments;
            for (const auto& argument : n->arguments)
                arguments.push_back(evaluate(argument));

            return value_cast<FunctionValue>(function)->run(arguments);
        }

        std::shared_ptr<Value> evaluatePrototypeNode(const std::shared_ptr<Node>& node) {
            auto n = node_cast<PrototypeNode>(node);

            return std::make_shared<FunctionValue>(
                [this, n](const std::vector<std::shared_ptr<Value>>& args) {
                    Scope functionScope;
                    for (unsigned int i = 0; i < n->args.size(); ++i)
                        functionScope.variables.insert({
                            node_cast<NameNode>(n->args[i])->value,
                            args[i]
                        });
                    currentContext.push_back(functionScope);

                    auto value = evaluate(n->returnValue);
                    if (value->type == ValueType::ExplicitReturnValue)
                        value = value_cast<ExplicitReturnValue>(value)->value;

                    currentContext.pop_back();

                    return value;
                }
            );
        }

        std::shared_ptr<Value> evaluateBlockNode(const std::shared_ptr<Node>& node) {
            const auto n = node_cast<BlockNode>(node);

            currentContext.push_back(Scope{});
            for (const auto& expr : n->nodes) {
                auto value = evaluate(expr);
                if (value->type == ValueType::ExplicitReturnValue) {
                    currentContext.pop_back();
                    return value;
                }
            }
            currentContext.pop_back();

            return std::make_shared<Value>(ValueType::UndefinedValue);
        }

        std::shared_ptr<Value> evaluateDotOpNode(const std::shared_ptr<Node>& node) {
            const auto n = node_cast<DotOpNode>(node);

            const auto right = node_cast<NameNode>(n->right)->value;

            auto variable = evaluate(n->left);
            if (variable->type == ValueType::PrototypeObjectValue) {
                auto prototypeObject = value_cast<PrototypeObjectValue>(variable);

                if (prototypeObject->prototype->hasMethod(right))
                    return std::make_shared<FunctionValue>([prototypeObject, right](
                        std::vector<std::shared_ptr<Value>>& args
                    ) {

                        return prototypeObject->call(right, args);
                    });

                if (prototypeObject->object->type == ValueType::ObjectValue) {
                    auto obj = value_cast<ObjectValue>(prototypeObject->object);
                    if (obj->has(right))
                        return obj->get(right);
                }

                return std::make_shared<Value>(ValueType::UndefinedValue);
            } else if (variable->type == ValueType::ObjectValue) {
                return value_cast<ObjectValue>(variable)->get(right);
            }

            const auto left = node_cast<NameNode>(n->left)->value;
            throw EvaluateError("unknow object: " + left);
        }
    };
}

#endif //SOVIET_EVALUATOR_HPP
