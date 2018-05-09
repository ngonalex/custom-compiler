#ifndef NULL_PARSER_H
#define NULL_PARSER_H

#include "abstract_syntax/abstract_syntax.h"

struct ParseStatus {
	bool status;
	std::string remainingCharacters;
};

struct SuccessStatus{
	struct ParseStatus;
}

// check if two ParseStatuses are equal
bool operator==(const ParseStatus &a, const ParseStatus &b);

bool operator!=(const ParseStatus &a, const ParseStatus &b);

class NullParser
{
public:
	virtual ParseStatus parse(std::string inputProgram);
};

#endif NULL_PARSER_H
