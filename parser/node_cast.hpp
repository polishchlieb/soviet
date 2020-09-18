#ifndef SOVIET_NODE_CAST_HPP
#define SOVIET_NODE_CAST_HPP

#include <memory>
#include "nodes/Node.hpp"

namespace soviet {
    template<typename T, typename std::enable_if<std::is_base_of<Node, T>::value>::type* = nullptr>
    inline std::shared_ptr<T> node_cast(const std::shared_ptr<Node>& node) {
        return std::static_pointer_cast<T>(node);
    }
}

#endif //SOVIET_NODE_CAST_HPP
