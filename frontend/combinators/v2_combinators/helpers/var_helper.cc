#include "frontend/combinators/v2_combinators/helpers/var_helper.h"
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
