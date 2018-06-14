#include "frontend/combinators/v1_combinators/semicolon_parser.h"

#define super NullParser

using namespace cs160::frontend;

ParseStatus SemiColonParser::parse(std::string inputProgram, std::string errorType) {
	trim(inputProgram);
	std::string errorMessage = "Missing semicolon";

	if (inputProgram.size() == 0) {
		return super::parse(inputProgram, errorMessage);
	}

	ParseStatus status;
	if ((inputProgram[0] == ';')){
		status.status = true;
		status.parsedCharacters = inputProgram[0];
		status.remainingCharacters = inputProgram.erase(0, 1);
	} else {
		return super::parse(inputProgram, errorMessage);
	}
	return status;
}
