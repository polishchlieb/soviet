#ifndef SOVIET_DUMPNODE_HPP
#define SOVIET_DUMPNODE_HPP

#include "nodes/nodes.hpp"
#include <memory>
#include <string>
#include <iostream>
#include "nodes/OperatorNode.hpp"
#include "nodes/LeafNode.hpp"
#include "nodeCast.hpp"

namespace soviet {
    static std::string times(const std::string& str, unsigned int num) {
        std::string result;
        for (unsigned int i = 0; i < num; i++)
            result += str;
        return result;
    }

    template<typename T>
    static void dumpLeafNode(const std::shared_ptr<Node>& node, unsigned int spacing = 0) {
        const auto& leafNode = nodeCast<LeafNode<T>>(node);
        std::cout << times(" ", spacing)
            << dumpNodeType(node->type) << ": " << leafNode->value << std::endl;
    }

    static void dump(const std::shared_ptr<Node>& node, unsigned int spacing = 0) {
        switch (node->type) {
            case NodeType::AddOpNode:
            case NodeType::SubOpNode:
            case NodeType::MulOpNode:
            case NodeType::DivOpNode:
            case NodeType::EqualsOpNode:
            case NodeType::DoubleEqualsOpNode:
            case NodeType::DotOpNode:
            case NodeType::GreaterThanOpNode:
            case NodeType::LessThanOpNode: {
                const auto& operatorNode = nodeCast<OperatorNode>(node);

                std::cout << times(" ", spacing)
                          << dumpNodeType(node->type) << ":" << std::endl;
                dump(operatorNode->left, spacing + 2);
                dump(operatorNode->right, spacing + 2);
                break;
            }
            case NodeType::NumberNode:
                dumpLeafNode<float>(node, spacing);
                break;
            case NodeType::NameNode:
            case NodeType::StringNode:
                dumpLeafNode<std::string>(node, spacing);
                break;
            case NodeType::FuncCallNode: {
                const auto& funcNode = nodeCast<FuncCallNode>(node);

                std::cout << times(" ", spacing)
                    << dumpNodeType(node->type) << ":" << std::endl
                    << times(" ", spacing + 2) << "function: " << std::endl;

                dump(funcNode->name, spacing + 4);

                std::cout << times(" ", spacing + 2) << "arguments: "
                    << std::endl;
                for (const auto& arg : funcNode->arguments)
                    dump(arg, spacing + 4);
                break;
            }
            case NodeType::IfNode: {
                const auto n = nodeCast<IfNode>(node);
                std::cout << times(" ", spacing)
                    << dumpNodeType(node->type) << ":" << std::endl

                    << times(" ", spacing + 2) << "condition:"
                    << std::endl;

                dump(n->condition, spacing + 4);

                std::cout
                    << times(" ", spacing + 2) << "then:"
                    << std::endl;

                dump(n->body, spacing + 4);

                if (n->elseBody) {
                    std::cout
                        << times(" ", spacing + 2) << "else:"
                        << std::endl;

                    dump(n->elseBody, spacing + 4);
                }
                break;
            }
            case NodeType::PrototypeNode: {
                const auto n = nodeCast<PrototypeNode>(node);
                std::cout << times(" ", spacing)
                    << dumpNodeType(node->type) << ":" << std::endl

                    << times(" ", spacing + 2) << "arguments:"
                    << std::endl;

                for (const auto& arg : n->args)
                    dump(arg, spacing + 4);

                std::cout
                    << times(" ", spacing + 2) << "return value:"
                    << std::endl;

                dump(n->returnValue, spacing + 4);
                break;
            }
            case NodeType::BlockNode: {
                const auto n = nodeCast<BlockNode>(node);
                std::cout << times(" ", spacing)
                    << dumpNodeType(n->type) << ":" << std::endl;

                for (const auto& expr : n->nodes)
                    dump(expr, spacing + 2);
                break;
            }
            case NodeType::ReturnNode: {
                const auto n = nodeCast<ReturnNode>(node);
                std::cout << times(" ", spacing)
                    << dumpNodeType(n->type) << ":" << std::endl;

                dump(n->returnValue, spacing + 2);
                break;
            }
            case NodeType::ImportNode: {
                const auto n = nodeCast<ImportNode>(node);
                std::cout << times(" ", spacing)
                    << dumpNodeType(n->type) << ":" << std::endl;

                dump(n->module, spacing + 2);
                break;
            }
            case NodeType::NegationNode: {
                const auto n = nodeCast<NegationNode>(node);
                std::cout << times(" ", spacing)
                    << dumpNodeType(n->type) << ":" << std::endl;

                dump(n->expression, spacing + 2);
                break;
            }
        }
    }
}

#endif //SOVIET_DUMPNODE_HPP
