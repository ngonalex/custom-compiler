#include "frontend/combinators/v2_combinators/helpers/var_helper.h"
#include "frontend/combinators/basic_combinators/one_or_more_combinator.h"
#include "frontend/combinators/v1_combinators/single_char.h"

#include <string>     // std::string, std::stoi

#define super NullParser

using namespace cs160::frontend;

ParseStatus VarKeywordParser::parse(std::string inputProgram) {
	if (inputProgram.size() == 0) {
		return super::parse(inputProgram);
	}

  trim(inputProgram);
  ParseStatus result;

  if(inputProgram.substr(0,3) == "var") {
    result.status = true;
    result.parsedCharacters = "var";
    result.remainingCharacters = inputProgram.erase(0,3);
  }
  else {
    result.status = false;
    result.errorType = "Start variable declaration with var";
  }
  return result;
}


ParseStatus ColonParser::parse(std::string inputProgram) {
	if (inputProgram.size() == 0) {
		return super::parse(inputProgram);
	}

  trim(inputProgram);
  ParseStatus result;

  if(inputProgram.substr(0,1) == ":") {
    result.status = true;
    result.parsedCharacters = ":";
    result.remainingCharacters = inputProgram.erase(0,1);
  }
  else {
    result.status = false;
    result.errorType = "Missing colon";
  }
  return result;
}


ParseStatus TypeParser::parse(std::string inputProgram) {
	if (inputProgram.size() == 0) {
		return super::parse(inputProgram);
	}

  trim(inputProgram);
  OneOrMoreCombinator oneOrMore;
  SingleCharParser charParser;

	oneOrMore.parser = reinterpret_cast<NullParser *>(&charParser);
  ParseStatus result = oneOrMore.parse(inputProgram);

  if(!result.status) {
    result.errorType = "Check type in variable declaration";
  }

  return result;
}

ParseStatus EqualSignParser::parse(std::string inputProgram) {
	if (inputProgram.size() == 0) {
		return super::parse(inputProgram);
	}

  trim(inputProgram);
  ParseStatus result;

  if(inputProgram.substr(0,1) == "=") {
    result.status = true;
    result.parsedCharacters = "=";
    result.remainingCharacters = inputProgram.erase(0,1);
  }
  else {
    result.status = false;
    result.errorType = "Missing equal sign in variable assignment";
  }
  return result;
}
