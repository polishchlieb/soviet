#ifndef SOVIET_OPERATORNODE_HPP
#define SOVIET_OPERATORNODE_HPP

#include <memory>
#include <stdexcept>
#include "Node.hpp"

namespace soviet {
    struct OperatorNode : Node {
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;

        OperatorNode() {
            throw std::runtime_error("constructor of OperatorNode was called");
        }
    };
}

#endif //SOVIET_OPERATORNODE_HPP
