#include "GlobalScope.hpp"
#include "modules/Module.hpp"
#include "modules/Array.hpp"
#include "modules/IO.hpp"
#include "modules/Map.hpp"
#include "modules/String.hpp"
#include "modules/Math.hpp"
#include "modules/ThreadModule.hpp"

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
