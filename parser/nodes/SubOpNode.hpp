#ifndef SOVIET_SUBOPNODE_HPP
#define SOVIET_SUBOPNODE_HPP

#include <memory>
#include <utility>
#include "Node.hpp"
#include "NumberNode.hpp"

namespace soviet {
    struct SubOpNode : Node {
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;

        SubOpNode(std::shared_ptr<Node>&& left, std::shared_ptr<Node>&& right) : Node{NodeType::SubOpNode},
            left(std::move(left)), right(std::move(right)) {}
    };
}

#endif //SOVIET_SUBOPNODE_HPP
