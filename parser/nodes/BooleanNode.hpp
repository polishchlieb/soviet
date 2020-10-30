#ifndef SOVIET_BOOLEANNODE_HPP
#define SOVIET_BOOLEANNODE_HPP

#include "Node.hpp"

namespace soviet {
    struct BooleanNode : Node {
        bool value;
        explicit BooleanNode(bool value)
            : Node{NodeType::BooleanNode}, value(value) {}
    };
}

#endif //SOVIET_BOOLEANNODE_HPP
