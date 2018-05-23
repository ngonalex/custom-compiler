#ifndef CLOSE_PAREN_H_
#define CLOSE_PAREN_H_

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/null.h"

namespace cs160 {
namespace frontend {

class CloseParenParser : NullParser {
 public:
  virtual ParseStatus parse(std::string inputProgram);
};

}  // namespace frontend
}  // namespace cs160

#endif  // CLOSE_PAREN_H
