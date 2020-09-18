#ifndef SOVIET_DIVOPNODE_HPP
#define SOVIET_DIVOPNODE_HPP

#include <memory>
#include <utility>
#include "Node.hpp"
#include "NumberNode.hpp"

namespace soviet {
    struct DivOpNode : Node {
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;

        DivOpNode(std::shared_ptr<Node>&& left, std::shared_ptr<Node>&& right) : Node{NodeType::DivOpNode},
            left(std::move(left)), right(std::move(right)) {}
    };
}

#endif //SOVIET_DIVOPNODE_HPP
