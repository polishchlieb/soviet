#ifndef SOVIET_LESSTHANOPNODE_HPP
#define SOVIET_LESSTHANOPNODE_HPP

#include <memory>
#include "Node.hpp"

namespace soviet {
    struct LessThanOpNode : Node {
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;

        LessThanOpNode(
            std::shared_ptr<Node>&& left,
            std::shared_ptr<Node>&& right
        ) : Node{NodeType::LessThanOpNode},
            left(std::move(left)), right(std::move(right)) {}
    };
}

#endif //SOVIET_LESSTHANOPNODE_HPP
