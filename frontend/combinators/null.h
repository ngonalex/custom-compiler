#ifndef NULL_PARSER_H
#define NULL_PARSER_H

#include "abstract_syntax/abstract_syntax.h"
#include "parsestatus.h"

class NullParser
{
public:
	virtual ParseStatus parse(std::string inputProgram);
};

#endif NULL_PARSER_H
