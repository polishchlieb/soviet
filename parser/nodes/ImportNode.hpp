#ifndef SOVIET_IMPORTNODE_HPP
#define SOVIET_IMPORTNODE_HPP

#include "Node.hpp"
#include <memory>

namespace soviet {
    struct ImportNode : Node {
        std::shared_ptr<Node> module;
        explicit ImportNode(std::shared_ptr<Node>&& module)
            : Node{NodeType::ImportNode}, module(module) {}
    };
}

#endif //SOVIET_IMPORTNODE_HPP
