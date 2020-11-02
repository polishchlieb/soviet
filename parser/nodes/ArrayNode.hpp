#ifndef SOVIET_ARRAYNODE_HPP
#define SOVIET_ARRAYNODE_HPP

#include <vector>
#include <memory>
#include "Node.hpp"

namespace soviet {
    struct ArrayNode : Node {
        std::vector<std::shared_ptr<Node>> elements;
        explicit ArrayNode(std::vector<std::shared_ptr<Node>>&& elements)
            : Node{NodeType::ArrayNode}, elements(std::move(elements)) {}
    };
}

#endif //SOVIET_ARRAYNODE_HPP
