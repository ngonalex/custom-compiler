#include "frontend/combinators/null.h"

ParseStatus NullParser::parse(std::string inputProgram){
	ParseStatus status;
	status.status = false;
	status.remainingCharacters = inputProgram;
	return status;
}
