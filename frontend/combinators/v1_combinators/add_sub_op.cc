#include "frontend/combinators/v1_combinators/add_sub_op.h"
#include <string>

#define super NullParser

using namespace cs160::frontend;

ParseStatus AddSubOpParser::parse(std::string inputProgram) {
	if (inputProgram.size() == 0) {
		return super::parse(inputProgram);
	}
	ParseStatus status;

	if (inputProgram[0] == '+' || inputProgram[0] == '-') {
		status.status = true;
		status.parsedCharacters = inputProgram[0];
		status.remainingCharacters = inputProgram.erase(0, 1);
	} else {
		return super::parse(inputProgram);
	}
	return status;
}