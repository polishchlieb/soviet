#ifndef SOVIET_ADDOPNODE_HPP
#define SOVIET_ADDOPNODE_HPP

#include <memory>
#include <utility>
#include "Node.hpp"
#include "NumberNode.hpp"

namespace soviet {
    struct AddOpNode : Node {
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;

        AddOpNode(std::shared_ptr<Node>&& left, std::shared_ptr<Node>&& right) : Node{NodeType::AddOpNode},
            left(std::move(left)), right(std::move(right)) {}
    };
}

#endif //SOVIET_ADDOPNODE_HPP
