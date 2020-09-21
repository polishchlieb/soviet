#ifndef SOVIET_PROTOTYPENODE_HPP
#define SOVIET_PROTOTYPENODE_HPP

#include "Node.hpp"
#include <vector>
#include <memory>

namespace soviet {
    struct PrototypeNode : Node {
        std::vector<std::shared_ptr<Node>> args;
        std::shared_ptr<Node> returnValue;

        PrototypeNode(
            std::vector<std::shared_ptr<Node>>&& args,
            std::shared_ptr<Node>&& returnValue
        )
            : Node{NodeType::PrototypeNode},
              args(std::move(args)), returnValue(std::move(returnValue)) {}
    };
}

#endif //SOVIET_PROTOTYPENODE_HPP
