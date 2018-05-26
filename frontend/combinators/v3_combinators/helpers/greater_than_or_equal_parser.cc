#include "frontend/combinators/v3_combinators/helpers/greater_than_or_equal_parser.h"
#include <string>

#define super NullParser

using namespace cs160::frontend;

ParseStatus GreaterThanOrEqualToParser::parse(std::string inputProgram) {
	trim(inputProgram);
	if (inputProgram.size() <= 1) {
		return super::parse(inputProgram);
	}

	ParseStatus status;

	if (inputProgram[0] == '>' && inputProgram[1] == '=') {
		status.status = true;
		status.remainingCharacters = inputProgram.erase(0, 2);
	} else {
		return super::parse(inputProgram);
	}
	return status;
}
