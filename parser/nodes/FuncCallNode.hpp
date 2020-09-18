#ifndef SOVIET_FUNCCALLNODE_HPP
#define SOVIET_FUNCCALLNODE_HPP

#include "Node.hpp"

namespace soviet {
    struct FuncCallNode : Node {
        std::shared_ptr<Node> name;
        std::vector<std::shared_ptr<Node>> arguments;

        FuncCallNode(
            std::shared_ptr<Node>&& name,
            std::vector<std::shared_ptr<Node>>&& arguments
        )
            : Node{NodeType::FuncCallNode},
              name(std::move(name)), arguments(std::move(arguments)) {}
    };
}

#endif //SOVIET_FUNCCALLNODE_HPP
