#ifndef SOVIET_NEGATIONNODE_HPP
#define SOVIET_NEGATIONNODE_HPP

#include <memory>
#include "Node.hpp"

namespace soviet {
    struct NegationNode : Node {
        std::shared_ptr<Node> expression;

        explicit NegationNode(std::shared_ptr<Node>&& expression)
            : Node{NodeType::NegationNode}, expression(expression) {}
    };
}

#endif //SOVIET_NEGATIONNODE_HPP
