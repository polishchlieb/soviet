#pragma once
#include <memory>
#include <vector>
#include <unordered_map>
#include <string>
#include "ParseError.hpp"

namespace soviet {
	enum class NodeType {
		NumberNode,
		AddOpNode,
		SubOpNode,
		MulOpNode,
		DivOpNode,
		EqualsOpNode,
		DoubleEqualsOpNode,
		NotEqualsOpNode,
		NameNode,
		FuncCallNode,
		StringNode,
		IfNode,
		PrototypeNode,
		BlockNode,
		DotOpNode,
		ReturnNode,
		ImportNode,
		GreaterThanOpNode,
		NegationNode,
		LessThanOpNode,
		GreaterThanOrEqualOpNode,
		LessThanOrEqualOpNode,
		BooleanNode,
		ArrayNode,
		WhileLoopNode,
		BinOpNode,
		ModuleNode,
		ForLoopNode,
		PipeOpNode,
		NullNode
	};

	struct Node {
		NodeType type;
	};

	std::string dumpNodeType(const NodeType type);

	struct AddOpNode : Node {
		std::shared_ptr<Node> left;
		std::shared_ptr<Node> right;

		AddOpNode(std::shared_ptr<Node> left, std::shared_ptr<Node> right);
	};

	struct ArrayNode : Node {
		std::vector<std::shared_ptr<Node>> elements;
		explicit ArrayNode(std::vector<std::shared_ptr<Node>> elements);
	};

	struct SubOpNode : Node {
		std::shared_ptr<Node> left;
		std::shared_ptr<Node> right;

		SubOpNode(std::shared_ptr<Node> left, std::shared_ptr<Node> right);
	};

	struct NumberNode : Node {
		float value;
		explicit NumberNode(float value);
	};

	struct BlockNode : Node {
		std::vector<std::shared_ptr<Node>> nodes;

		explicit BlockNode(
			std::vector<std::shared_ptr<Node>> nodes
		);
	};

	enum class ImportType {
		file,
		module,
		dll
	};

	struct ImportNode : Node {
		std::string moduleName;
		ImportType importType;

		ImportNode(std::string moduleName, ImportType type);
	};

	struct ModuleNode : Node {
		std::string name;
		std::unordered_map<std::string, std::shared_ptr<Node>> members;

		explicit ModuleNode(
			std::string name,
			std::unordered_map<std::string, std::shared_ptr<Node>> members
		);
	};

	struct BooleanNode : Node {
		bool value;
		explicit BooleanNode(bool value);
	};

	struct NullNode : Node {
		NullNode();
	};

	struct DivOpNode : Node {
		std::shared_ptr<Node> left;
		std::shared_ptr<Node> right;

		DivOpNode(std::shared_ptr<Node> left, std::shared_ptr<Node> right);
	};

	struct DotOpNode : Node {
		std::shared_ptr<Node> left, right;

		DotOpNode(std::shared_ptr<Node> left, std::shared_ptr<Node> right);
	};

	struct DoubleEqualsOpNode : Node {
		std::shared_ptr<Node> left;
		std::shared_ptr<Node> right;

		DoubleEqualsOpNode(std::shared_ptr<Node> left, std::shared_ptr<Node> right);
	};

	struct NotEqualsOpNode : Node {
		std::shared_ptr<Node> left;
		std::shared_ptr<Node> right;

		NotEqualsOpNode(std::shared_ptr<Node> left, std::shared_ptr<Node> right);
	};

	struct EqualsOpNode : Node {
		std::shared_ptr<Node> left;
		std::shared_ptr<Node> right;

		EqualsOpNode(std::shared_ptr<Node> left, std::shared_ptr<Node> right);
	};

	struct FuncCallNode : Node {
		std::shared_ptr<Node> name;
		std::vector<std::shared_ptr<Node>> arguments;

		FuncCallNode(
			std::shared_ptr<Node> name,
			std::vector<std::shared_ptr<Node>> arguments
		);
	};

	struct GreaterThanOpNode : Node {
		std::shared_ptr<Node> left, right;

		GreaterThanOpNode(
			std::shared_ptr<Node> left,
			std::shared_ptr<Node> right
		);
	};

	struct GreaterThanOrEqualOpNode : Node {
		std::shared_ptr<Node> left, right;

		GreaterThanOrEqualOpNode(
			std::shared_ptr<Node> left,
			std::shared_ptr<Node> right
		);
	};

	struct IfNode : Node {
		std::shared_ptr<Node> condition;
		std::shared_ptr<Node> body;
		std::shared_ptr<Node> elseBody;

		IfNode(
			std::shared_ptr<Node> condition,
			std::shared_ptr<Node> body,
			std::shared_ptr<Node> elseBody
		);

		IfNode(
			std::shared_ptr<Node> condition,
			std::shared_ptr<Node> body
		);
	};

	template<typename T>
	struct LeafNode : Node {
		T value;

		LeafNode() {
			throw std::runtime_error("constructor of LeafNode was called");
		}
	};

	struct LessThanOpNode : Node {
		std::shared_ptr<Node> left, right;

		LessThanOpNode(
			std::shared_ptr<Node> left,
			std::shared_ptr<Node> right
		);
	};

	struct LessThanOrEqualOpNode : Node {
		std::shared_ptr<Node> left, right;

		LessThanOrEqualOpNode(
			std::shared_ptr<Node> left,
			std::shared_ptr<Node> right
		);
	};

	struct MulOpNode : Node {
		std::shared_ptr<Node> left;
		std::shared_ptr<Node> right;

		MulOpNode(std::shared_ptr<Node> left, std::shared_ptr<Node> right);
	};

	struct NameNode : Node {
		std::string value;

		explicit NameNode(std::string value);
	};

	struct NegationNode : Node {
		std::shared_ptr<Node> expression;

		explicit NegationNode(std::shared_ptr<Node> expression);
	};

	struct OperatorNode : Node {
		std::shared_ptr<Node> left;
		std::shared_ptr<Node> right;

		OperatorNode();
	};

	struct PrototypeNode : Node {
		std::vector<std::shared_ptr<Node>> args;
		std::shared_ptr<Node> returnValue;

		PrototypeNode(
			std::vector<std::shared_ptr<Node>> args,
			std::shared_ptr<Node> returnValue
		);
	};

	struct ReturnNode : Node {
		std::shared_ptr<Node> returnValue;

		explicit ReturnNode(std::shared_ptr<Node> returnValue);
	};

	struct StringNode : Node {
		std::string value;
		explicit StringNode(std::string value);
	};

	struct WhileLoopNode : Node {
		std::shared_ptr<Node> condition;
		std::shared_ptr<Node> body;

		WhileLoopNode(std::shared_ptr<Node> condition, std::shared_ptr<Node> body);
	};

	struct ForLoopNode : Node {
		std::shared_ptr<Node> iterator;
		std::shared_ptr<Node> iterable;
		std::shared_ptr<Node> body;

		ForLoopNode(std::shared_ptr<Node> iterator, std::shared_ptr<Node> iterable, std::shared_ptr<Node> body);
	};

	struct PipeOpNode : Node {
		std::shared_ptr<Node> value;
		std::shared_ptr<Node> function;

		PipeOpNode(std::shared_ptr<Node> value, std::shared_ptr<Node> function);
	};

	enum class BinOpType {
		Add,
		Subtract,
		Multiply,
		Divide,
		GreaterThan,
		GreaterThanOrEqual,
		LessThan,
		LessThanOrEqual,
		DoubleEquals,
		NotEquals,
		Equals,
		Dot
	};

	std::string dumpBinOpType(BinOpType type);

	struct BinOpNode : Node {
		std::shared_ptr<Node> left, right;
		BinOpType binOpType;

		BinOpNode(BinOpType type, std::shared_ptr<Node> left, std::shared_ptr<Node> right);
	};
}

