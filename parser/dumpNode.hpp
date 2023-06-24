#ifndef SOVIET_DUMPNODE_HPP
#define SOVIET_DUMPNODE_HPP

#include <memory>
#include <string>
#include <iostream>
#include "nodeCast.hpp"
#include "nodes.hpp"

namespace soviet {
	static std::string times(const std::string& str, unsigned int num) {
		std::string result;
		for (unsigned int i = 0; i < num; i++)
			result += str;
		return result;
	}

	template<typename T>
	static void dumpLeafNode(const std::shared_ptr<Node>& node, unsigned int spacing = 0) {
		const auto& leafNode = nodeCast<LeafNode<T>>(node);
		std::cout << times(" ", spacing)
			<< dumpNodeType(node->type) << ": " << leafNode->value << std::endl;
	}

    static void dump(const std::shared_ptr<Node>& node, unsigned int spacing = 0);
}

#endif //SOVIET_DUMPNODE_HPP
