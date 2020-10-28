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
        StringNode,
        IfNode,
        PrototypeNode,
        BlockNode,
        DotOpNode,
        ReturnNode,
        ImportNode,
        GreaterThanOpNode,
        NegationNode
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
            case NodeType::IfNode: return "if_node";
            case NodeType::PrototypeNode: return "prototype_node";
            case NodeType::BlockNode: return "block_node";
            case NodeType::DotOpNode: return "dot_operator_node";
            case NodeType::ReturnNode: return "return_node";
            case NodeType::ImportNode: return "import_node";
            case NodeType::GreaterThanOpNode: return "greater_than_operator_node";
            case NodeType::NegationNode: return "negation_node";
            default:
                throw ParseError("Unknown error");
        }
    }
}

#endif //SOVIET_NODETYPE_HPP
