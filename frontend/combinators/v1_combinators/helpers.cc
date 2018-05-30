#include "frontend/combinators/v2_combinators/helpers/var_helper.h"
#include "frontend/combinators/basic_combinators/one_or_more_combinator.h"
#include "frontend/combinators/v1_combinators/single_char.h"
#include "frontend/combinators/v2_combinators/main/word_parser.h"
#include "frontend/combinators/v1_combinators/semicolon_parser.h"
#include "frontend/combinators/v1_combinators/num_parser.h"
#include "frontend/combinators/basic_combinators/or_combinator.h"

#include <string>     // std::string, std::stoi

#define super NullParser

using namespace cs160::frontend;

P#define super NullParser

using namespace cs160::frontend;

ParseStatus AddSubOpParser::parse(std::string inputProgram, std::string errorType) {
	std::string errorMessage = "Expecting + or -";

	if (inputProgram.size() == 0) {
		return super::parse(inputProgram, errorMessage);
	}
  
  OrCombinator orC;
  SingleCharParser plus;
  SingleCharParser minus;

	if (inputProgram[0] == '+' || inputProgram[0] == '-') {
		status.status = true;
		status.parsedCharacters = inputProgram[0];
		status.remainingCharacters = inputProgram.erase(0, 1);
	} else {
		return super::parse(inputProgram, errorMessage);
	}
	return status;
}