#ifndef SEMICOLON_PARSER_H_
#define SEMICOLON_PARSER_H_

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/null.h"

namespace cs160 {
namespace frontend {

class SemiColonParser : NullParser {
 public:
  virtual ParseStatus parse(std::string inputProgram,
			    std::string errorType = "");
};

}  // namespace frontend
}  // namespace cs160

#endif  // SEMICOLON_PARSER_H_
