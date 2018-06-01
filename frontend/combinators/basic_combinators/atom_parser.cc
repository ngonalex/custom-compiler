#include "frontend/combinators/basic_combinators/atom_parser.h"

using namespace cs160::frontend;

ParseStatus AtomParser::parse(std::string input, int start_character) {

  // Failures
  if (input.size() == 0 || input[0] != char_to_parse) {
    ParseStatus result; 
    result.startCharacter = start_character;
    result.remainingCharacters = input;
    result.errorType = "Error parsing Atom \n";
    return result;
  } 

  // Success
  std::string remaining_chars(input.begin()+1, input.end());
  std::string parsed_char(1, char_to_parse);

  ParseStatus result;
  result.status = true;
  result.startCharacter = start_character;
  result.endCharacter = start_character + 1;
  result.remainingCharacters = remaining_chars;
  result.parsedCharacters = parsed_char;

  return result;
}

// NUM and Operator
//
