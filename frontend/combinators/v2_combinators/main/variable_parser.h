#ifndef FRONTEND_COMBINATORS_V2_COMBINATORS_MAIN_VARIABLE_PARSER_H_
#define FRONTEND_COMBINATORS_V2_COMBINATORS_MAIN_VARIABLE_PARSER_H_

#include <string>

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/null.h"

namespace cs160 {
namespace frontend {

class VariableParser : public NullParser {
 public:
  virtual ParseStatus do_parse(std::string inputProgram, int startCharacter);
};

}  // namespace frontend
}  // namespace cs160

#endif  // FRONTEND_COMBINATORS_V2_COMBINATORS_MAIN_VARIABLE_PARSER_H_
