#include "frontend/combinators/v2_combinators/helpers/v1_helpers.h"

#include <string>     // std::string, std::stoi

#define super NullParser

using namespace cs160::frontend;
using namespace std;

// )
ParseStatus CloseParenParser::parse(std::string inputProgram, std::string errorType) {
	trim(inputProgram);
	std::string errorMessage = "Expecting close paranthesis";

	if (inputProgram.size() == 0) {
		return super::parse(inputProgram, errorMessage);
	}

	ParseStatus status;
	if (inputProgram[0] == ')') {
		status.status = true;
		status.remainingCharacters = inputProgram.erase(0, 1);
		status.parsedCharacters = ')';
	} else {
		return super::parse(inputProgram, errorMessage);
	}
	return status;
}

// (
ParseStatus OpenParenParser::parse(std::string inputProgram, std::string errorType) {
	trim(inputProgram);

	std::string errorMessage = "Expecting open paranthesis";

	if (inputProgram.size() == 0) {
		return super::parse(inputProgram, errorMessage);
	}

	ParseStatus status;
	if (inputProgram[0] == '(') {
		status.status = true;
		status.remainingCharacters = inputProgram.erase(0, 1);
		status.parsedCharacters = '(';
	} else {
		return super::parse(inputProgram, errorMessage);
	}
	return status;
}

// - 
ParseStatus NegativeParser::parse(std::string inputProgram, std::string errorType) {
	trim(inputProgram);
	std::string errorMessage = "Expecting -";


	if (inputProgram.size() == 0) {
		return super::parse(inputProgram, errorMessage);
	}

	ParseStatus status;

	if (inputProgram[0] == '-') {
		status.status = true;
		status.remainingCharacters = inputProgram.erase(0, 1);
		status.parsedCharacters = '-';
	} else {
		return super::parse(inputProgram, errorMessage);
	}
	return status;
}

// + - 
ParseStatus AddSubOpParser::parse(std::string inputProgram, std::string errorType) {
	trim(inputProgram);
	std::string errorMessage = "Expecting + or -";

	if (inputProgram.size() == 0) {
		return super::parse(inputProgram, errorMessage);
	}
	ParseStatus status;

	if (inputProgram[0] == '+' || inputProgram[0] == '-') {
		status.status = true;
		status.parsedCharacters = inputProgram[0];
		status.remainingCharacters = inputProgram.erase(0, 1);
	} else {
		return super::parse(inputProgram, errorMessage);
	}
	return status;
}

// * /
ParseStatus MulDivOpParser::parse(std::string inputProgram, std::string errorType) {
	trim(inputProgram);
	std::string errorMessage = "Expecting * or /";

	if (inputProgram.size() == 0) {
		return super::parse(inputProgram, errorMessage);
	}

	ParseStatus status;

	if (inputProgram[0] == '*' || inputProgram[0] == '/') {
		status.status = true;
		status.parsedCharacters = inputProgram[0];
		status.remainingCharacters = inputProgram.erase(0, 1);
	} else {
		return super::parse(inputProgram, errorMessage);
	}
	return status;
}

// ;
ParseStatus SemiColonParser::parse(std::string inputProgram, std::string errorType) {
	trim(inputProgram);
	std::string errorMessage = "Missing semicolon";

	if (inputProgram.size() == 0) {
		return super::parse(inputProgram, errorMessage);
	}

	ParseStatus status;
	if ((inputProgram[0] == ';')){
		status.status = true;
		status.parsedCharacters = inputProgram[0];
		status.remainingCharacters = inputProgram.erase(0, 1);
	} else {
		return super::parse(inputProgram, errorMessage);
	}
	return status;
}

