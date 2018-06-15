#ifndef FRONTEND_COMBINATORS_V1_COMBINATORS_SINGLE_CHAR_H_
#define FRONTEND_COMBINATORS_V1_COMBINATORS_SINGLE_CHAR_H_

#include <string>

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/null.h"

namespace cs160 {
namespace frontend {

class SingleCharParser : public NullParser {
 public:
  virtual ParseStatus do_parse(std::string inputProgram, int startCharacter);
};

class SingleVarCharParser : public NullParser {
 public:
  virtual ParseStatus do_parse(std::string inputProgram, int startCharacter);
};

}  // namespace frontend
}  // namespace cs160

#endif  // FRONTEND_COMBINATORS_V1_COMBINATORS_SINGLE_CHAR_H_
