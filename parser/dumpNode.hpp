#ifndef SOVIET_DUMPNODE_HPP
#define SOVIET_DUMPNODE_HPP

#include "nodes/nodes.hpp"
#include <memory>
#include <string>
#include <iostream>
#include "nodes/OperatorNode.hpp"
#include "nodes/LeafNode.hpp"
#include "node_cast.hpp"

namespace soviet {
    static std::string times(const std::string& str, unsigned int num) {
        std::string result;
        for (unsigned int i = 0; i < num; i++)
            result += str;
        return result;
    }

    template<typename T>
    static void dumpLeafNode(const std::shared_ptr<Node>& node, unsigned int spacing = 0) {
        const auto& leafNode = node_cast<LeafNode<T>>(node);
        std::cout << times(" ", 2 * spacing)
            << dumpNodeType(node->type) << ": " << leafNode->value << std::endl;
    }

    static void dump(const std::shared_ptr<Node>& node, unsigned int spacing = 0) {
        switch (node->type) {
            case NodeType::AddOpNode:
            case NodeType::SubOpNode:
            case NodeType::MulOpNode:
            case NodeType::DivOpNode:
            case NodeType::EqualsOpNode:
            case NodeType::DoubleEqualsOpNode: {
                const auto& operatorNode = node_cast<OperatorNode>(node);

                std::cout << times(" ", 2 * spacing)
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
                const auto& funcNode = node_cast<FuncCallNode>(node);
                const auto& funcName = node_cast<NameNode>(funcNode->name);

                std::cout << times(" ", 2 * spacing)
                    << dumpNodeType(node->type) << ":" << std::endl
                    << times(" ", 2 * spacing + 2) << "name: "
                    << funcName->value << std::endl
                    << times(" ", 2 * spacing + 2) << "arguments: "
                    << std::endl;
                for (const auto& arg : funcNode->arguments)
                    dump(arg, 2 * spacing + 2);
                break;
            }
            case NodeType::IfNode: {
                const auto n = node_cast<IfNode>(node);
                std::cout << times(" ", 2 * spacing)
                    << dumpNodeType(node->type) << ":" << std::endl

                    << times(" ", 2 * spacing + 2) << "condition:"
                    << std::endl;

                dump(n->condition, spacing + 2);

                std::cout
                    << times(" ", 2 * spacing + 2) << "then:"
                    << std::endl;

                dump(n->body, spacing + 2);

                if (n->elseBody) {
                    std::cout
                        << times(" ", 2 * spacing + 2) << "else:"
                        << std::endl;

                    dump(n->elseBody, spacing + 2);
                }
            }
        }
    }
}

#endif //SOVIET_DUMPNODE_HPP
