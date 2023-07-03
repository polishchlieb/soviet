#pragma once
#include <memory>
#include <string>
#include <iostream>
#include "nodeCast.hpp"
#include "nodes.hpp"
#include "../util/util.hpp"

namespace soviet {
    void dump(const std::shared_ptr<Node>& node, unsigned int spacing = 0);
}
