#ifndef TERM_EXPR_H_
#define TERM_EXPR_H_

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/null.h"
#include "frontend/combinators/v1_combinators/add_sub_op.h"
#include "frontend/combinators/v1_combinators/num_parser.h"
#include "frontend/combinators/v1_combinators/add_sub_expr.h"
#include "frontend/combinators/v1_combinators/open_paren.h"
#include "frontend/combinators/v1_combinators/close_paren.h"


class TermExpr : NullParser {
 public:
   virtual ParseStatus parse(std::string inputProgram);

};

#endif // TERM_EXPR_H
