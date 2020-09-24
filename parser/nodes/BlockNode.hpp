#ifndef SOVIET_BLOCKNODE_HPP
#define SOVIET_BLOCKNODE_HPP

#include "Node.hpp"
#include <vector>
#include <memory>

namespace soviet {
    struct BlockNode : Node {
        std::vector<std::shared_ptr<Node>> nodes;

        explicit BlockNode(
            std::vector<std::shared_ptr<Node>>&& nodes
        ) : Node{NodeType::BlockNode}, nodes(nodes) {}
    };
}

#endif //SOVIET_BLOCKNODE_HPP
