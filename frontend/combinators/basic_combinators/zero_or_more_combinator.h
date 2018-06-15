#ifndef FRONTEND_COMBINATORS_BASIC_COMBINATORS_ZERO_OR_MORE_COMBINATOR_H_
#define FRONTEND_COMBINATORS_BASIC_COMBINATORS_ZERO_OR_MORE_COMBINATOR_H_

#include <string>

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/null.h"

namespace cs160 {
namespace frontend {

class ZeroOrMoreCombinator : public NullParser {
 public:
  NullParser *parser;
  virtual ParseStatus do_parse(std::string inputProgram, int startCharacter);
};

}  // namespace frontend
}  // namespace cs160

#endif  // FRONTEND_COMBINATORS_BASIC_COMBINATORS_ZERO_OR_MORE_COMBINATOR_H_
