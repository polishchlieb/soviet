#ifndef SOVIET_GREATERTHANOREQUALOPNODE_HPP
#define SOVIET_GREATERTHANOREQUALOPNODE_HPP

#include <memory>
#include "Node.hpp"

namespace soviet {
    struct GreaterThanOrEqualOpNode : Node {
        std::shared_ptr<Node> left, right;

        GreaterThanOrEqualOpNode(
            std::shared_ptr<Node>&& left,
            std::shared_ptr<Node>&& right
        ) : Node{NodeType::GreaterThanOrEqualOpNode},
            left(std::move(left)), right(std::move(right)) {}
    };
}

#endif //SOVIET_GREATERTHANOREQUALOPNODE_HPP
