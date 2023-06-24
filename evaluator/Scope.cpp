#include "Scope.hpp"

namespace soviet {
	Scope::Scope(Evaluator& evaluator)
		: evaluator{evaluator}
	{}

	void Scope::merge(Scope* other) {
		for (const auto& [name, value] : other->variables)
			variables.insert({ name, value });
	}

	void Scope::merge(Scope& other) {
		this->merge(&other);
	}

	Scope::~Scope() {
		for (auto& mod : modules)
			delete mod.second;
	}
}