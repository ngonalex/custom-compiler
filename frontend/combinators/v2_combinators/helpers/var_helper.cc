#include "frontend/combinators/v2_combinators/helpers/var_helper.h"
#include "frontend/combinators/basic_combinators/one_or_more_combinator.h"
#include "frontend/combinators/v1_combinators/single_char.h"
#include "frontend/combinators/v2_combinators/main/word_parser.h"
#include "frontend/combinators/v1_combinators/num_parser.h"
#include "frontend/combinators/v1_combinators/helpers/v1_helpers.h"
#include "frontend/combinators/basic_combinators/and_combinator.h"
#include "frontend/combinators/basic_combinators/atom_parser.h"


#include <string>     // std::string, std::stoi

#define super NullParser

using namespace cs160::frontend;
using namespace std;

ParseStatus VarKeywordParser::parse(std::string inputProgram, int startCharacter, std::string errorType) {
  int endCharacter = startCharacter;
  endCharacter += trim(inputProgram);

  std::string errorMessage = "Start variable declaration with var";

	if (inputProgram.size() == 0) {
		return super::parse(inputProgram, endCharacter, errorMessage);
	}

  auto vParser = AtomParser('v');
  auto aParser = AtomParser('a');
  auto rParser = AtomParser('r'); 
  AndCombinator andOne;
  andOne.firstParser = reinterpret_cast<NullParser *>(&vParser);
  andOne.secondParser = reinterpret_cast<NullParser *>(&aParser);
  AndCombinator andTwo;
  andTwo.firstParser = reinterpret_cast<NullParser *>(&andOne);
  andTwo.secondParser = reinterpret_cast<NullParser *>(&rParser);

  ParseStatus result = andTwo.parse(inputProgram, endCharacter);
  result.errorType = errorMessage;
  return result;
}


ParseStatus ColonParser::parse(std::string inputProgram, int startCharacter, std::string errorType) {
  int endCharacter = startCharacter;
  endCharacter += trim(inputProgram);

  std::string errorMessage = "Missing colon";

	if (inputProgram.size() == 0) {
		return super::parse(inputProgram, endCharacter, errorMessage);
	}

  auto vParser = AtomParser(':');
  ParseStatus result = vParser.parse(inputProgram, endCharacter);
  result.errorType = errorMessage;

  return result;
}


ParseStatus TypeParser::parse(std::string inputProgram, int startCharacter, std::string errorType) {
  int endCharacter = startCharacter;
  endCharacter += trim(inputProgram);

  std::string errorMessage = "Incorrect type in variable declaration";
	if (inputProgram.size() == 0) {
		return super::parse(inputProgram, endCharacter, errorMessage);
	}

  OneOrMoreCombinator oneOrMore;
  SingleCharParser charParser;

	oneOrMore.parser = reinterpret_cast<NullParser *>(&charParser);
  ParseStatus result = oneOrMore.parse(inputProgram, endCharacter);

  if(!result.status) {
    result.errorType = errorMessage;
  }

  return result;
}

ParseStatus EqualSignParser::parse(std::string inputProgram, int startCharacter, std::string errorType) {
   int endCharacter = startCharacter;
  endCharacter += trim(inputProgram);

  std::string errorMessage = "Missing equal sign";

  if (inputProgram.size() == 0) {
    return super::parse(inputProgram, endCharacter, errorMessage);
  }

  auto vParser = AtomParser('=');
  ParseStatus result = vParser.parse(inputProgram, endCharacter);
  result.errorType = errorMessage;

  return result;

}

/*
ParseStatus HelperVariableParser::parse(std::string inputProgram, int startCharacter, std::string errorType) {
  int endCharacter = startCharacter;
  endCharacter += trim(inputProgram);

  if (inputProgram.size() == 0) {
    return super::parse(inputProgram, endCharacter);
  }

  VarKeywordParser varParser;
  WordParser wordParser;
  ColonParser colonParser;
  TypeParser typeParser;
  SemiColonParser semiColonP;

  // Parse the first character
  ParseStatus result = varParser.parse(inputProgram);

  if (result.status) {
    ParseStatus wordResult = wordParser.parse(result.remainingCharacters);
    if (wordResult.status) {
      result.parsedCharacters += (" " + wordResult.parsedCharacters);
      result.remainingCharacters = wordResult.remainingCharacters;
      ParseStatus colonStatus = colonParser.parse(result.remainingCharacters);

      if (colonStatus.status) {
        result.parsedCharacters += (" " + colonStatus.parsedCharacters);
        result.remainingCharacters = colonStatus.remainingCharacters;
        ParseStatus typeStatus =
            typeParser.parse(result.remainingCharacters);
        if (typeStatus.status) {
          result.parsedCharacters += (" " + typeStatus.parsedCharacters);
          result.remainingCharacters = typeStatus.remainingCharacters;
          result.ast = std::move(make_unique<const VariableExpr>(wordResult.parsedCharacters));

        }
        else {
          result.status = typeStatus.status;
          result.errorType = typeStatus.errorType; 
        }
      }
      else {
        result.status = colonStatus.status;
        result.errorType = colonStatus.errorType;
      }
    }
    else {
      result.status = wordResult.status;
      result.errorType = wordResult.errorType;
    }
  }

  return result;
}*/
