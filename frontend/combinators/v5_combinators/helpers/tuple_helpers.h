#ifndef TUPLE_HELPER_H_
#define TUPLE_HELPER_H_

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/null.h"

namespace cs160 {
namespace frontend {

class LeftBracketOp : public NullParser {
 public:
  virtual ParseStatus do_parse(std::string inputProgram, int startCharacter);
};

class RightBracketOp : public NullParser {
 public:
  virtual ParseStatus do_parse(std::string inputProgram, int startCharacter);
};

}  // namespace frontend
}  // namespace cs160
#endif
