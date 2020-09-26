#ifndef SOVIET_DOTOPNODE_HPP
#define SOVIET_DOTOPNODE_HPP

#include <memory>
#include "Node.hpp"

namespace soviet {
    struct DotOpNode : Node {
        std::shared_ptr<Node> left, right;

        DotOpNode(std::shared_ptr<Node>&& left, std::shared_ptr<Node>&& right)
            : Node{NodeType::DotOpNode}, left(left), right(right) {}
    };
}

#endif //SOVIET_DOTOPNODE_HPP
