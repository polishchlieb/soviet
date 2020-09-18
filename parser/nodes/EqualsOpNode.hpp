#ifndef SOVIET_EQUALSOPNODE_HPP
#define SOVIET_EQUALSOPNODE_HPP

#include <memory>
#include <utility>
#include "Node.hpp"
#include "NumberNode.hpp"

namespace soviet {
    struct EqualsOpNode : Node {
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;

        EqualsOpNode(std::shared_ptr<Node>&& left, std::shared_ptr<Node>&& right) : Node{NodeType::EqualsOpNode},
                                                                                 left(std::move(left)), right(std::move(right)) {}
    };
}

#endif //SOVIET_EQUALSOPNODE_HPP
