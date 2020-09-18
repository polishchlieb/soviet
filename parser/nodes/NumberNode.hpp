#ifndef SOVIET_NUMBERNODE_HPP
#define SOVIET_NUMBERNODE_HPP

#include "Node.hpp"

namespace soviet {
    struct NumberNode : Node {
        float value;
        explicit NumberNode(float value) : Node{NodeType::NumberNode}, value(value) {}
    };
}

#endif //SOVIET_NUMBERNODE_HPP
