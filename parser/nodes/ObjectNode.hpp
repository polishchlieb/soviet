#ifndef SOVIET_OBJECTNODE_HPP
#define SOVIET_OBJECTNODE_HPP

#include <memory>
#include <unordered_map>
#include "Node.hpp"

namespace soviet {
    struct ObjectNode : Node {
        std::unordered_map<std::shared_ptr<Node>, std::shared_ptr<Node>> properties;

        explicit ObjectNode(std::unordered_map<std::shared_ptr<Node>, std::shared_ptr<Node>>&& properties)
            : Node{NodeType::ObjectNode}, properties(std::move(properties)) {}
    };
}

#endif //SOVIET_OBJECTNODE_HPP
