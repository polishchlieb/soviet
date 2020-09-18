#ifndef SOVIET_LEAFNODE_HPP
#define SOVIET_LEAFNODE_HPP

#include "Node.hpp"
#include <stdexcept>

namespace soviet {
    template<typename T>
    struct LeafNode : Node {
        T value;

        LeafNode() {
            throw std::runtime_error("constructor of LeafNode was called");
        }
    };
}

#endif //SOVIET_LEAFNODE_HPP
