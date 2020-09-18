#ifndef SOVIET_MULOPNODE_HPP
#define SOVIET_MULOPNODE_HPP

#include <memory>
#include <utility>
#include "Node.hpp"
#include "NumberNode.hpp"

namespace soviet {
    struct MulOpNode : Node {
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;

        MulOpNode(std::shared_ptr<Node>&& left, std::shared_ptr<Node>&& right) : Node{NodeType::MulOpNode},
            left(std::move(left)), right(std::move(right)) {}
    };
}

#endif //SOVIET_MULOPNODE_HPP
