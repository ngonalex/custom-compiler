#ifndef AND_COMBINATOR_H_
#define AND_COMBINATOR_H_

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/null.h"

namespace cs160 {
namespace frontend {

class AndCombinator : public NullParser {
 public:
  virtual ParseStatus do_parse(std::string inputProgram, int startCharacter);
  NullParser* firstParser;
  NullParser* secondParser;

  bool noBackwardsCompat =
      false;  // XXX: Toggle bug fix required for v4 (but breaks v1 if enabled)
};

}  // namespace frontend
}  // namespace cs160

#endif  // AND_COMBINATOR_H_
