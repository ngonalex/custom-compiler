#ifndef PARSESTATUS_H_
#define PARSESTATUS_H_

#include "abstract_syntax/abstract_syntax.h"
#include "utility/memory.h"

#include <string>

class ParseStatus { // Super class
 public:
	bool status;
	std::string remainingCharacters;
	std::string parsedCharacters;

	// Only for the success case
	std::unique_ptr<const AstNode> ast;

	// Only for the failed case
	std::string expectedCharacters;

	// check if two ParseStatuses are equal
	bool operator==(const ParseStatus &b) const;

	bool operator!=(const ParseStatus &b) const;
};

#endif // PARSESTATUS_H_
