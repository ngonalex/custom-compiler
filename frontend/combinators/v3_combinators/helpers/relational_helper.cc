#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/v3_combinators/helpers/relational_helper.h"

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

	if (inputProgram[0] == '>' && inputProgram[1] == '=') {
		status.status = true;
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
		status.remainingCharacters = inputProgram.erase(0, 1);
	} else {
		return super::parse(inputProgram);
	}
	return status;
}