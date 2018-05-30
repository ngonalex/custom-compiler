#include "frontend/combinators/basic_combinators/atom_parser.h"

using namespace cs160::frontend;

ParseStatus<const AstNode> AtomParser::parse(std::string input, 
                              int start_character, 
                              std::string error_type) {

  // Failures
  if (input.size() == 0 || input[0] != char_to_parse) {
    return ParseStatus<const AstNode>::failure(start_character, 
                                  input, "Error parsing Atom\n");
  } 

  // Success
  std::string remaining_chars(input.begin()+1, input.end());
  std::string parsed_char(1, char_to_parse);
  return ParseStatus<const AstNode>::success(
    start_character, 
    start_character+1, 
    remaining_chars, 
    parsed_char);
}

// NUM and Operator
//
