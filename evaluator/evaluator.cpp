#include "evaluator.hpp"
#include <fstream>
#include "../file.hpp"
#if defined(_WIN32)
#include <Windows.h>
#endif
#include <filesystem>

namespace soviet {
	typedef void(__stdcall* LoadFunction)(int& value);

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
				[value](Evaluator&, std::vector<std::shared_ptr<Value>>& args) {
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

			Evaluator fileEvaluator{ thread };

			while (!file.eof()) {
				getfileline(fileEvaluator, file);
			}

			const auto& fileScope = fileEvaluator.currentContext[0];
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
}