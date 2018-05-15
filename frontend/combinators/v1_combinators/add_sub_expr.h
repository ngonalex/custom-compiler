#ifndef ADD_SUB_EXPR_H_
#define ADD_SUB_EXPR_H_

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/null.h"
#include "frontend/combinators/v1_combinators/add_sub_op.h"
#include "frontend/combinators/v1_combinators/num_parser.h"
#include "frontend/combinators/v1_combinators/mul_div_expr.h"

class AddSubExprParser : NullParser {
 public:
   virtual ParseStatus parse(std::string inputProgram); 
   std::unique_ptr<const AstNode> make_node(std::string op, 
     std::unique_ptr<const AstNode> first_leaf,
     std::unique_ptr<const AstNode> second_leaf);
};

#endif // ADD_SUB_EXPR_H_
