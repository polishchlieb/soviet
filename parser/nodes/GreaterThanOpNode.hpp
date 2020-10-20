#ifndef SOVIET_GREATERTHANOPNODE_HPP
#define SOVIET_GREATERTHANOPNODE_HPP

#include "Node.hpp"
#include <memory>

namespace soviet {
    struct GreaterThanOpNode : Node {
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;

        GreaterThanOpNode(
            std::shared_ptr<Node>&& left,
            std::shared_ptr<Node>&& right
        ) : Node{NodeType::GreaterThanOpNode},
            left(std::move(left)), right(std::move(right)) {}
    };
}

#endif //SOVIET_GREATERTHANOPNODE_HPP
