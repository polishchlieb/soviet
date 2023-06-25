#include "colors.hpp"
#include <stdexcept>

namespace soviet {
	const char* color(Color color)
	{
		switch (color) {
			case Color::RED:
			#ifdef unix
				return "\x1B[31m";
			#else
				return "";
			#endif
			case Color::RESET:
			#ifdef unix
				return "\033[0m\t\t";
			#else
				return "";
			#endif
			default:
				throw std::runtime_error("Unknown color");
		}
	}

}