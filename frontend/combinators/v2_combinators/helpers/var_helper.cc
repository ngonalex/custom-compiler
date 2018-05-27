#include "frontend/combinators/v2_combinators/helpers/var_helper.h"
#include "frontend/combinators/basic_combinators/one_or_more_combinator.h"
#include "frontend/combinators/v1_combinators/single_char.h"
#include "frontend/combinators/v2_combinators/main/word_parser.h"
#include "frontend/combinators/v1_combinators/semicolon_parser.h"

#include <string>     // std::string, std::stoi

#define super NullParser

using namespace cs160::frontend;

ParseStatus VarKeywordParser::parse(std::string inputProgram, std::string errorType) {
	trim(inputProgram);

  std::string errorMessage = "Start variable declaration with var";

	if (inputProgram.size() == 0) {
		return super::parse(inputProgram, errorMessage);
	}

  ParseStatus result;

  if(inputProgram.substr(0,3) == "var") {
    result.status = true;
    result.parsedCharacters = "var";
    result.remainingCharacters = inputProgram.erase(0,3);
  }
  else {
    result.status = false;
    result.errorType = errorMessage;
  }
  return result;
}


ParseStatus ColonParser::parse(std::string inputProgram, std::string errorType) {
	trim(inputProgram);

  std::string errorMessage = "Missing colon";

	if (inputProgram.size() == 0) {
		return super::parse(inputProgram, errorMessage);
	}

  ParseStatus result;

  if(inputProgram.substr(0,1) == ":") {
    result.status = true;
    result.parsedCharacters = ":";
    result.remainingCharacters = inputProgram.erase(0,1);
  }
  else {
    result.status = false;
    result.errorType = errorMessage;
  }
  return result;
}


ParseStatus TypeParser::parse(std::string inputProgram, std::string errorType) {
	trim(inputProgram);

  std::string errorMessage = "Incorrect type in variable declaration";
	if (inputProgram.size() == 0) {
		return super::parse(inputProgram, errorMessage);
	}

  OneOrMoreCombinator oneOrMore;
  SingleCharParser charParser;

	oneOrMore.parser = reinterpret_cast<NullParser *>(&charParser);
  ParseStatus result = oneOrMore.parse(inputProgram);

  if(!result.status) {
    result.errorType = errorMessage;
  }

  return result;
}

ParseStatus EqualSignParser::parse(std::string inputProgram, std::string errorType) {
	trim(inputProgram);
  std::string errorMessage = "Missing equal sign";
	if (inputProgram.size() == 0) {
		return super::parse(inputProgram, errorMessage);
	}

  ParseStatus result;

  if(inputProgram.substr(0,1) == "=") {
    result.status = true;
    result.parsedCharacters = "=";
    result.remainingCharacters = inputProgram.erase(0,1);
  }
  else {
    result.status = false;
    result.errorType = errorMessage;
  }
  return result;
}

ParseStatus HelperVariableParser::parse(std::string inputProgram, std::string errorType) {
  trim(inputProgram);

  if (inputProgram.size() == 0) {
    return super::parse(inputProgram);
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
}
