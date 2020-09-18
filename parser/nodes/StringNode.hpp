#ifndef SOVIET_STRINGNODE_HPP
#define SOVIET_STRINGNODE_HPP

#include "Node.hpp"

namespace soviet {
    struct StringNode : Node {
        std::string value;
        explicit StringNode(std::string&& value)
            : Node{NodeType::StringNode}, value(std::move(value)) {}
    };
}

#endif //SOVIET_STRINGNODE_HPP
