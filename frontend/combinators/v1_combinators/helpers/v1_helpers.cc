#include "frontend/combinators/v1_combinators/helpers/v1_helpers.h"
#include "frontend/combinators/basic_combinators/atom_parser.h"
#include "frontend/combinators/basic_combinators/or_combinator.h"

#include <iostream>
#include <string>  // std::string, std::stoi

#define super NullParser

using namespace cs160::frontend;
using namespace std;

// )
ParseStatus CloseParenParser::parse(std::string inputProgram, int startCharacter) {
	int endCharacter = startCharacter;
	endCharacter += trim(inputProgram);
	// Check in the cache if the character cuont parsestatus already exists, return that parsestatus if it does
	std::string errorMessage = "Expecting close parenthesis";

	if (inputProgram.size() == 0) {
		return super::fail(inputProgram, endCharacter, errorMessage);
	}

	auto atomParser = AtomParser(')');
	auto result = atomParser.parse(inputProgram, endCharacter);

	// append to cache startCharacter and ParseStatus
	return result;
}

// (
ParseStatus OpenParenParser::parse(std::string inputProgram, int startCharacter) {
	int endCharacter = startCharacter;
	endCharacter += trim(inputProgram);

  std::string errorMessage = "Expecting open parenthesis";

	if (inputProgram.size() == 0) {
		return super::fail(inputProgram, endCharacter, errorMessage);
	}

  auto atomParser = AtomParser('(');
  auto result = atomParser.parse(inputProgram, endCharacter);
  return result;
}

// - 
ParseStatus NegativeParser::parse(std::string inputProgram, int startCharacter) {
	int endCharacter = startCharacter;
	endCharacter += trim(inputProgram);
	std::string errorMessage = "Expecting -";

  if (inputProgram.size() == 0) {
    return super::fail(inputProgram, endCharacter, errorMessage);
  }

  auto atomParser = AtomParser('-');
  auto result = atomParser.parse(inputProgram, endCharacter);
  return result;
}

// + - 
ParseStatus AddSubOpParser::parse(std::string inputProgram, int startCharacter) {
	int endCharacter = startCharacter;
	endCharacter += trim(inputProgram);
	std::string errorMessage = "Expecting + or -";

	if (inputProgram.size() == 0) {
		return super::fail(inputProgram, endCharacter, errorMessage);
	}

  auto addParser = AtomParser('+');
  auto subParser = AtomParser('-');

  OrCombinator addOrSub;
  addOrSub.firstParser = reinterpret_cast<NullParser *>(&addParser);
  addOrSub.secondParser = reinterpret_cast<NullParser *>(&subParser);

  auto result = addOrSub.parse(inputProgram, endCharacter);
	return result;
}

// * /
ParseStatus MulDivOpParser::parse(std::string inputProgram, int startCharacter) {
	int endCharacter = startCharacter;
	endCharacter += trim(inputProgram);
	std::string errorMessage = "Expecting * or /";

	if (inputProgram.size() == 0) {
		return super::fail(inputProgram, endCharacter, errorMessage);
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
ParseStatus SemiColonParser::parse(std::string inputProgram, int startCharacter) {
	int endCharacter = startCharacter;
	endCharacter += trim(inputProgram);
	std::string errorMessage = "Expecting ;";

	if (inputProgram.size() == 0) {
		return super::fail(inputProgram, endCharacter, errorMessage);
	}

	auto atomParser = AtomParser(';');
	auto result = atomParser.parse(inputProgram, endCharacter);
	return result;
}
