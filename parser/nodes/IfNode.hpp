#ifndef SOVIET_IFNODE_HPP
#define SOVIET_IFNODE_HPP

#include <memory>
#include "Node.hpp"

namespace soviet {
    struct IfNode : Node {
        std::shared_ptr<Node> condition;
        std::shared_ptr<Node> body;
        std::shared_ptr<Node> elseBody;

        IfNode(
            std::shared_ptr<Node>&& condition,
            std::shared_ptr<Node>&& body,
            std::shared_ptr<Node>&& elseBody
        )
            : Node{NodeType::IfNode}, condition(std::move(condition)),
            body(std::move(body)), elseBody(std::move(elseBody)) {}

        IfNode(
            std::shared_ptr<Node>&& condition,
            std::shared_ptr<Node>&& body
        )
            : Node{NodeType::IfNode}, condition(std::move(condition)),
            body(std::move(body)) {}
    };
}


#endif //SOVIET_IFNODE_HPP
