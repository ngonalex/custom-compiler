#ifndef FRONTEND_COMBINATORS_BASIC_COMBINATORS_ATOM_PARSER_H_
#define FRONTEND_COMBINATORS_BASIC_COMBINATORS_ATOM_PARSER_H_

#include <string>

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/null.h"
#include "frontend/combinators/basic_combinators/parsestatus.h"

#define super NullParser

using namespace cs160::abstract_syntax::frontend;

namespace cs160 {
namespace frontend {

class AtomParser : public NullParser {
 public:
  virtual ParseStatus do_parse(std::string inputProgram, int startCharacter);

  explicit AtomParser(char char_to_parse) {
    this->char_to_parse = char_to_parse;
  }

  char char_to_parse;
};

}  // namespace frontend
}  // namespace cs160

#endif  // FRONTEND_COMBINATORS_BASIC_COMBINATORS_ATOM_PARSER_H_
