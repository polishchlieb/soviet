#pragma once
#include "Module.hpp"
#include "../values/values.hpp"

namespace soviet {
	class ArrayModule : public Module {
	public:
		std::string name = "Array";
		ArrayModule(Evaluator& evaluator);

	private:
		static std::shared_ptr<Value> newarray(Evaluator&, std::vector<std::shared_ptr<Value>>& args);
		static std::shared_ptr<Value> at(Evaluator&, std::vector<std::shared_ptr<Value>>& args);
		static std::shared_ptr<Value> remove_at(Evaluator&, std::vector<std::shared_ptr<Value>>& args);
		static std::shared_ptr<Value> remove(Evaluator&, std::vector<std::shared_ptr<Value>>& args);
		static std::shared_ptr<Value> map(Evaluator&, std::vector<std::shared_ptr<Value>>& args);
		static std::shared_ptr<Value> for_each(Evaluator&, std::vector<std::shared_ptr<Value>>& args);
		static std::shared_ptr<Value> add(Evaluator&, std::vector<std::shared_ptr<Value>>& args);
		static std::shared_ptr<Value> filter(Evaluator&, std::vector<std::shared_ptr<Value>>& args);
		static std::shared_ptr<Value> contains(Evaluator&, std::vector<std::shared_ptr<Value>>& args);
		static std::shared_ptr<Value> slice(Evaluator&, std::vector<std::shared_ptr<Value>>& args);
		static std::shared_ptr<Value> length(Evaluator&, std::vector<std::shared_ptr<Value>>& args);
		static std::shared_ptr<Value> find(Evaluator&, std::vector<std::shared_ptr<Value>>& args);
		static std::shared_ptr<Value> find_index(Evaluator&, std::vector<std::shared_ptr<Value>>& args);
		static std::shared_ptr<Value> index_of(Evaluator&, std::vector<std::shared_ptr<Value>>& args);
		static std::shared_ptr<Value> concat(Evaluator&, std::vector<std::shared_ptr<Value>>& args);
		static std::shared_ptr<Value> some(Evaluator&, std::vector<std::shared_ptr<Value>>& args);
		static std::shared_ptr<Value> sort(Evaluator&, std::vector<std::shared_ptr<Value>>& args);
		static std::shared_ptr<Value> flat(Evaluator&, std::vector<std::shared_ptr<Value>>& args);
		static std::shared_ptr<Value> is_array(Evaluator&, std::vector<std::shared_ptr<Value>>& args);
		static std::shared_ptr<Value> reduce(Evaluator&, std::vector<std::shared_ptr<Value>>& args);
		static std::shared_ptr<Value> reverse(Evaluator&, std::vector<std::shared_ptr<Value>>& args);
		static std::shared_ptr<Value> reduce_right(Evaluator&, std::vector<std::shared_ptr<Value>>& args);
		static std::shared_ptr<Value> join(Evaluator&, std::vector<std::shared_ptr<Value>>& args);
		static std::shared_ptr<Value> range(Evaluator&, std::vector<std::shared_ptr<Value>>& args);

		static std::shared_ptr<ArrayValue> _flat(std::shared_ptr<ArrayValue> value);
	};
}