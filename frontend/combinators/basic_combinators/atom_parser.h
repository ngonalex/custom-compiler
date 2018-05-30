#ifndef ATOM_PARSER_H_
#define ATOM_PARSER_H_

#include "abstract_syntax/abstract_syntax.h"

#include <string>

using namespace cs160::abstract_syntax::frontend;

namespace cs160 {
namespace frontend {

class AtomParser {
 public:
  ParseStatus<const AstNode> parse(
            std::string intput,
  				  int start_character,
			      std::string error_type = "");

  explicit AtomParser(char char_to_parse) { 
    this->char_to_parse = char_to_parse;
  }

  char char_to_parse;
};

}  // namespace frontend
}  // namespace cs160

#endif  // ATOM_PARSER_H_
