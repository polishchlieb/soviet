#pragma once
#include <memory>
#include <string>
#include <iostream>
#include "nodeCast.hpp"
#include "nodes.hpp"
#include "../util/util.hpp"

namespace soviet {
	template<typename T>
	static void dumpLeafNode(const std::shared_ptr<Node>& node, unsigned int spacing = 0) {
		const auto& leafNode = nodeCast<LeafNode<T>>(node);
		std::cout << times(" ", spacing)
			<< dumpNodeType(node->type) << ": " << leafNode->value << std::endl;
	}

    static void dump(const std::shared_ptr<Node>& node, unsigned int spacing = 0);
}
