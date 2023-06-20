#include "evaluator.hpp"
#include <fstream>
#include "../file.hpp"

namespace soviet {
	std::shared_ptr<Value> Evaluator::evaluateImportNode(const std::shared_ptr<Node>& node) {
		const auto& n = nodeCast<ImportNode>(node);

		std::ifstream file{n->moduleName};
		if (!file.is_open())
			throw EvaluateError{"file opening error"};

		Evaluator fileEvaluator{ thread };

		while (!file.eof()) {
			getfileline(fileEvaluator, file);
		}

		const auto& fileScope = fileEvaluator.currentContext[0];
		for (const auto& [name, value] : fileScope->variables)
			currentContext[0]->variables[name] = value;
		for (const auto& [name, module] : fileScope->modules) {
			// overriding a module would cause a memory leak
			// since they're allocated manually
			if (!currentContext[0]->modules.contains(name))
				currentContext[0]->modules[name] = module;
		}

		return std::make_shared<UndefinedValue>();
	}
}