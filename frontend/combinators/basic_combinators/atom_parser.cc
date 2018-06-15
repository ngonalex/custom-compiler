#include "frontend/combinators/basic_combinators/atom_parser.h"
#include <iostream>
using namespace cs160::frontend;

ParseStatus AtomParser::do_parse(std::string inputProgram, int startCharacter) {
  std::string errorMessage = "Expecting character: ";
  errorMessage += char_to_parse;

  ParseStatus result;
  // Failures
  if (inputProgram.size() == 0 || inputProgram[0] != char_to_parse) {
    result.status = false;
    result.startCharacter = startCharacter;
    result.remainingCharacters = inputProgram;
    result.endCharacter = startCharacter;
    result.errorType = errorMessage;
    return result;
  }

  // Success
  std::string remaining_chars(inputProgram.begin() + 1, inputProgram.end());
  std::string parsed_char(1, char_to_parse);

  result.status = true;
  result.startCharacter = startCharacter;
  result.endCharacter = startCharacter + 1;
  result.remainingCharacters = remaining_chars;
  result.parsedCharacters = parsed_char;

  return result;
}

// NUM and Operator
//
