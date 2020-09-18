#ifndef SOVIET_DOUBLEEQUALSOPNODE_HPP
#define SOVIET_DOUBLEEQUALSOPNODE_HPP

#include <memory>
#include <utility>
#include "Node.hpp"
#include "NumberNode.hpp"

namespace soviet {
    struct DoubleEqualsOpNode : Node {
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;

        DoubleEqualsOpNode(std::shared_ptr<Node>&& left, std::shared_ptr<Node>&& right) : Node{NodeType::DoubleEqualsOpNode},
            left(std::move(left)), right(std::move(right)) {}
    };
}

#endif //SOVIET_DOUBLEEQUALSOPNODE_HPP
