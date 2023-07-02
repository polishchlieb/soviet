#include "evaluator.hpp"
#include <fstream>
#include "../file.hpp"
#if defined(_WIN32)
#include <Windows.h>
#endif
#include <filesystem>
#include <fstream>
#include "../file.hpp"
#include "GlobalScope.hpp"
#include "../parser/dumpNode.hpp"
#include "../parser/nodeCast.hpp"
#include <cmath>
#include "../util/util.hpp"

namespace soviet {
	typedef void(__stdcall* LoadFunction)(int& value);

	Evaluator::Evaluator(Thread& thread) : thread(thread) {
		currentContext.push_back(std::make_shared<GlobalScope>(*this));
	}

	std::shared_ptr<Value> Evaluator::evaluate(const std::shared_ptr<Node>& node) {
		switch (node->type) {
		case NodeType::NumberNode:
			return evaluateNumberNode(node);
		case NodeType::NameNode:
			return evaluateNameNode(node);
		case NodeType::StringNode:
			return evaluateStringNode(node);
		case NodeType::FuncCallNode:
			return evaluateFuncCallNode(node);
		case NodeType::IfNode:
			return evaluateIfNode(node);
		case NodeType::PrototypeNode:
			return evaluatePrototypeNode(node);
		case NodeType::BlockNode:
			return evaluateBlockNode(node);
		case NodeType::ReturnNode:
			return evaluateReturnNode(node);
		case NodeType::NegationNode:
			return evaluateNegationNode(node);
		case NodeType::BooleanNode:
			return evaluateBooleanNode(node);
		case NodeType::ArrayNode:
			return evaluateArrayNode(node);
		case NodeType::WhileLoopNode:
			return evaluateWhileLoopNode(node);
		case NodeType::BinOpNode:
			return evaluateBinOpNode(node);
		case NodeType::ModuleNode:
			return evaluateModuleNode(node);
		case NodeType::ForLoopNode:
			return evaluateForLoopNode(node);
		case NodeType::PipeOpNode:
			return evaluatePipeOpNode(node);
		case NodeType::ImportNode:
			return evaluateImportNode(node);
		case NodeType::NullNode:
			return evaluateNullNode(node);
		default:
			throw EvaluateError("Unexpected node");
		}
	}

	std::shared_ptr<Value> Evaluator::callFunction(const std::shared_ptr<FunctionValue>& function, std::vector<std::shared_ptr<Value>>& arguments) {
		if (function->isNative)
			return function->run(*this, arguments);

		if (arguments.size() != function->prototype->args.size()) {
			const auto expectedArgs = std::to_string(function->prototype->args.size());
			const auto gotArgs = std::to_string(arguments.size());
			throw EvaluateError(
				"function expects " + expectedArgs + " arguments but got " + gotArgs
			);
		}

		currentContext.reserve(currentContext.size() + function->declarationScope.size() + 1);
		currentContext.insert(
			currentContext.end(),
			function->declarationScope.begin(),
			function->declarationScope.end()
		);

		std::shared_ptr<Scope>& functionScope = currentContext.emplace_back(*this);
		for (int i = 0; i < function->prototype->args.size(); ++i) {
			const auto& argName = nodeCast<NameNode>(function->prototype->args[i])->value;
			const auto& argValue = arguments[i];
			functionScope->variables[argName] = argValue;
		}

		auto functionOutput = evaluate(function->prototype->returnValue);
		if (functionOutput->type == ValueType::ExplicitReturnValue)
			functionOutput = valueCast<ExplicitReturnValue>(functionOutput)->value;

		// remove function declaration scope and function scope from context
		currentContext.resize(currentContext.size() - (function->declarationScope.size() + 1));

		return functionOutput;
	}

	std::shared_ptr<soviet::Value> Evaluator::evaluateBinOpNode(const std::shared_ptr<Node>& node) {
		const auto& n = nodeCast<BinOpNode>(node);

		switch (n->binOpType) {
			case BinOpType::Add:
				return evaluateAddOpNode(n);
			case BinOpType::Subtract:
				return evaluateSubOpNode(n);
			case BinOpType::Multiply:
				return evaluateMulOpNode(n);
			case BinOpType::Divide:
				return evaluateDivOpNode(n);
			case BinOpType::Equals:
				return evaluateEqualsOpNode(n);
			case BinOpType::DoubleEquals:
				return evaluateDoubleEqualsOpNode(n);
			case BinOpType::Dot:
				return evaluateDotOpNode(n);
			case BinOpType::GreaterThan:
				return evaluateGreaterThanOpNode(n);
			case BinOpType::GreaterThanOrEqual:
				return evaluateGreaterThanOrEqualOpNode(n);
			case BinOpType::LessThan:
				return evaluateLessThanOpNode(n);
			case BinOpType::LessThanOrEqual:
				return evaluateLessThanOrEqualOpNode(n);
			case BinOpType::NotEquals:
				return evaluateNotEqualsOpNode(n);
		}

		throw EvaluateError("unknown binary operator");
	}

	std::shared_ptr<Value> Evaluator::evaluateImportNode(const std::shared_ptr<Node>& node) {
		const auto& n = nodeCast<ImportNode>(node);

		if (n->importType == ImportType::dll) {
#if defined(WIN32)
			std::filesystem::path filepath = std::filesystem::current_path() / n->moduleName;

			HINSTANCE handle = LoadLibrary((LPCWSTR) filepath.c_str());
			if (!handle)
				throw EvaluateError("couldn't load c library");
			
			LoadFunction load = (LoadFunction)GetProcAddress(handle, "load");

			int value = 0;
			load(value);

			auto module = new Module{*this};
			module->variables["get_value"] = std::make_shared<FunctionValue>(
				[value](Evaluator&, std::vector<std::shared_ptr<Value>>&) {
					return std::make_shared<NumberValue>((float) value);
				}
			);
			currentContext[0]->modules["Test"] = module;
#else
			throw EvaluateError{"not implemented"};
#endif
		} else {
			std::ifstream file{n->moduleName};
			if (!file.is_open())
				throw EvaluateError{"file opening error"};

			Evaluator fileEvaluator{thread};

			while (!file.eof()) {
				getfileline(fileEvaluator, file);
			}

			const std::shared_ptr<Scope>& fileScope = fileEvaluator.currentContext[0];
			for (const auto& [name, value] : fileScope->variables)
				currentContext[0]->variables[name] = value;
			for (const auto& [name, module] : fileScope->modules) {
				if (currentContext[0]->modules.contains(name))
					delete currentContext[0]->modules[name];
				currentContext[0]->modules[name] = module;
			}
		}

		return std::make_shared<UndefinedValue>();
	}

	std::shared_ptr<soviet::Value> Evaluator::evaluateLessThanOpNode(const std::shared_ptr<BinOpNode>& node) {
		const auto left = evaluate(node->left);
		const auto right = evaluate(node->right);
		if (left->type != ValueType::NumberValue || right->type != ValueType::NumberValue) {
			throw EvaluateError("unknown operands");
		}

		const auto leftNum = valueCast<NumberValue>(left);
		const auto rightNum = valueCast<NumberValue>(right);
		return std::make_shared<BooleanValue>(
			leftNum->compare(rightNum) == NumberComparisonResult::LESS
		);
	}

	std::shared_ptr<soviet::Value> Evaluator::evaluateArrayNode(const std::shared_ptr<Node>& node) {
		const auto n = nodeCast<ArrayNode>(node);

		std::vector<std::shared_ptr<Value>> elements;
		for (const auto& elementNode : n->elements)
			elements.push_back(evaluate(elementNode));

		return std::make_shared<ArrayValue>(
			std::move(elements)
		);
	}

	std::shared_ptr<Value> Evaluator::evaluateGreaterThanOrEqualOpNode(const std::shared_ptr<BinOpNode>& node) {
		const auto left = evaluate(node->left);
		const auto right = evaluate(node->right);
		if (left->type != ValueType::NumberValue) {
			throw EvaluateError("unknown operands");
		}

		const auto leftNum = valueCast<NumberValue>(left);
		const auto rightNum = valueCast<NumberValue>(right);
		const auto comparison = leftNum->compare(rightNum);
		return std::make_shared<BooleanValue>(
			comparison == NumberComparisonResult::GREATER || comparison == NumberComparisonResult::EQUAL
		);
	}

	std::shared_ptr<soviet::Value> Evaluator::evaluateLessThanOrEqualOpNode(const std::shared_ptr<BinOpNode>& node) {
		const auto left = evaluate(node->left);
		const auto right = evaluate(node->right);
		if (left->type != ValueType::NumberValue
			|| right->type != left->type) {
			throw EvaluateError("unknown operands");
		}

		const auto leftNum = valueCast<NumberValue>(left);
		const auto rightNum = valueCast<NumberValue>(right);
		const auto comparison = leftNum->compare(rightNum);
		return std::make_shared<BooleanValue>(
			comparison == NumberComparisonResult::LESS || comparison == NumberComparisonResult::EQUAL
		);
	}

	std::shared_ptr<soviet::Value> Evaluator::evaluateNegationNode(const std::shared_ptr<Node>& node) {
		const auto n = nodeCast<NegationNode>(node);

		const auto expression = evaluate(n->expression);
		if (expression->type != ValueType::BooleanValue)
			throw EvaluateError("jakas dziwna liczba mi sie tu dzieje");

		const auto v = valueCast<BooleanValue>(expression);
		return v->negate();
	}

	std::shared_ptr<soviet::Value> Evaluator::evaluateGreaterThanOpNode(const std::shared_ptr<BinOpNode>& node) {
		const auto left = evaluate(node->left);
		const auto right = evaluate(node->right);
		if (left->type != ValueType::NumberValue
			|| right->type != left->type) {
			throw EvaluateError("unknown operands");
		}

		const auto leftValue = valueCast<NumberValue>(left)->value;
		const auto rightValue = valueCast<NumberValue>(right)->value;
		return std::make_shared<BooleanValue>(
			leftValue > rightValue
		);
	}

	std::shared_ptr<soviet::Value> Evaluator::evaluateNumberNode(const std::shared_ptr<Node>& node) {
		const auto n = nodeCast<NumberNode>(node);
		return std::make_shared<NumberValue>(n->value);
	}

	std::shared_ptr<soviet::Value> Evaluator::evaluateReturnNode(const std::shared_ptr<Node>& node) {
		const auto n = nodeCast<ReturnNode>(node);
		return std::make_shared<ExplicitReturnValue>(
			evaluate(n->returnValue)
		);
	}

	std::shared_ptr<soviet::Value> Evaluator::evaluateNameNode(const std::shared_ptr<Node>& node) {
		const auto n = nodeCast<NameNode>(node);

		for (auto& scope : currentContext) {
			if (scope->variables.contains(n->value))
				return scope->variables[n->value];
		}

		throw EvaluateError("unknow name: " + n->value);
	}

	std::shared_ptr<soviet::Value> Evaluator::evaluateStringNode(const std::shared_ptr<Node>& node) {
		const auto n = nodeCast<StringNode>(node);
		return std::make_shared<StringValue>(n->value);
	}

	std::shared_ptr<soviet::Value> Evaluator::evaluateAddOpNode(const std::shared_ptr<BinOpNode>& node) {
		const auto left = evaluate(node->left);
		const auto right = evaluate(node->right);

		switch (left->type) {
			case ValueType::NumberValue: {
				const auto leftValue = valueCast<NumberValue>(left);
				return leftValue->add(right);
			}
			case ValueType::StringValue: {
				const auto leftValue = valueCast<StringValue>(left);
				return leftValue->add(right);
			}
		}

		throw EvaluateError("Unknown operands");
	}

	std::shared_ptr<soviet::Value> Evaluator::evaluateIfNode(const std::shared_ptr<Node>& node) {
		const auto n = nodeCast<IfNode>(node);

		const auto condition = valueCast<BooleanValue>(evaluate(n->condition));
		if (condition->value)
			return evaluate(n->body);
		else if (n->elseBody)
			return evaluate(n->elseBody);

		return std::make_shared<UndefinedValue>();
	}

	std::shared_ptr<soviet::Value> Evaluator::evaluateWhileLoopNode(const std::shared_ptr<Node>& node) {
		const auto n = nodeCast<WhileLoopNode>(node);

		while (true) {
			const auto condition = evaluate(n->condition);
			if (condition->type != ValueType::BooleanValue)
				throw EvaluateError("while loop condition has to be a boolean");
			const auto value = valueCast<BooleanValue>(condition)->value;
			if (!value)
				break;
			evaluate(n->body);
		}

		return std::make_shared<UndefinedValue>();
	}

	std::shared_ptr<soviet::Value> Evaluator::evaluateSubOpNode(const std::shared_ptr<BinOpNode>& node) {
		const auto left = evaluate(node->left);
		const auto right = evaluate(node->right);

		if (left->type != ValueType::NumberValue)
			throw EvaluateError{"subtraction operands have to be number values"};

		return valueCast<NumberValue>(left)->subtract(right);
	}

	std::shared_ptr<soviet::Value> Evaluator::evaluateMulOpNode(const std::shared_ptr<BinOpNode>& node) {
		const auto left = evaluate(node->left);
		const auto right = evaluate(node->right);

		switch (left->type) {
			case ValueType::NumberValue: {
				const auto leftValue = valueCast<NumberValue>(left);
				return leftValue->multiply(right);
			}
			case ValueType::StringValue: {
				const auto leftValue = valueCast<StringValue>(left);
				return leftValue->multiply(right);
			}
		}

		throw EvaluateError("unknown operands");
	}

	std::shared_ptr<soviet::Value> Evaluator::evaluateDivOpNode(const std::shared_ptr<BinOpNode>& node) {
		const auto left = evaluate(node->left);
		const auto right = evaluate(node->right);

		if (left->type != ValueType::NumberValue)
			throw EvaluateError{"division operands have to be number values"};

		return valueCast<NumberValue>(left)->divide(right);
	}

	std::shared_ptr<soviet::Value> Evaluator::setVariable(const std::string& name, std::shared_ptr<Value> value) {
		for (auto& scope : currentContext) {
			if (scope->variables.contains(name))
				return scope->variables[name] = value;
		}

		currentContext[currentContext.size() - 1]->variables[name] = value;
		return value;
	}

	std::shared_ptr<soviet::ArrayValue> Evaluator::destructure(const std::shared_ptr<ArrayNode>& left, const std::shared_ptr<ArrayValue>& right) {
		std::vector<std::shared_ptr<Value>> result;
		for (size_t i = 0; i < left->elements.size(); ++i) {
			std::shared_ptr<Node>& name = left->elements[i];
			std::shared_ptr<Value> value = right->at(i);

			switch (name->type) {
				case NodeType::NameNode: {
					setVariable(
						nodeCast<NameNode>(name)->value,
						value
					);
					break;
				}
				case NodeType::ArrayNode: {
					if (value->type != ValueType::ArrayValue)
						throw EvaluateError("value destructured to an array must be an array");
					destructure(
						nodeCast<ArrayNode>(name),
						valueCast<ArrayValue>(value)
					);
					break;
				}
				default: {
					const auto leftValue = evaluate(name);
					if (!leftValue->equals(value))
						throw EvaluateError{ "matching failed" };
					break;
				}
			}

			result.push_back(value);
		}

		return std::make_shared<ArrayValue>(result);
	}

	std::shared_ptr<soviet::Value> Evaluator::evaluateEqualsOpNode(const std::shared_ptr<BinOpNode>& node) {
		switch (node->left->type) {
			case NodeType::NameNode: {
				const auto& name = nodeCast<NameNode>(node->left)->value;
				auto value = evaluate(node->right);

				return setVariable(name, value);
			}
			case NodeType::ArrayNode: {
				const auto left = nodeCast<ArrayNode>(node->left);
				const auto _right = evaluate(node->right);
				if (_right->type != ValueType::ArrayValue)
					throw EvaluateError("value destructured to an array must be an array");

				const auto right = valueCast<ArrayValue>(_right);
				if (left->elements.size() != right->size())
					throw EvaluateError("arrays have to be the same length when destructuring");

				return destructure(left, right);
			}
			default:
				throw EvaluateError("Unknown operands");
		}

		throw EvaluateError("Unexpected node");
	}

	std::shared_ptr<soviet::Value> Evaluator::evaluateDoubleEqualsOpNode(const std::shared_ptr<BinOpNode>& node) {
		const auto left = evaluate(node->left);
		const auto right = evaluate(node->right);

		return std::make_shared<BooleanValue>(left->equals(right));
	}

	std::shared_ptr<soviet::Value> Evaluator::evaluateNotEqualsOpNode(const std::shared_ptr<BinOpNode>& node) {
		const auto left = evaluate(node->left);
		const auto right = evaluate(node->right);

		return std::make_shared<BooleanValue>(!left->equals(right));
	}

	std::shared_ptr<soviet::Value> Evaluator::evaluateFuncCallNode(const std::shared_ptr<Node>& node) {
		const auto n = nodeCast<FuncCallNode>(node);
		const auto functionValue = evaluate(n->name);

		if (functionValue->type != ValueType::FunctionValue)
			throw EvaluateError("You can only call a function");

		auto function = valueCast<FunctionValue>(functionValue);

		std::vector<std::shared_ptr<Value>> args;
		for (const auto& argument : n->arguments)
			args.push_back(evaluate(argument));

		return callFunction(function, args);
	}

	std::shared_ptr<soviet::Value> Evaluator::evaluatePrototypeNode(const std::shared_ptr<Node>& node) {
		return std::make_shared<FunctionValue>(
			nodeCast<PrototypeNode>(node),
			currentContext
		);
	}

	std::shared_ptr<soviet::Value> Evaluator::evaluateBlockNode(const std::shared_ptr<Node>& node) {
		const auto n = nodeCast<BlockNode>(node);

		currentContext.emplace_back(*this);
		for (const auto& expr : n->nodes) {
			auto value = evaluate(expr);
			if (value->type == ValueType::ExplicitReturnValue) {
				currentContext.pop_back();
				return value;
			}
		}
		currentContext.pop_back();

		return std::make_shared<UndefinedValue>();
	}

	std::shared_ptr<soviet::Value> Evaluator::evaluateBooleanNode(const std::shared_ptr<Node>& node) {
		const auto n = nodeCast<BooleanNode>(node);
		return std::make_shared<BooleanValue>(n->value);
	}

	std::shared_ptr<Value> Evaluator::evaluateNullNode(const std::shared_ptr<Node>& node) {
		return std::make_shared<NullValue>();
	}

	std::shared_ptr<soviet::Value> Evaluator::evaluateDotOpNode(const std::shared_ptr<BinOpNode>& node) {
		if (node->left->type != NodeType::NameNode)
			throw EvaluateError("tf?!?!");

		const auto& left = nodeCast<NameNode>(node->left)->value;
		const auto& right = nodeCast<NameNode>(node->right)->value;

		bool objectExists = false;
		for (auto& scope : currentContext) {
			if (scope->modules.contains(left)) {
				objectExists = true;

				auto m = scope->modules[left];
				if (m->variables.contains(right))
					return m->variables[right];
			}
		}

		if (objectExists)
			throw EvaluateError("object " + left + " has no property: " + right);
		throw EvaluateError("unknow object: " + left);
	}

	std::shared_ptr<soviet::Value> Evaluator::evaluateModuleNode(const std::shared_ptr<Node>& node) {
		auto moduleNode = nodeCast<ModuleNode>(node);

		auto* m = new Module{ *this };
		m->name = std::move(moduleNode->name);
		for (const auto& [name, value] : moduleNode->members)
			m->variables.emplace(name, evaluate(value));

		if (currentContext.size() != 1)
			throw EvaluateError{"modules can only be declared in global scope"};

		currentContext[0]->modules.emplace(m->name, m);

		return std::make_shared<UndefinedValue>();
	}

	std::shared_ptr<soviet::Value> Evaluator::evaluatePipeOpNode(const std::shared_ptr<Node>& node) {
		const auto pipeOpNode = nodeCast<PipeOpNode>(node);

		if (pipeOpNode->function->type == NodeType::FuncCallNode) {
			const auto funcCallNode = nodeCast<FuncCallNode>(pipeOpNode->function);
			funcCallNode->arguments.insert(funcCallNode->arguments.begin(), pipeOpNode->value);
			return evaluate(funcCallNode);
		} else {
			auto value = evaluate(pipeOpNode->value);

			const auto function = evaluate(pipeOpNode->function);
			if (function->type != ValueType::FunctionValue)
				throw EvaluateError("pipe argument is not a function");

			const auto f = valueCast<FunctionValue>(function);

			std::vector<std::shared_ptr<Value>> args{std::move(value)};
			return callFunction(f, args);
		}
	}

	std::shared_ptr<soviet::Value> Evaluator::evaluateForLoopNode(const std::shared_ptr<Node>& node) {
		auto forLoopNode = nodeCast<ForLoopNode>(node);

		if (forLoopNode->iterator->type != NodeType::NameNode)
			throw EvaluateError("no");

		auto iteratorName = nodeCast<NameNode>(forLoopNode->iterator);
		auto iterableArg = evaluate(forLoopNode->iterable);

		std::shared_ptr<Value> previousReturnValue;

		if (iterableArg->type == ValueType::ArrayValue) {
			auto iterable = valueCast<ArrayValue>(iterableArg);

			for (const auto& element : iterable->getData()) {
				std::shared_ptr<Scope>& loopScope = currentContext.emplace_back(*this);

				loopScope->variables[iteratorName->value] = element;
				previousReturnValue = evaluate(forLoopNode->body);

				currentContext.pop_back();
			}
		} else if (iterableArg->type == ValueType::RangeValue) {
			auto iterable = valueCast<RangeValue>(iterableArg);

			for (size_t i = iterable->from; i < iterable->to; ++i) {
				std::shared_ptr<Scope>& loopScope = currentContext.emplace_back(*this);

				loopScope->variables[iteratorName->value] = std::make_shared<NumberValue>((float) i);
				previousReturnValue = evaluate(forLoopNode->body);

				currentContext.pop_back();
			}
		} else {
			throw EvaluateError{"not iterable"};
		}

		return previousReturnValue;
	}
}