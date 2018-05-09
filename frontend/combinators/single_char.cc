#include "frontend/combinators/single_char.h"

#define super NullParser

ParseStatus SingleCharParser::parse(std::string inputProgram){
	if (inputProgram.size() == 0){
		return super::parse(inputProgram);
	}

	if ((inputProgram[0] >= 'a' && inputProgram[0] <= 'z') ||
		(inputProgram[0] >= 'A' && inputProgram[0] <= 'Z')){
		ParseStatus status;
		status.status = true;
		status.remainingCharacters = std::erase(inputProgram.begin() + 1, inputProgram.end());
	} else {
		return super::parse(inputProgram);
	}
	return status;
}