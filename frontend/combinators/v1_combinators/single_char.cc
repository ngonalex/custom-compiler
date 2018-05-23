#include "frontend/combinators/v1_combinators/single_char.h"

#define super NullParser

using namespace cs160::frontend;

ParseStatus SingleCharParser::parse(std::string inputProgram) {
	if (inputProgram.size() == 0){
		return super::parse(inputProgram);
	}

	ParseStatus status;
	if ((inputProgram[0] >= 'a' && inputProgram[0] <= 'z') ||
		(inputProgram[0] >= 'A' && inputProgram[0] <= 'Z') ||
		inputProgram[0] == '_' || inputProgram[0] == ';'){
		status.status = true;
        status.parsedCharacters = inputProgram[0];
		status.remainingCharacters = inputProgram.erase(0, 1);
	} else {
		return super::parse(inputProgram);
	}
	return status;
}
