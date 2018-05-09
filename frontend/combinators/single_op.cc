#include "frontend/combinators/single_op.h"

#define super NullParser

ParseStatus SingleOperatorParser::parse(std::string inputProgram){
	if (inputProgram.size() == 0){
		return super::parse(inputProgram);
	}

	if (inputProgram[0] == '+' || inputProgram[0] == '-' ||
		(inputProgram[0] == '*' || inputProgram[0] == '/')) {
		ParseStatus status;
		status.status = true;
		status.remainingCharacters = std::erase(inputProgram.begin() + 1, inputProgram.end());
	} else {
		return super::parse(inputProgram);
	}
	return status;
}