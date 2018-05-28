#include "frontend/combinators/v3_combinators/helpers/less_than_op_parser.h"
#include <string>

#define super NullParser

using namespace cs160::frontend;

ParseStatus LessThanParser::parse(std::string inputProgram, std::string errorType) {
	trim(inputProgram);
	if (inputProgram.size() == 0) {
		return super::parse(inputProgram);
	}

	ParseStatus status;

	if (inputProgram[0] == '<') {
		status.status = true;
		status.remainingCharacters = inputProgram.erase(0, 1);
	} else {
		return super::parse(inputProgram);
	}
	return status;
}
