#ifndef FRONTEND_COMBINATORS_V1_COMBINATORS_MUL_DIV_EXPR_H_
#define FRONTEND_COMBINATORS_V1_COMBINATORS_MUL_DIV_EXPR_H_

#include <string>

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/null.h"

namespace cs160 {
namespace frontend {

class MulDivExprParser : public NullParser {
 public:
  virtual ParseStatus do_parse(std::string inputProgram, int startCharacter);
  std::unique_ptr<const ArithmeticExpr> make_node(
      std::string op, std::unique_ptr<const ArithmeticExpr> first_leaf,
      std::unique_ptr<const ArithmeticExpr> second_leaf);
};

}  // namespace frontend
}  // namespace cs160

#endif  // FRONTEND_COMBINATORS_V1_COMBINATORS_MUL_DIV_EXPR_H_
