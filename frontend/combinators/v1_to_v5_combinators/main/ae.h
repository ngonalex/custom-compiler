#ifndef AE_H_
#define AE_H_

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/null.h"
#include "frontend/combinators/v1_to_v5_combinators/main/add_sub_expr.h"

namespace cs160 {
namespace frontend {

class ArithExprParser : public NullParser {
 public:
  virtual ParseStatus do_parse(std::string inputProgram, int startCharacter);
};

}  // namespace frontend
}  // namespace cs160

#endif  // AE_H_
