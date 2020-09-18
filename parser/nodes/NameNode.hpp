#ifndef SOVIET_NAMENODE_HPP
#define SOVIET_NAMENODE_HPP

#include <memory>
#include <utility>
#include <string>
#include "Node.hpp"
#include "NumberNode.hpp"

namespace soviet {
    struct NameNode : Node {
        std::string value;

        explicit NameNode(std::string&& value) : Node{NodeType::NameNode}, value(std::move(value)) {}
    };
}

#endif //SOVIET_NAMENODE_HPP
