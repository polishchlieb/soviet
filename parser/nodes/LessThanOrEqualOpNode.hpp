#ifndef SOVIET_LESSTHANOREQUALOPNODE_HPP
#define SOVIET_LESSTHANOREQUALOPNODE_HPP

#include <memory>
#include "Node.hpp"

namespace soviet {
    struct LessThanOrEqualOpNode : Node {
        std::shared_ptr<Node> left, right;

        LessThanOrEqualOpNode(
            std::shared_ptr<Node>&& left,
            std::shared_ptr<Node>&& right
        ) : Node{NodeType::LessThanOrEqualOpNode},
            left(std::move(left)), right(std::move(right)) {}
    };
}

#endif //SOVIET_LESSTHANOREQUALOPNODE_HPP
