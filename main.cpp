#ifdef RUN_TESTS
#define CATCH_CONFIG_RUNNER
#include <catch.hpp>
#else
#include "repl.hpp"
#include "file.hpp"
#endif

int main(int argc, char** argv) {
#ifdef RUN_TESTS
	Catch::Session().run(1, argv);
#else
	if (argc == 1) { // REPL
		std::cout << "wellCUM to soviet 2.0 REPL (Really Epic Program Launcher)" << std::endl
			<< std::endl;

		while (true) {
			getline();
		}
	} else {
		std::ifstream file{argv[1]};
		if (!file.is_open()) {
			std::cerr << "no i dupa! cos sie zesralo z plikiem" << std::endl;
			return 1;
		}

		soviet::Thread thread;
		while (!file.eof()) {
			getfileline(*thread.evaluator, file);
		}
	}
#endif
}
