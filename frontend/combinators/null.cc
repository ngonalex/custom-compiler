#include "frontend/combinators/null.h"

bool operator==(const ParseStatus &a, const ParseStatus &b) {
	return (a.status == b.status && a.remainingCharacters == b.remainingCharacters);
}

bool operator!=(const ParseStatus &a, const ParseStatus &b) {
	return (a.status != b.status || a.remainingCharacters != b.remainingCharacters);
}

ParseStatus NullParser::parse(std::string inputProgram){
	ParseStatus status;
	status.status = false;
	status.remainingCharacters = inputProgram;
	return status;
}
