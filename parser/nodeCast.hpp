#ifndef SOVIET_NODECAST_HPP
#define SOVIET_NODECAST_HPP

#include <memory>
#include "nodes/Node.hpp"

namespace soviet {
    template<typename T, typename std::enable_if<std::is_base_of<Node, T>::value>::type* = nullptr>
    inline std::shared_ptr<T> nodeCast(const std::shared_ptr<Node>& node) {
        return std::static_pointer_cast<T>(node);
    }
}

#endif //SOVIET_NODECAST_HPP
