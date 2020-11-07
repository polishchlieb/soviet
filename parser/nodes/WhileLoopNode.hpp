#ifndef SOVIET_WHILELOOPNODE_HPP
#define SOVIET_WHILELOOPNODE_HPP

#include <memory>
#include "Node.hpp"

namespace soviet {
    struct WhileLoopNode : Node {
        std::shared_ptr<Node> condition;
        std::shared_ptr<Node> body;

        WhileLoopNode(std::shared_ptr<Node>&& condition, std::shared_ptr<Node>&& body)
            : Node{NodeType::WhileLoopNode}, condition(std::move(condition)),
            body(std::move(body)) {}
    };
}

#endif //SOVIET_WHILELOOPNODE_HPP
