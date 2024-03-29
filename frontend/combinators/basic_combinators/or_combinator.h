#ifndef OR_COMBINATOR_H_
#define OR_COMBINATOR_H_

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/null.h"

namespace cs160 {
namespace frontend {

class OrCombinator : public NullParser {
 public:
  NullParser *firstParser;
  NullParser *secondParser;

  virtual ParseStatus do_parse(std::string inputProgram, int startCharacter);
};

}  // namespace frontend
}  // namespace cs160

#endif  // OR_COMBINATOR_H_
