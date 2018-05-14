#include "frontend/combinators/v1_combinators/single_char.h"

#define super NullParser

ParseStatus SingleCharParser::parse(std::string inputProgram) {
	if (inputProgram.size() == 0){
		return super::parse(inputProgram);
	}

	ParseStatus status;
	if ((inputProgram[0] >= 'a' && inputProgram[0] <= 'z') ||
		(inputProgram[0] >= 'A' && inputProgram[0] <= 'Z')){
		status.status = true;
		status.remainingCharacters = inputProgram.erase(0, 1);
	} else {
		return super::parse(inputProgram);
	}
	return status;
}
