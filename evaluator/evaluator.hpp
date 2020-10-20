#ifndef SOVIET_EVALUATOR_HPP
#define SOVIET_EVALUATOR_HPP

#include <cmath>
#include <memory>
#include "../parser/nodes/nodes.hpp"
#include "../parser/nodeCast.hpp"
#include "values/values.hpp"
#include "EvaluateError.hpp"
#include "../parser/dumpNode.hpp"
#include <unordered_map>
#include <cmath>
#include <cfloat>
#include "dumpValue.hpp"
#include "Scope.hpp"
#include "GlobalScope.hpp"
#include "moduleLoader.hpp"

namespace soviet {
    class Evaluator {
    public:
        auto evaluate(const std::shared_ptr<Node>& node) {
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
                case NodeType::ImportNode:
                    return evaluateImportNode(node);
                case NodeType::GreaterThanOpNode:
                    return evaluateGreaterThanOpNode(node);
                default:
                    throw EvaluateError("Unexpected node");
            }
        }
    private:
        std::vector<Scope> currentContext = {GlobalScope{}};

        auto evaluateGreaterThanOpNode(const std::shared_ptr<Node>& node)
          -> std::shared_ptr<Value> {
            const auto n = nodeCast<GreaterThanOpNode>(node);

            const auto left = evaluate(n->left);
            const auto right = evaluate(n->right);
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

        auto evaluateImportNode(const std::shared_ptr<Node>& node)
          -> std::shared_ptr<Value> {
            const auto n = nodeCast<ImportNode>(node);
            if (n->module->type == NodeType::NameNode) {
                const auto name = nodeCast<NameNode>(n->module);
                const auto fileName = "soviet-lib/" + name->value + ".so";
                auto module = loadModule(fileName.c_str());
                currentContext[0].merge(module);
                delete module;
            } else {
                throw EvaluateError("string module import not implemented");
            }

            return std::make_shared<Value>(ValueType::UndefinedValue);
        }

        auto evaluateNameNode(
            const std::shared_ptr<Node>& node,
            bool require = true
        ) -> std::shared_ptr<Value> {
            const auto n = nodeCast<NameNode>(node);

            for (auto i = currentContext.rbegin(); i != currentContext.rend(); ++i) {
                if (i->variables.contains(n->value))
                    return i->variables[n->value];
            }

            if (require) throw EvaluateError("unknown name: " + n->value);
            return nullptr;
        }

        static auto evaluateStringNode(const std::shared_ptr<Node>& node)
          -> std::shared_ptr<Value> {
            const auto n = nodeCast<StringNode>(node);
            return std::make_shared<StringValue>(n->value);
        }

        auto evaluateAddOpNode(const std::shared_ptr<Node>& node)
          -> std::shared_ptr<Value>{
            const auto n = nodeCast<AddOpNode>(node);

            const auto left = evaluate(n->left);
            if (left->type == ValueType::NumberValue) {
                const auto leftValue = valueCast<NumberValue>(left)->value;
                const auto right = evaluate(n->right);
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
                const auto right = evaluate(n->right);
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

            return std::make_shared<Value>(ValueType::UndefinedValue);
        }

        auto evaluateSubOpNode(const std::shared_ptr<Node>& node)
          -> std::shared_ptr<Value> {
            const auto [left, right] = getNumberValues(node);
            return std::make_shared<NumberValue>(left - right);
        }

        auto evaluateMulOpNode(const std::shared_ptr<Node>& node)
          -> std::shared_ptr<Value> {
            const auto n = nodeCast<DivOpNode>(node);

            const auto left = evaluate(n->left);
            if (left->type == ValueType::NumberValue) {
                const auto leftValue = valueCast<NumberValue>(left)->value;
                const auto right = evaluate(n->right);
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
                const auto right = evaluate(n->right);
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

        auto evaluateDivOpNode(const std::shared_ptr<Node>& node)
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

        auto evaluateEqualsOpNode(const std::shared_ptr<Node>& node)
          -> std::shared_ptr<Value> {
            const auto n = nodeCast<EqualsOpNode>(node);

            switch (n->left->type) {
                case NodeType::NameNode: {
                    const auto& name = nodeCast<NameNode>(n->left)->value;
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
                    const auto op = nodeCast<DotOpNode>(n->left);
                    const auto left = nodeCast<NameNode>(op->left)->value;
                    const auto right = nodeCast<NameNode>(op->right)->value;

                    auto value = evaluate(n->right);

                    // resolve name
                    for (auto i = currentContext.rbegin(); i != currentContext.rend(); ++i) {
                        if (i->variables.contains(left)) {
                            valueCast<ObjectValue>(i->variables[left])
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

        auto evaluateDoubleEqualsOpNode(const std::shared_ptr<Node>& node)
          -> std::shared_ptr<Value> {
            const auto n = nodeCast<DoubleEqualsOpNode>(node);

            const auto left = evaluate(n->left);
            const auto right = evaluate(n->right);

            if (left->type != right->type)
                return std::make_shared<BooleanValue>(false);

            switch (left->type) {
                case ValueType::NumberValue: {
                    const auto leftNum = valueCast<NumberValue>(left);
                    const auto rightNum = valueCast<NumberValue>(right);
                    return std::make_shared<BooleanValue>(
                        leftNum->value == rightNum->value
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
                case ValueType::ObjectValue:
                    return std::make_shared<BooleanValue>(left == right);
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
          -> std::shared_ptr<Value>{
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

            return std::make_shared<Value>(ValueType::UndefinedValue);
        }

        auto evaluateDotOpNode(const std::shared_ptr<Node>& node)
          -> std::shared_ptr<Value> {
            const auto n = nodeCast<DotOpNode>(node);

            const auto right = nodeCast<NameNode>(n->right)->value;

            auto variable = evaluate(n->left);
            if (variable->type == ValueType::PrototypeObjectValue) {
                auto prototypeObject = valueCast<PrototypeObjectValue>(variable);

                if (prototypeObject->prototype->hasMethod(right))
                    return std::make_shared<FunctionValue>([prototypeObject, right](
                        std::vector<std::shared_ptr<Value>>& args
                    ) {
                        return prototypeObject->call(right, args);
                    });

                if (prototypeObject->object->type == ValueType::ObjectValue) {
                    auto obj = valueCast<ObjectValue>(prototypeObject->object);
                    if (obj->has(right))
                        return obj->get(right);
                }

                return std::make_shared<Value>(ValueType::UndefinedValue);
            } else if (variable->type == ValueType::ObjectValue) {
                return valueCast<ObjectValue>(variable)->get(right);
            }

            const auto left = nodeCast<NameNode>(n->left)->value;
            throw EvaluateError("unknow object: " + left);
        }
    };
}

#endif //SOVIET_EVALUATOR_HPP
