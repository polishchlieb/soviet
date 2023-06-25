#pragma once
#include <fstream>
#include <string>
#include "tokenizer/Tokenizer.hpp"
#include "parser/Parser.hpp"

namespace soviet {
	class Evaluator;
}

void getfileline(soviet::Evaluator& evaluator, std::ifstream& file, std::string initial = "");