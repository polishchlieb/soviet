cmake_minimum_required(VERSION 3.17)
project(soviet)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(lib/googletest)
include_directories(lib/googletest/googletest/include)
include_directories(lib/googletest/googlemock/include)

add_executable(soviet main.cpp tokenizer/Token.hpp tokenizer/TokenType.hpp tokenizer/Tokenizer.hpp tokenizer/PendingToken.hpp tokenizer/PendingTokenType.hpp parser/Parser.hpp parser/nodes/Node.hpp parser/nodes/NodeType.hpp parser/nodes/NumberNode.hpp parser/node_cast.hpp parser/nodes/AddOpNode.hpp parser/nodes/nodes.hpp parser/dumpNode.hpp parser/nodes/SubOpNode.hpp parser/nodes/MulOpNode.hpp parser/nodes/DivOpNode.hpp evaluator/evaluator.hpp evaluator/values/Value.hpp evaluator/value_cast.hpp evaluator/values/ValueType.hpp evaluator/values/NumberValue.hpp evaluator/values/values.hpp evaluator/dumpValue.hpp parser/nodes/EqualsOpNode.hpp parser/nodes/OperatorNode.hpp parser/nodes/NameNode.hpp parser/nodes/LeafNode.hpp tokenizer/dumpTokens.hpp parser/nodes/DoubleEqualsOpNode.hpp evaluator/values/BooleanValue.hpp tokenizer/Tokens.hpp parser/TokenIterator.hpp util/util.hpp parser/nodes/FuncCallNode.hpp parser/nodes/StringNode.hpp evaluator/values/StringValue.hpp evaluator/Function.hpp)
add_executable(soviet_test test/tokenizer_tests.cpp test/test.cpp)

target_link_libraries(soviet_test gtest gtest_main)