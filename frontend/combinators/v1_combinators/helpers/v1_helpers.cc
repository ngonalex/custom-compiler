#include "frontend/combinators/v1_combinators/helpers/v1_helpers.h"
#include "frontend/combinators/basic_combinators/atom_parser.h"
#include "frontend/combinators/basic_combinators/or_combinator.h"

#include <string>     // std::string, std::stoi
#include <iostream>

#define super NullParser

using namespace cs160::frontend;
using namespace std;

// )
ParseStatus CloseParenParser::parse(std::string inputProgram, int startCharacter, std::string errorType) {
	int endCharacter = startCharacter;
	endCharacter += trim(inputProgram);
	std::string errorMessage = "Expecting close parenthesis";

	if (inputProgram.size() == 0) {
		return super::parse(inputProgram, endCharacter, errorMessage);
	}

	auto atomParser = AtomParser(')');
	auto result = atomParser.parse(inputProgram, endCharacter);
	return result;
}

// (
ParseStatus OpenParenParser::parse(std::string inputProgram, int startCharacter, std::string errorType) {
	int endCharacter = startCharacter;
	endCharacter += trim(inputProgram);

	std::string errorMessage = "Expecting open parenthesis";

	if (inputProgram.size() == 0) {
		return super::parse(inputProgram, endCharacter, errorMessage);
	}

	auto atomParser = AtomParser('(');
	auto result = atomParser.parse(inputProgram, endCharacter);
	return result;
}

// - 
ParseStatus NegativeParser::parse(std::string inputProgram, int startCharacter, std::string errorType) {
	int endCharacter = startCharacter;
	endCharacter += trim(inputProgram);
	std::string errorMessage = "Expecting -";


	if (inputProgram.size() == 0) {
		return super::parse(inputProgram, endCharacter, errorMessage);
	}

	auto atomParser = AtomParser('-');
	auto result = atomParser.parse(inputProgram, endCharacter);
	return result;
}

// + - 
ParseStatus AddSubOpParser::parse(std::string inputProgram, int startCharacter, std::string errorType) {
	int endCharacter = startCharacter;
	endCharacter += trim(inputProgram);
	std::string errorMessage = "Expecting + or -";

	if (inputProgram.size() == 0) {
		return super::parse(inputProgram, endCharacter, errorMessage);
	}

	auto plusParser = AtomParser('+');
	auto minusParser = AtomParser('-');
	OrCombinator plusOrMinus;
	plusOrMinus.firstParser = reinterpret_cast<NullParser *>(&plusParser);
	plusOrMinus.secondParser = reinterpret_cast<NullParser *>(&minusParser);

	auto result = plusOrMinus.parse(inputProgram, endCharacter);
	return result;
}

// * /
ParseStatus MulDivOpParser::parse(std::string inputProgram, int startCharacter, std::string errorType) {
	int endCharacter = startCharacter;
	endCharacter += trim(inputProgram);
	std::string errorMessage = "Expecting * or /";

	if (inputProgram.size() == 0) {
		return super::parse(inputProgram, endCharacter, errorMessage);
	}

	auto mulParser = AtomParser('*');
	auto divParser = AtomParser('/');
	
	OrCombinator mulOrDiv;
	mulOrDiv.firstParser = reinterpret_cast<NullParser *>(&mulParser);

	mulOrDiv.secondParser = reinterpret_cast<NullParser *>(&divParser);
	auto result = mulOrDiv.parse(inputProgram, endCharacter);
	return result;
}

// ;
ParseStatus SemiColonParser::parse(std::string inputProgram, int startCharacter, std::string errorType) {
	int endCharacter = startCharacter;
	endCharacter += trim(inputProgram);
	std::string errorMessage = "Expecting ;";

	if (inputProgram.size() == 0) {
		return super::parse(inputProgram, endCharacter, errorMessage);
	}

	auto atomParser = AtomParser(';');
	auto result = atomParser.parse(inputProgram, endCharacter);
	return result;
}

