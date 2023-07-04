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
			previousValue = evaluateNumberNode(node); break;
		case NodeType::NameNode:
			previousValue = evaluateNameNode(node); break;
		case NodeType::StringNode:
			previousValue = evaluateStringNode(node); break;
		case NodeType::FuncCallNode:
			previousValue = evaluateFuncCallNode(node); break;
		case NodeType::IfNode:
			previousValue = evaluateIfNode(node); break;
		case NodeType::PrototypeNode:
			previousValue = evaluatePrototypeNode(node); break;
		case NodeType::BlockNode:
			previousValue = evaluateBlockNode(node); break;
		case NodeType::ReturnNode:
			previousValue = evaluateReturnNode(node); break;
		case NodeType::NegationNode:
			previousValue = evaluateNegationNode(node); break;
		case NodeType::BooleanNode:
			previousValue = evaluateBooleanNode(node); break;
		case NodeType::ArrayNode:
			previousValue = evaluateArrayNode(node); break;
		case NodeType::WhileLoopNode:
			previousValue = evaluateWhileLoopNode(node); break;
		case NodeType::AddOpNode:
			previousValue = evaluateAddOpNode(node); break;
		case NodeType::SubOpNode:
			previousValue = evaluateSubOpNode(node); break;
		case NodeType::MulOpNode:
			previousValue = evaluateMulOpNode(node); break;
		case NodeType::DivOpNode:
			previousValue = evaluateDivOpNode(node); break;
		case NodeType::EqualsOpNode:
			previousValue = evaluateEqualsOpNode(node); break;
		case NodeType::DoubleEqualsOpNode:
			previousValue = evaluateDoubleEqualsOpNode(node); break;
		case NodeType::DotOpNode:
			previousValue = evaluateDotOpNode(node); break;
		case NodeType::GreaterThanOpNode:
			previousValue = evaluateGreaterThanOpNode(node); break;
		case NodeType::GreaterThanOrEqualOpNode:
			previousValue = evaluateGreaterThanOrEqualOpNode(node); break;
		case NodeType::LessThanOpNode:
			previousValue = evaluateLessThanOpNode(node); break;
		case NodeType::LessThanOrEqualOpNode:
			previousValue = evaluateLessThanOrEqualOpNode(node); break;
		case NodeType::NotEqualsOpNode:
			previousValue = evaluateNotEqualsOpNode(node); break;
		case NodeType::ModuleNode:
			previousValue = evaluateModuleNode(node); break;
		case NodeType::ForLoopNode:
			previousValue = evaluateForLoopNode(node); break;
		case NodeType::PipeOpNode:
			previousValue = evaluatePipeOpNode(node); break;
		case NodeType::ImportNode:
			previousValue = evaluateImportNode(node); break;
		case NodeType::NullNode:
			previousValue = evaluateNullNode(node); break;
		case NodeType::AtOpNode:
			previousValue = evaluateAtOpNode(node); break;
		case NodeType::AndNode:
			previousValue = evaluateAndNode(node); break;
		case NodeType::OrNode:
			previousValue = evaluateOrNode(node); break;
		default:
			throw EvaluateError("Unexpected node");
		}

		if (node->type != NodeType::EqualsOpNode)
			recentAssignment = nullptr;

		return previousValue;
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

		std::shared_ptr<Scope>& functionScope = currentContext.emplace_back(std::make_shared<Scope>(*this));
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

			if (currentContext[0]->modules.contains("Test"))
				delete currentContext[0]->modules["Test"];
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

	std::shared_ptr<Value> Evaluator::evaluateLessThanOpNode(const std::shared_ptr<Node>& n) {
		const auto& node = nodeCast<LessThanOpNode>(n);

		const auto left = evaluate(node->left);
		const auto right = evaluate(node->right);
		if (left->type != ValueType::NumberValue || right->type != ValueType::NumberValue) {
			throw EvaluateError("unknown operands");
		}

		const auto& leftNum = valueCast<NumberValue>(left);
		const auto& rightNum = valueCast<NumberValue>(right);
		return std::make_shared<BooleanValue>(
			leftNum->compare(rightNum) == NumberComparisonResult::LESS
		);
	}

	std::shared_ptr<Value> Evaluator::evaluateArrayNode(const std::shared_ptr<Node>& node) {
		const auto& n = nodeCast<ArrayNode>(node);

		std::vector<std::shared_ptr<Value>> elements;
		for (const auto& elementNode : n->elements)
			elements.push_back(evaluate(elementNode));

		return std::make_shared<ArrayValue>(
			std::move(elements)
		);
	}

	std::shared_ptr<Value> Evaluator::evaluateGreaterThanOrEqualOpNode(const std::shared_ptr<Node>& n) {
		const auto& node = nodeCast<GreaterThanOrEqualOpNode>(n);

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

	std::shared_ptr<Value> Evaluator::evaluateLessThanOrEqualOpNode(const std::shared_ptr<Node>& n) {
		const auto& node = nodeCast<LessThanOrEqualOpNode>(n);

		const auto left = evaluate(node->left);
		const auto right = evaluate(node->right);
		if (left->type != ValueType::NumberValue || right->type != left->type) {
			throw EvaluateError("unknown operands");
		}

		const auto leftNum = valueCast<NumberValue>(left);
		const auto rightNum = valueCast<NumberValue>(right);
		const auto comparison = leftNum->compare(rightNum);
		return std::make_shared<BooleanValue>(
			comparison == NumberComparisonResult::LESS || comparison == NumberComparisonResult::EQUAL
		);
	}

	std::shared_ptr<Value> Evaluator::evaluateNegationNode(const std::shared_ptr<Node>& node) {
		const auto& n = nodeCast<NegationNode>(node);

		const auto expression = evaluate(n->expression);
		if (expression->type != ValueType::BooleanValue)
			throw EvaluateError("jakas dziwna liczba mi sie tu dzieje");

		const auto v = valueCast<BooleanValue>(expression);
		return v->negate();
	}

	std::shared_ptr<Value> Evaluator::evaluateGreaterThanOpNode(const std::shared_ptr<Node>& n) {
		const auto& node = nodeCast<GreaterThanOpNode>(n);

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

	std::shared_ptr<Value> Evaluator::evaluateNumberNode(const std::shared_ptr<Node>& node) {
		const auto& n = nodeCast<NumberNode>(node);
		return std::make_shared<NumberValue>(n->value);
	}

	std::shared_ptr<Value> Evaluator::evaluateReturnNode(const std::shared_ptr<Node>& node) {
		const auto& n = nodeCast<ReturnNode>(node);
		return std::make_shared<ExplicitReturnValue>(
			evaluate(n->returnValue)
		);
	}

	std::shared_ptr<Value> Evaluator::evaluateNameNode(const std::shared_ptr<Node>& node) {
		const auto& n = nodeCast<NameNode>(node);

		const auto value = resolveName(n->value);
		if (!value)
			throw EvaluateError("unknow name: " + n->value);
		
		return value;
	}

	std::shared_ptr<Value> Evaluator::evaluateStringNode(const std::shared_ptr<Node>& node) {
		const auto n = nodeCast<StringNode>(node);
		return std::make_shared<StringValue>(n->value);
	}

	std::shared_ptr<Value> Evaluator::evaluateAddOpNode(const std::shared_ptr<Node>& n) {
		const auto& node = nodeCast<AddOpNode>(n);

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

	std::shared_ptr<Value> Evaluator::evaluateIfNode(const std::shared_ptr<Node>& node) {
		const auto& n = nodeCast<IfNode>(node);

		const auto condition = valueCast<BooleanValue>(evaluate(n->condition));
		if (condition->value)
			return evaluate(n->body);
		else if (n->elseBody)
			return evaluate(n->elseBody);

		return std::make_shared<UndefinedValue>();
	}

	std::shared_ptr<Value> Evaluator::evaluateWhileLoopNode(const std::shared_ptr<Node>& node) {
		const auto& n = nodeCast<WhileLoopNode>(node);

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

	std::shared_ptr<Value> Evaluator::evaluateSubOpNode(const std::shared_ptr<Node>& n) {
		const auto& node = nodeCast<SubOpNode>(n);

		const auto left = evaluate(node->left);
		const auto right = evaluate(node->right);

		if (left->type != ValueType::NumberValue)
			throw EvaluateError{"subtraction operands have to be number values"};

		return valueCast<NumberValue>(left)->subtract(right);
	}

	std::shared_ptr<Value> Evaluator::evaluateMulOpNode(const std::shared_ptr<Node>& n) {
		const auto& node = nodeCast<MulOpNode>(n);

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

	std::shared_ptr<Value> Evaluator::evaluateDivOpNode(const std::shared_ptr<Node>& n) {
		const auto& node = nodeCast<DivOpNode>(n);

		const auto left = evaluate(node->left);
		const auto right = evaluate(node->right);

		if (left->type != ValueType::NumberValue)
			throw EvaluateError{"division operands have to be number values"};

		return valueCast<NumberValue>(left)->divide(right);
	}

	std::shared_ptr<Value> Evaluator::setVariable(const std::string& name, std::shared_ptr<Value> value) {
		for (auto& scope : currentContext) {
			if (scope->variables.contains(name))
				return scope->variables[name] = value;
		}

		currentContext[currentContext.size() - 1]->variables[name] = value;
		return value;
	}

	void Evaluator::assign(const std::shared_ptr<Node>& left, const std::shared_ptr<Value>& right) {
		switch (left->type) {
			case NodeType::NameNode: {
				const auto& name = nodeCast<NameNode>(left)->value;
				setVariable(name, right);
				return;
			}
			case NodeType::ArrayNode: {
				const auto& leftArray = nodeCast<ArrayNode>(left);
				if (right->type != ValueType::ArrayValue)
					throw EvaluateError("value destructured to an array must be an array");

				const auto& rightArray = valueCast<ArrayValue>(right);
				if (leftArray->elements.size() != rightArray->size())
					throw EvaluateError("arrays have to be the same length when destructuring");

				destructure(leftArray, rightArray);
				return;
			}
			default:
				throw EvaluateError("Unknown operands");
		}
	}

	std::shared_ptr<Value> Evaluator::resolveName(const std::string& name) {
		for (auto& scope : currentContext) {
			if (scope->variables.contains(name))
				return scope->variables[name];
		}
		return nullptr;
	}

	std::shared_ptr<ArrayValue> Evaluator::destructure(const std::shared_ptr<ArrayNode>& left, const std::shared_ptr<ArrayValue>& right) {
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

	std::shared_ptr<Value> Evaluator::evaluateEqualsOpNode(const std::shared_ptr<Node>& n) {
		const auto& node = nodeCast<EqualsOpNode>(n);

		const auto& right = evaluate(node->right);
		assign(recentAssignment = node->left, right);
		return right;
	}

	std::shared_ptr<Value> Evaluator::evaluateDoubleEqualsOpNode(const std::shared_ptr<Node>& n) {
		const auto& node = nodeCast<DoubleEqualsOpNode>(n);

		const auto left = evaluate(node->left);
		const auto right = evaluate(node->right);

		return std::make_shared<BooleanValue>(left->equals(right));
	}

	std::shared_ptr<Value> Evaluator::evaluateNotEqualsOpNode(const std::shared_ptr<Node>& n) {
		const auto& node = nodeCast<NotEqualsOpNode>(n);

		const auto left = evaluate(node->left);
		const auto right = evaluate(node->right);

		return std::make_shared<BooleanValue>(!left->equals(right));
	}

	std::shared_ptr<Value> Evaluator::evaluateFuncCallNode(const std::shared_ptr<Node>& node) {
		const auto& n = nodeCast<FuncCallNode>(node);
		const auto functionValue = evaluate(n->name);

		if (functionValue->type != ValueType::FunctionValue)
			throw EvaluateError("You can only call a function");

		const auto& function = valueCast<FunctionValue>(functionValue);

		std::vector<std::shared_ptr<Value>> args;
		args.reserve(n->arguments.size());
		for (const auto& argument : n->arguments)
			args.push_back(evaluate(argument));

		return callFunction(function, args);
	}

	std::shared_ptr<Value> Evaluator::evaluatePrototypeNode(const std::shared_ptr<Node>& node) {
		return std::make_shared<FunctionValue>(
			nodeCast<PrototypeNode>(node),
			currentContext
		);
	}

	std::shared_ptr<Value> Evaluator::evaluateBlockNode(const std::shared_ptr<Node>& node) {
		const auto& n = nodeCast<BlockNode>(node);

		currentContext.emplace_back(std::make_shared<Scope>(*this));
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

	std::shared_ptr<Value> Evaluator::evaluateBooleanNode(const std::shared_ptr<Node>& node) {
		const auto& n = nodeCast<BooleanNode>(node);
		return std::make_shared<BooleanValue>(n->value);
	}

	std::shared_ptr<Value> Evaluator::evaluateNullNode(const std::shared_ptr<Node>& node) {
		return std::make_shared<NullValue>();
	}

	std::shared_ptr<Value> Evaluator::evaluateDotOpNode(const std::shared_ptr<Node>& n) {
		const auto& node = nodeCast<DotOpNode>(n);

		if (node->right->type != NodeType::NameNode)
			throw EvaluateError{"right operand has to be a name"};
		const auto& right = nodeCast<NameNode>(node->right)->value;

		if (node->left->type == NodeType::NameNode) {
			const auto& left = nodeCast<NameNode>(node->left)->value;

			if (currentContext[0]->modules.contains(left)) {
				auto m = currentContext[0]->modules[left];
				if (!m->variables.contains(right))
					throw EvaluateError("object " + left + " has no property: " + right);

				return m->variables[right];
			}
		}

		const auto value = evaluate(node->left);
		if (value->type != ValueType::MapValue)
			throw EvaluateError("not a map");

		return valueCast<MapValue>(value)->get(
			std::make_shared<StringValue>(right)
		);
	}

	std::shared_ptr<Value> Evaluator::evaluateAtOpNode(const std::shared_ptr<Node>& node) {
		const auto& n = nodeCast<AtOpNode>(node);

		const auto array = evaluate(n->array);
		if (array->type == ValueType::ArrayValue) {
			const auto index = evaluate(n->index);
			if (index->type != ValueType::NumberValue)
				throw EvaluateError("right operand has to be a number");

			return valueCast<ArrayValue>(array)->at(
				(size_t) valueCast<NumberValue>(index)->value
			);
		}

		if (array->type == ValueType::MapValue) {
			const auto key = evaluate(n->index);
			return valueCast<MapValue>(array)->get(key);
		}

		throw EvaluateError{"unknown object"};
	}

	std::shared_ptr<Value> Evaluator::evaluateOrNode(const std::shared_ptr<Node>& n) {
		const auto& node = nodeCast<OrNode>(n);
		
		const auto left = evaluate(node->left);
		const auto right = evaluate(node->right);

		if (left->type != ValueType::BooleanValue || right->type != ValueType::BooleanValue)
			throw EvaluateError{"operands have to be boolean"};

		const auto leftValue = valueCast<BooleanValue>(left)->value;
		const auto rightValue = valueCast<BooleanValue>(right)->value;
		return std::make_shared<BooleanValue>(leftValue || rightValue);
	}

	std::shared_ptr<Value> Evaluator::evaluateAndNode(const std::shared_ptr<Node>& n) {
		const auto& node = nodeCast<AndNode>(n);

		const auto left = evaluate(node->left);
		const auto right = evaluate(node->right);

		if (left->type != ValueType::BooleanValue || right->type != ValueType::BooleanValue)
			throw EvaluateError{"operands have to be boolean"};

		const auto leftValue = valueCast<BooleanValue>(left)->value;
		const auto rightValue = valueCast<BooleanValue>(right)->value;
		return std::make_shared<BooleanValue>(leftValue && rightValue);
	}

	std::shared_ptr<Value> Evaluator::evaluateModuleNode(const std::shared_ptr<Node>& node) {
		const auto& moduleNode = nodeCast<ModuleNode>(node);

		auto* m = new Module{*this};
		m->name = std::move(moduleNode->name);
		for (const auto& [name, value] : moduleNode->members)
			m->variables.emplace(name, evaluate(value));

		if (currentContext.size() != 1)
			throw EvaluateError{"modules can only be declared in global scope"};

		if (currentContext[0]->modules.contains(m->name))
			delete currentContext[0]->modules[m->name];
		currentContext[0]->modules.emplace(m->name, m);

		return std::make_shared<UndefinedValue>();
	}

	std::shared_ptr<Value> Evaluator::evaluatePipeOpNode(const std::shared_ptr<Node>& node) {
		const auto& pipeOpNode = nodeCast<PipeOpNode>(node);

		std::shared_ptr<Node> recentAssignment = this->recentAssignment;

		auto value = pipeOpNode->value ? evaluate(pipeOpNode->value) : previousValue;
		if (!value)
			throw EvaluateError{"unexpected pipe operator"};

		std::shared_ptr<Value> pipeResult;

		if (pipeOpNode->function->type == NodeType::FuncCallNode) {
			const auto& funcCallNode = nodeCast<FuncCallNode>(pipeOpNode->function);
			const auto functionValue = evaluate(funcCallNode->name);

			if (functionValue->type != ValueType::FunctionValue)
				throw EvaluateError{"not a function"};

			std::vector<std::shared_ptr<Value>> arguments;
			arguments.reserve(funcCallNode->arguments.size() + 1);
			arguments.push_back(value);
			for (const auto& argNode : funcCallNode->arguments)
				arguments.push_back(evaluate(argNode));

			pipeResult = callFunction(valueCast<FunctionValue>(functionValue), arguments);
		} else {
			const auto function = evaluate(pipeOpNode->function);
			if (function->type != ValueType::FunctionValue)
				throw EvaluateError("pipe argument is not a function");

			const auto f = valueCast<FunctionValue>(function);

			std::vector<std::shared_ptr<Value>> args{std::move(value)};
			pipeResult = callFunction(f, args);
		}

		if (!pipeOpNode->value && recentAssignment)
			assign(recentAssignment, pipeResult);

		return pipeResult;
	}

	std::shared_ptr<Value> Evaluator::evaluateForLoopNode(const std::shared_ptr<Node>& node) {
		const auto& forLoopNode = nodeCast<ForLoopNode>(node);

		if (forLoopNode->iterator->type != NodeType::NameNode)
			throw EvaluateError("no");

		auto iteratorName = nodeCast<NameNode>(forLoopNode->iterator);
		auto iterableArg = evaluate(forLoopNode->iterable);

		std::shared_ptr<Value> previousReturnValue;

		if (iterableArg->type == ValueType::ArrayValue) {
			auto iterable = valueCast<ArrayValue>(iterableArg);

			for (const auto& element : iterable->getData()) {
				std::shared_ptr<Scope>& loopScope = currentContext.emplace_back(std::make_shared<Scope>(*this));

				loopScope->variables[iteratorName->value] = element;
				previousReturnValue = evaluate(forLoopNode->body);

				currentContext.pop_back();
			}
		} else if (iterableArg->type == ValueType::RangeValue) {
			auto iterable = valueCast<RangeValue>(iterableArg);

			for (size_t i = iterable->from; i < iterable->to; ++i) {
				std::shared_ptr<Scope>& loopScope = currentContext.emplace_back(std::make_shared<Scope>(*this));

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