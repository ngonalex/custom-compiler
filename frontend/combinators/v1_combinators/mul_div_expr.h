#ifndef MUL_DIV_EXPR_H_
#define MUL_DIV_EXPR_H_

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/null.h"

namespace cs160 {
namespace frontend {

class MulDivExprParser : NullParser {
 public:
  virtual ParseStatus parse(std::string inputProgram, int startCharacter,
                            std::string errorType = "");
  std::unique_ptr<const ArithmeticExpr> make_node(
      std::string op, std::unique_ptr<const ArithmeticExpr> first_leaf,
      std::unique_ptr<const ArithmeticExpr> second_leaf);
};

}  // namespace frontend
}  // namespace cs160

#endif  // MUL_DIV_EXPR_H_
