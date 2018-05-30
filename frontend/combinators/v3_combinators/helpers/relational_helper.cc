#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/or_combinator.h"
#include "frontend/combinators/v3_combinators/helpers/relational_helper.h"

#include <iostream>

#include <string>     // std::string, std::stoi

#define super NullParser

using namespace cs160::frontend;

std::string errorMessage = "Expected relational operator";

ParseStatus EqualToOpParser::parse(std::string inputProgram, std::string errorType) {
	trim(inputProgram);
	if (inputProgram.size() <= 1) {
		return super::parse(inputProgram);
	}

	ParseStatus status;

	if (inputProgram[0] == '=' && inputProgram[1] == '=') {
		status.status = true;
		status.parsedCharacters = "==";
		status.remainingCharacters = inputProgram.erase(0, 2);
	} else {
		return super::parse(inputProgram);
	}
	return status;
}

ParseStatus GreaterThanOpParser::parse(std::string inputProgram, std::string errorType) {
	trim(inputProgram);
	if (inputProgram.size() == 0) {
		return super::parse(inputProgram);
	}

	ParseStatus status;

	if (inputProgram[0] == '>') {
		status.status = true;
		status.parsedCharacters = ">";
		status.remainingCharacters = inputProgram.erase(0, 1);
	} else {
		return super::parse(inputProgram);
	}
	return status;
}

ParseStatus LessThanOpParser::parse(std::string inputProgram, std::string errorType) {
	trim(inputProgram);
	if (inputProgram.size() == 0) {
		return super::parse(inputProgram);
	}

	ParseStatus status;

	if (inputProgram[0] == '<') {
		status.status = true;
		status.parsedCharacters = "<";
		status.remainingCharacters = inputProgram.erase(0, 1);
	} else {
		return super::parse(inputProgram);
	}
	return status;
}

ParseStatus GreaterThanOrEqualToOpParser::parse(std::string inputProgram, std::string errorType) {
	trim(inputProgram);
	if (inputProgram.size() <= 1) {
		return super::parse(inputProgram);
	}

	ParseStatus status;

	if (inputProgram[0] == '>' && inputProgram[1] == '=') {
		status.status = true;
		status.parsedCharacters = ">=";
		status.remainingCharacters = inputProgram.erase(0, 2);
	} else {
		return super::parse(inputProgram);
	}
	return status;
}

ParseStatus LessThanOrEqualToOpParser::parse(std::string inputProgram, std::string errorType) {
	trim(inputProgram);
	if (inputProgram.size() <= 1) {
		return super::parse(inputProgram);
	}

	ParseStatus status;

	if (inputProgram[0] == '<' && inputProgram[1] == '=') {
		status.status = true;
		status.parsedCharacters = "<=";
		status.remainingCharacters = inputProgram.erase(0, 2);
	} else {
		return super::parse(inputProgram);
	}
	return status;
}

ParseStatus AndOpParser::parse(std::string inputProgram, std::string errorType) {
	trim(inputProgram);
	if (inputProgram.size() <= 1) {
		return super::parse(inputProgram);
	}

	ParseStatus status;

	if (inputProgram[0] == '&' && inputProgram[1] == '&') {
		status.status = true;
		status.parsedCharacters = "&&";
		status.remainingCharacters = inputProgram.erase(0, 2);
	} else {
		return super::parse(inputProgram);
	}
	return status;
}

ParseStatus OrOpParser::parse(std::string inputProgram, std::string errorType) {
	trim(inputProgram);
	if (inputProgram.size() <= 1) {
		return super::parse(inputProgram);
	}

	ParseStatus status;

	if (inputProgram[0] == '|' && inputProgram[1] == '|') {
		status.status = true;
		status.parsedCharacters = "||";
		status.remainingCharacters = inputProgram.erase(0, 2);
	} else {
		return super::parse(inputProgram);
	}
	return status;
}

ParseStatus NotOpParser::parse(std::string inputProgram, std::string errorType) {
	trim(inputProgram);
	if (inputProgram.size() == 0) {
		return super::parse(inputProgram);
	}

	ParseStatus status;

	if (inputProgram[0] == '!') {
		status.status = true;
		status.parsedCharacters = "!";
		status.remainingCharacters = inputProgram.erase(0, 1);
	} else {
		return super::parse(inputProgram);
	}
	return status;
}

ParseStatus RelationOperator::parse(std::string inputProgram, std::string errorType) {
	EqualToOpParser equalTo;
  GreaterThanOpParser greaterThan;
  LessThanOpParser lessThan;
  GreaterThanOrEqualToOpParser greaterOrEqual;
  LessThanOrEqualToOpParser lessOrEqual;
    // > or <
  OrCombinator greaterOrLess;
  greaterOrLess.firstParser = reinterpret_cast<NullParser *>(&greaterThan);
  greaterOrLess.secondParser = reinterpret_cast<NullParser *>(&lessThan);
  // >= or <=
  OrCombinator orEqualTo;
  orEqualTo.firstParser = reinterpret_cast<NullParser *>(&greaterOrEqual);
  orEqualTo.secondParser = reinterpret_cast<NullParser *>(&lessOrEqual);
  // > or < or >= or <=
  OrCombinator both;
  both.firstParser = reinterpret_cast<NullParser *>(&orEqualTo);
  both.secondParser = reinterpret_cast<NullParser *>(&greaterOrLess);
  // == or > or < or >= or <=
  OrCombinator all;
  all.firstParser = reinterpret_cast<NullParser *>(&equalTo);
  all.secondParser = reinterpret_cast<NullParser *>(&both);
	
	return all.parse(inputProgram);
}