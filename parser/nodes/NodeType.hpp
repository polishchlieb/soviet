#ifndef SOVIET_NODETYPE_HPP
#define SOVIET_NODETYPE_HPP

#include <string>

namespace soviet {
    enum class NodeType {
        NumberNode,
        AddOpNode,
        SubOpNode,
        MulOpNode,
        DivOpNode,
        EqualsOpNode,
        DoubleEqualsOpNode,
        NameNode,
        FuncCallNode,
        StringNode
    };

    std::string dumpNodeType(const NodeType type) {
        switch (type) {
            case NodeType::NumberNode: return "number_node";
            case NodeType::AddOpNode: return "add_operator_node";
            case NodeType::SubOpNode: return "substract_operator_node";
            case NodeType::MulOpNode: return "multiply_operator_node";
            case NodeType::DivOpNode: return "divide_operator_node";
            case NodeType::EqualsOpNode: return "equals_operator_node";
            case NodeType::DoubleEqualsOpNode: return "double_equals_operator_node";
            case NodeType::NameNode: return "name_node";
            case NodeType::FuncCallNode: return "function_call_node";
            case NodeType::StringNode: return "string_node";
        }
    }
}

#endif //SOVIET_NODETYPE_HPP
