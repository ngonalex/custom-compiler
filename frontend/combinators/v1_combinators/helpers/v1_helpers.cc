#include "frontend/combinators/v1_combinators/helpers/v1_helpers.h"
#include "frontend/combinators/basic_combinators/atom_parser.h"

#include <string>     // std::string, std::stoi

#define super NullParser

using namespace cs160::frontend;
using namespace std;

// )
ParseStatus<const AstNode> CloseParenParser::parse(std::string inputProgram, int startCharacter, std::string errorType) {
	int endCharacter = startCharacter;
	endCharacter += trim(inputProgram);
	std::string errorMessage = "Expecting close paranthesis";

	if (inputProgram.size() == 0) {
		return super::parse(inputProgram, endCharacter, errorMessage);
	}

	auto atomParser = AtomParser(')');
	auto result = atomParser.parse(inputProgram, startCharacter);
	if (atomParser.status) {
		auto result = ParseStatus<const AstNode>::success(startCharacter, 
			endCharacter + 1, inputProgram.erase(0,1), atom);
	}
	else {
		auto result = ParseStatus<const AstNode>::failure(startCharacter, result.remainingCharacters, errorMessage);
	}
	return result;
}

// (
ParseStatus<const AstNode> OpenParenParser::parse(std::string inputProgram, int startCharacter, std::string errorType) {
	int endCharacter = startCharacter;
	endCharacter += trim(inputProgram);

	std::string errorMessage = "Expecting open paranthesis";

	if (inputProgram.size() == 0) {
		return super::parse(inputProgram, endCharacter, errorMessage);
	}

	ParseStatus status;
	if (inputProgram[0] == '(') {
		status.status = true;
		status.remainingCharacters = inputProgram.erase(0, 1);
		status.parsedCharacters = '(';
		status.startCharacter = startCharacter;
		status.endCharacter = endCharacter + 1;
	} else {
		return super::parse(inputProgram, endCharacter, errorMessage);
	}
	return status;
}

// - 
ParseStatus<const AstNode> NegativeParser::parse(std::string inputProgram, int startCharacter, std::string errorType) {
	int endCharacter = startCharacter;
	endCharacter += trim(inputProgram);
	std::string errorMessage = "Expecting -";


	if (inputProgram.size() == 0) {
		return super::parse(inputProgram, endCharacter, errorMessage);
	}

	ParseStatus status;

	if (inputProgram[0] == '-') {
		status.status = true;
		status.remainingCharacters = inputProgram.erase(0, 1);
		status.parsedCharacters = '-';
		status.startCharacter = startCharacter;
		status.endCharacter = endCharacter + 1;
	} else {
		return super::parse(inputProgram, endCharacter, errorMessage);
	}
	return status;
}

// + - 
ParseStatus<const AstNode> AddSubOpParser::parse(std::string inputProgram, int startCharacter, std::string errorType) {
	int endCharacter = startCharacter;
	endCharacter += trim(inputProgram);
	std::string errorMessage = "Expecting + or -";

	if (inputProgram.size() == 0) {
		return super::parse(inputProgram, endCharacter, errorMessage);
	}
	ParseStatus status;

	if (inputProgram[0] == '+' || inputProgram[0] == '-') {
		status.status = true;
		status.parsedCharacters = inputProgram[0];
		status.remainingCharacters = inputProgram.erase(0, 1);
		status.startCharacter = startCharacter;
		status.endCharacter = endCharacter + 1;
	} else {
		return super::parse(inputProgram, endCharacter, errorMessage);
	}
	return status;
}

// * /
ParseStatus<const AstNode> MulDivOpParser::parse(std::string inputProgram, int startCharacter, std::string errorType) {
	int endCharacter = startCharacter;
	endCharacter += trim(inputProgram);
	std::string errorMessage = "Expecting * or /";

	if (inputProgram.size() == 0) {
		return super::parse(inputProgram, endCharacter, errorMessage);
	}

	ParseStatus status;

	if (inputProgram[0] == '*' || inputProgram[0] == '/') {
		status.status = true;
		status.parsedCharacters = inputProgram[0];
		status.remainingCharacters = inputProgram.erase(0, 1);
		status.startCharacter = startCharacter;
		status.endCharacter = endCharacter + 1;
	} else {
		return super::parse(inputProgram, endCharacter, errorMessage);
	}
	return status;
}

// ;
ParseStatus<const AstNode> SemiColonParser::parse(std::string inputProgram, int startCharacter, std::string errorType) {
	int endCharacter = startCharacter;
	endCharacter += trim(inputProgram);
	std::string errorMessage = "Missing semicolon";

	if (inputProgram.size() == 0) {
		return super::parse(inputProgram, endCharacter, errorMessage);
	}

	ParseStatus status;
	if ((inputProgram[0] == ';')){
		status.status = true;
		status.parsedCharacters = inputProgram[0];
		status.remainingCharacters = inputProgram.erase(0, 1);
		status.startCharacter = startCharacter;
		status.endCharacter = endCharacter + 1;
	} else {
		return super::parse(inputProgram, endCharacter, errorMessage);
	}
	return status;
}

