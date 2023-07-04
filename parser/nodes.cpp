#include "nodes.hpp"

namespace soviet {
	std::string dumpNodeType(const NodeType type) {
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
			case NodeType::ModuleNode: return "module_node";
			case NodeType::ForLoopNode: return "for_loop_node";
			case NodeType::PipeOpNode: return "pipe_op_node";
			case NodeType::AtOpNode: return "at_op_node";
			case NodeType::NullNode: return "null_node";
			case NodeType::AndNode: return "and_node";
			case NodeType::OrNode: return "or_node";
			default:
				throw ParseError("Unknown error");
		}
	}

	AddOpNode::AddOpNode(std::shared_ptr<Node> left, std::shared_ptr<Node> right)
		: Node{NodeType::AddOpNode}, left(std::move(left)), right(std::move(right))
	{}

	ArrayNode::ArrayNode(std::vector<std::shared_ptr<Node>> elements)
		: Node{NodeType::ArrayNode}, elements(std::move(elements))
	{}

	SubOpNode::SubOpNode(std::shared_ptr<Node> left, std::shared_ptr<Node> right)
		: Node{NodeType::SubOpNode}, left(std::move(left)), right(std::move(right))
	{}

	NumberNode::NumberNode(float value)
		: Node{NodeType::NumberNode}, value(value)
	{}

	BlockNode::BlockNode(std::vector<std::shared_ptr<Node>> nodes)
		: Node{NodeType::BlockNode}, nodes(std::move(nodes))
	{}

	ImportNode::ImportNode(std::string moduleName, ImportType type)
		: Node{NodeType::ImportNode}, moduleName(std::move(moduleName)), importType(type)
	{}

	ModuleNode::ModuleNode(std::string name, std::unordered_map<std::string, std::shared_ptr<Node>> members)
		: Node{NodeType::ModuleNode}, name(std::move(name)), members(std::move(members))
	{}

	BooleanNode::BooleanNode(bool value)
		: Node{NodeType::BooleanNode}, value(value)
	{}

	DivOpNode::DivOpNode(std::shared_ptr<Node> left, std::shared_ptr<Node> right)
		: Node{NodeType::DivOpNode}, left(std::move(left)), right(std::move(right))
	{}

	DotOpNode::DotOpNode(std::shared_ptr<Node> left, std::shared_ptr<Node> right)
		: Node{NodeType::DotOpNode}, left(left), right(right)
	{}

	DoubleEqualsOpNode::DoubleEqualsOpNode(std::shared_ptr<Node> left, std::shared_ptr<Node> right)
		: Node{NodeType::DoubleEqualsOpNode}, left(std::move(left)), right(std::move(right))
	{}

	NotEqualsOpNode::NotEqualsOpNode(std::shared_ptr<Node> left, std::shared_ptr<Node> right)
		: Node{NodeType::NotEqualsOpNode}, left(std::move(left)), right(std::move(right))
	{}

	EqualsOpNode::EqualsOpNode(std::shared_ptr<Node> left, std::shared_ptr<Node> right)
		: Node{NodeType::EqualsOpNode}, left(std::move(left)), right(std::move(right))
	{}

	FuncCallNode::FuncCallNode(std::shared_ptr<Node> name, std::vector<std::shared_ptr<Node>> arguments)
		: Node{NodeType::FuncCallNode}, name(std::move(name)), arguments(std::move(arguments))
	{}

	GreaterThanOpNode::GreaterThanOpNode(std::shared_ptr<Node> left, std::shared_ptr<Node> right)
		: Node{NodeType::GreaterThanOpNode}, left(std::move(left)), right(std::move(right))
	{}


	GreaterThanOrEqualOpNode::GreaterThanOrEqualOpNode(std::shared_ptr<Node> left, std::shared_ptr<Node> right)
		: Node{NodeType::GreaterThanOrEqualOpNode}, left(std::move(left)), right(std::move(right))
	{}

	IfNode::IfNode(std::shared_ptr<Node> condition, std::shared_ptr<Node> body)
		: Node{NodeType::IfNode}, condition(std::move(condition)), body(std::move(body))
	{}

	IfNode::IfNode(std::shared_ptr<Node> condition, std::shared_ptr<Node> body, std::shared_ptr<Node> elseBody)
		: Node{NodeType::IfNode}, condition(std::move(condition)),
		  body(std::move(body)), elseBody(std::move(elseBody))
	{}

	LessThanOpNode::LessThanOpNode(std::shared_ptr<Node> left, std::shared_ptr<Node> right)
		: Node{NodeType::LessThanOpNode}, left(std::move(left)), right(std::move(right))
	{}

	LessThanOrEqualOpNode::LessThanOrEqualOpNode(std::shared_ptr<Node> left, std::shared_ptr<Node> right)
		: Node{NodeType::LessThanOrEqualOpNode}, left(std::move(left)), right(std::move(right))
	{}

	MulOpNode::MulOpNode(std::shared_ptr<Node> left, std::shared_ptr<Node> right)
		: Node{NodeType::MulOpNode}, left(std::move(left)), right(std::move(right))
	{}

	NameNode::NameNode(std::string value)
		: Node{NodeType::NameNode}, value(std::move(value))
	{}

	NegationNode::NegationNode(std::shared_ptr<Node> expression)
		: Node{NodeType::NegationNode}, expression(std::move(expression))
	{}

	OperatorNode::OperatorNode() : Node() {
		throw std::runtime_error("constructor of OperatorNode was called");
	}

	PrototypeNode::PrototypeNode(std::vector<std::shared_ptr<Node>> args, std::shared_ptr<Node> returnValue)
		: Node{NodeType::PrototypeNode}, args(std::move(args)), returnValue(std::move(returnValue))
	{}

	ReturnNode::ReturnNode(std::shared_ptr<Node> returnValue)
		: Node{NodeType::ReturnNode}, returnValue(std::move(returnValue))
	{}

	StringNode::StringNode(std::string value)
		: Node{NodeType::StringNode}, value(std::move(value))
	{}

	WhileLoopNode::WhileLoopNode(std::shared_ptr<Node> condition, std::shared_ptr<Node> body)
		: Node{NodeType::WhileLoopNode}, condition(std::move(condition)), body(std::move(body))
	{}

	ForLoopNode::ForLoopNode(std::shared_ptr<Node> iterator, std::shared_ptr<Node> iterable, std::shared_ptr<Node> body)
		: Node{NodeType::ForLoopNode}, iterator(std::move(iterator)),
		  iterable(std::move(iterable)), body(std::move(body))
	{}

	PipeOpNode::PipeOpNode(std::shared_ptr<Node> value, std::shared_ptr<Node> function)
		: Node{NodeType::PipeOpNode}, value(std::move(value)), function(std::move(function))
	{}

	NullNode::NullNode() : Node{NodeType::NullNode} {}

	AtOpNode::AtOpNode(std::shared_ptr<Node> array, std::shared_ptr<Node> index)
		: Node{NodeType::AtOpNode}, array(std::move(array)), index(std::move(index))
	{}

	OrNode::OrNode(std::shared_ptr<Node> left, std::shared_ptr<Node> right)
		: Node{NodeType::OrNode}, left(std::move(left)), right(std::move(right))
	{}

	AndNode::AndNode(std::shared_ptr<Node> left, std::shared_ptr<Node> right)
		: Node{NodeType::AndNode}, left(std::move(left)), right(std::move(right))
	{}
}