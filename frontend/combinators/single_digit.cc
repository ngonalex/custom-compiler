#include "frontend/combinators/single_char.h"

#define super NullParser

ParseStatus SingleDigitParser::parse(std::string inputProgram){
	if (inputProgram.size() == 0){
		return super::parse(inputProgram);
	}

	if ((inputProgram[0] >= '0' && inputProgram[0] <= '9')){
		ParseStatus status;
		status.status = true;
		status.remainingCharacters = std::erase(inputProgram.begin() + 1, inputProgram.end());
	} else {
		return super::parse(inputProgram);
	}
	return status;
}