#ifndef SOVIET_RETURNNODE_HPP
#define SOVIET_RETURNNODE_HPP

#include "Node.hpp"
#include <memory>

namespace soviet {
    struct ReturnNode : Node {
        std::shared_ptr<Node> returnValue;

        explicit ReturnNode(std::shared_ptr<Node>&& returnValue)
            : Node{NodeType::ReturnNode}, returnValue(std::move(returnValue)) {}
    };
}

#endif //SOVIET_RETURNNODE_HPP
