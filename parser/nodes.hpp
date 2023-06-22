#ifndef SOVIET_NODES_HPP
#define SOVIET_NODES_HPP
#include <memory>
#include <utility>
#include <vector>
#include <unordered_map>
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
		PipeOpNode
	};

	struct Node {
		NodeType type;
	};

	static std::string dumpNodeType(const NodeType type) {
		switch (type) {
			case NodeType::NumberNode: return "number_node";
			case NodeType::AddOpNode: return "add_operator_node";
			case NodeType::SubOpNode: return "substract_operator_node";
			case NodeType::MulOpNode: return "multiply_operator_node";
			case NodeType::DivOpNode: return "divide_operator_node";
			case NodeType::EqualsOpNode: return "equals_operator_node";
			case NodeType::DoubleEqualsOpNode: return "double_equals_operator_node";
			case NodeType::NotEqualsOpNode: return "not_equals_operator_node";
			case NodeType::NameNode: return "name_node";
			case NodeType::FuncCallNode: return "function_call_node";
			case NodeType::StringNode: return "string_node";
			case NodeType::IfNode: return "if_node";
			case NodeType::PrototypeNode: return "prototype_node";
			case NodeType::BlockNode: return "block_node";
			case NodeType::DotOpNode: return "dot_operator_node";
			case NodeType::ReturnNode: return "return_node";
			case NodeType::ImportNode: return "import_node";
			case NodeType::GreaterThanOpNode: return "greater_than_operator_node";
			case NodeType::LessThanOpNode: return "less_than_operator_node";
			case NodeType::NegationNode: return "negation_node";
			case NodeType::GreaterThanOrEqualOpNode: return "greater_than_or_equal_operator_node";
			case NodeType::LessThanOrEqualOpNode: return "less_than_or_equal_operator_node";
			case NodeType::BooleanNode: return "boolean_node";
			case NodeType::ArrayNode: return "array_node";
			case NodeType::WhileLoopNode: return "while_loop_node";
			case NodeType::BinOpNode: return "bin_op_node";
			case NodeType::ModuleNode: return "module_node";
			case NodeType::ForLoopNode: return "for_loop_node";
			case NodeType::PipeOpNode: return "pipe_op_node";
			default:
				throw ParseError("Unknown error");
		}
	}

	struct AddOpNode : Node {
		std::shared_ptr<Node> left;
		std::shared_ptr<Node> right;

		AddOpNode(std::shared_ptr<Node> left, std::shared_ptr<Node> right) : Node{ NodeType::AddOpNode },
			left(std::move(left)), right(std::move(right)) {}
	};

	struct ArrayNode : Node {
		std::vector<std::shared_ptr<Node>> elements;
		explicit ArrayNode(std::vector<std::shared_ptr<Node>> elements)
			: Node{ NodeType::ArrayNode }, elements(std::move(elements)) {}
	};

	struct SubOpNode : Node {
		std::shared_ptr<Node> left;
		std::shared_ptr<Node> right;

		SubOpNode(std::shared_ptr<Node> left, std::shared_ptr<Node> right) : Node{ NodeType::SubOpNode },
			left(std::move(left)), right(std::move(right)) {}
	};

	struct NumberNode : Node {
		float value;
		explicit NumberNode(float value) : Node{ NodeType::NumberNode }, value(value) {}
	};

	struct BlockNode : Node {
		std::vector<std::shared_ptr<Node>> nodes;

		explicit BlockNode(
			std::vector<std::shared_ptr<Node>> nodes
		) : Node{NodeType::BlockNode}, nodes(std::move(nodes)) {}
	};

	enum class ImportType {
		file,
		module,
		dll
	};

	struct ImportNode : Node {
		std::string moduleName;
		ImportType importType;

		ImportNode(std::string moduleName, ImportType type)
			: Node{NodeType::ImportNode}, moduleName(std::move(moduleName)), importType(type) {}
	};

	struct ModuleNode : Node {
		std::string name;
		std::unordered_map<std::string, std::shared_ptr<Node>> members;

		explicit ModuleNode(
			std::string name,
			std::unordered_map<std::string, std::shared_ptr<Node>> members
		) : Node{NodeType::ModuleNode},
			name(std::move(name)), members(std::move(members)) {}
	};

	struct BooleanNode : Node {
		bool value;
		explicit BooleanNode(bool value)
			: Node{ NodeType::BooleanNode }, value(value) {}
	};

	struct DivOpNode : Node {
		std::shared_ptr<Node> left;
		std::shared_ptr<Node> right;

		DivOpNode(std::shared_ptr<Node> left, std::shared_ptr<Node> right) : Node{ NodeType::DivOpNode },
			left(std::move(left)), right(std::move(right)) {}
	};

	struct DotOpNode : Node {
		std::shared_ptr<Node> left, right;

		DotOpNode(std::shared_ptr<Node> left, std::shared_ptr<Node> right)
			: Node{ NodeType::DotOpNode }, left(left), right(right) {}
	};

	struct DoubleEqualsOpNode : Node {
		std::shared_ptr<Node> left;
		std::shared_ptr<Node> right;

		DoubleEqualsOpNode(std::shared_ptr<Node> left, std::shared_ptr<Node> right) : Node{ NodeType::DoubleEqualsOpNode },
			left(std::move(left)), right(std::move(right)) {}
	};

	struct NotEqualsOpNode : Node {
		std::shared_ptr<Node> left;
		std::shared_ptr<Node> right;

		NotEqualsOpNode(std::shared_ptr<Node> left, std::shared_ptr<Node> right) : Node{ NodeType::NotEqualsOpNode },
			left(std::move(left)), right(std::move(right)) {}
	};

	struct EqualsOpNode : Node {
		std::shared_ptr<Node> left;
		std::shared_ptr<Node> right;

		EqualsOpNode(std::shared_ptr<Node> left, std::shared_ptr<Node> right) : Node{ NodeType::EqualsOpNode },
			left(std::move(left)), right(std::move(right)) {}
	};

	struct FuncCallNode : Node {
		std::shared_ptr<Node> name;
		std::vector<std::shared_ptr<Node>> arguments;

		FuncCallNode(
			std::shared_ptr<Node> name,
			std::vector<std::shared_ptr<Node>> arguments
		)
			: Node{ NodeType::FuncCallNode },
			name(std::move(name)), arguments(std::move(arguments)) {}
	};

	struct GreaterThanOpNode : Node {
		std::shared_ptr<Node> left, right;

		GreaterThanOpNode(
			std::shared_ptr<Node> left,
			std::shared_ptr<Node> right
		) : Node{ NodeType::GreaterThanOpNode },
			left(std::move(left)), right(std::move(right)) {}
	};

	struct GreaterThanOrEqualOpNode : Node {
		std::shared_ptr<Node> left, right;

		GreaterThanOrEqualOpNode(
			std::shared_ptr<Node> left,
			std::shared_ptr<Node> right
		) : Node{ NodeType::GreaterThanOrEqualOpNode },
			left(std::move(left)), right(std::move(right)) {}
	};

	struct IfNode : Node {
		std::shared_ptr<Node> condition;
		std::shared_ptr<Node> body;
		std::shared_ptr<Node> elseBody;

		IfNode(
			std::shared_ptr<Node> condition,
			std::shared_ptr<Node> body,
			std::shared_ptr<Node> elseBody
		)
			: Node{ NodeType::IfNode }, condition(std::move(condition)),
			body(std::move(body)), elseBody(std::move(elseBody)) {}

		IfNode(
			std::shared_ptr<Node> condition,
			std::shared_ptr<Node> body
		)
			: Node{ NodeType::IfNode }, condition(std::move(condition)),
			body(std::move(body)) {}
	};

	template<typename T>
	struct LeafNode : Node {
		T value;
#if DEBUG
		LeafNode() {
			throw std::runtime_error("constructor of LeafNode was called");
		}
#endif
	};

	struct LessThanOpNode : Node {
		std::shared_ptr<Node> left, right;

		LessThanOpNode(
			std::shared_ptr<Node> left,
			std::shared_ptr<Node> right
		) : Node{ NodeType::LessThanOpNode },
			left(std::move(left)), right(std::move(right)) {}
	};

	struct LessThanOrEqualOpNode : Node {
		std::shared_ptr<Node> left, right;

		LessThanOrEqualOpNode(
			std::shared_ptr<Node> left,
			std::shared_ptr<Node> right
		) : Node{ NodeType::LessThanOrEqualOpNode },
			left(std::move(left)), right(std::move(right)) {}
	};

	struct MulOpNode : Node {
		std::shared_ptr<Node> left;
		std::shared_ptr<Node> right;

		MulOpNode(std::shared_ptr<Node> left, std::shared_ptr<Node> right) : Node{ NodeType::MulOpNode },
			left(std::move(left)), right(std::move(right)) {}
	};

	struct NameNode : Node {
		std::string value;

		explicit NameNode(std::string value) : Node{ NodeType::NameNode }, value(std::move(value)) {}
	};

	struct NegationNode : Node {
		std::shared_ptr<Node> expression;

		explicit NegationNode(std::shared_ptr<Node> expression)
			: Node{ NodeType::NegationNode }, expression(expression) {}
	};

	struct OperatorNode : Node {
		std::shared_ptr<Node> left;
		std::shared_ptr<Node> right;

		OperatorNode() : Node() {
			throw std::runtime_error("constructor of OperatorNode was called");
		}
	};

	struct PrototypeNode : Node {
		std::vector<std::shared_ptr<Node>> args;
		std::shared_ptr<Node> returnValue;

		PrototypeNode(
			std::vector<std::shared_ptr<Node>> args,
			std::shared_ptr<Node> returnValue
		)
			: Node{ NodeType::PrototypeNode },
			args(std::move(args)), returnValue(std::move(returnValue)) {}
	};

	struct ReturnNode : Node {
		std::shared_ptr<Node> returnValue;

		explicit ReturnNode(std::shared_ptr<Node> returnValue)
			: Node{ NodeType::ReturnNode }, returnValue(std::move(returnValue)) {}
	};

	struct StringNode : Node {
		std::string value;
		explicit StringNode(std::string value)
			: Node{ NodeType::StringNode }, value(std::move(value)) {}
	};

	struct WhileLoopNode : Node {
		std::shared_ptr<Node> condition;
		std::shared_ptr<Node> body;

		WhileLoopNode(std::shared_ptr<Node> condition, std::shared_ptr<Node> body)
			: Node{ NodeType::WhileLoopNode }, condition(std::move(condition)),
			body(std::move(body)) {}
	};

	struct ForLoopNode : Node {
		std::shared_ptr<Node> iterator;
		std::shared_ptr<Node> iterable;
		std::shared_ptr<Node> body;

		ForLoopNode(std::shared_ptr<Node> iterator, std::shared_ptr<Node> iterable, std::shared_ptr<Node> body)
			: Node{ NodeType::ForLoopNode }, iterator(std::move(iterator)), iterable(std::move(iterable)), body(std::move(body)) {}
	};

	struct PipeOpNode : Node {
		std::shared_ptr<Node> value;
		std::shared_ptr<Node> function;

		PipeOpNode(std::shared_ptr<Node> value, std::shared_ptr<Node> function)
			: Node{ NodeType::PipeOpNode }, value(std::move(value)), function(std::move(function)) {}
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

	static std::string dumpBinOpType(BinOpType type) {
		switch (type) {
			case BinOpType::Add: return "Add";
			case BinOpType::Subtract: return "Subtract";
			case BinOpType::Multiply: return "Multiply";
			case BinOpType::Divide: return "Divide";
			case BinOpType::GreaterThan: return "GreaterThan";
			case BinOpType::GreaterThanOrEqual: return "GreaterThanOrEqual";
			case BinOpType::LessThan: return "LessThan";
			case BinOpType::LessThanOrEqual: return "LessThanOrEqual";
			case BinOpType::DoubleEquals: return "DoubleEquals";
			case BinOpType::Equals: return "Equals";
			case BinOpType::Dot: return "Dot";
		}
	}

	struct BinOpNode : Node {
		std::shared_ptr<Node> left, right;
		BinOpType binOpType;

		BinOpNode(BinOpType type, std::shared_ptr<Node> left, std::shared_ptr<Node> right)
			: Node{NodeType::BinOpNode}, binOpType(type), left(std::move(left)), right(std::move(right)) {}
	};
}

#endif
