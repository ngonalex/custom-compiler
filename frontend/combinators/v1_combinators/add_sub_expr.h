#ifndef ADD_SUB_EXPR_H_
#define ADD_SUB_EXPR_H_

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/null.h"
#include "frontend/combinators/v1_combinators/add_sub_op.h"
#include "frontend/combinators/v1_combinators/mul_div_expr.h"
#include "frontend/combinators/v1_combinators/num_parser.h"

namespace cs160 {
namespace frontend {

class AddSubExprParser : NullParser {
 public:
  virtual ParseStatus parse(std::string inputProgram,
  				int startCharacter,
			    std::string errorType = "");
  std::unique_ptr<const ArithmeticExpr> make_node(
      std::string op, std::unique_ptr<const ArithmeticExpr> first_leaf,
      std::unique_ptr<const ArithmeticExpr> second_leaf);
};

}  // namespace frontend
}  // namespace cs160

#endif  // ADD_SUB_EXPR_H_
