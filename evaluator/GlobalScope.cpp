#include "GlobalScope.hpp"

namespace soviet {
	GlobalScope::GlobalScope(Evaluator& evaluator) : Scope{evaluator} {
		modules.insert({"Array", new ArrayModule{evaluator}});
		modules.insert({"Map", new MapModule{evaluator}});
		modules.insert({"IO", new IOModule{evaluator}});
		modules.insert({"String", new StringModule{evaluator}});
		modules.insert({"Math", new MathModule{evaluator}});
		modules.insert({"Thread", new ThreadModule{evaluator}});
	}
}
