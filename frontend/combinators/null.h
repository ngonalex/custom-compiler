#ifndef NULL_PARSER_H
#define NULL_PARSER_H

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/tokenizer/token.h"

struct ParseStatus {
	bool status;
	std::string remainingCharacters;
};

class NullParser
{
	virtual ParseStatus parse(std::string inputProgram);
};

#endif NULL_PARSER_H