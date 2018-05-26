#ifndef AE_H_
#define AE_H_

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/null.h"
#include "frontend/combinators/v1_combinators/add_sub_expr.h"

namespace cs160 {
namespace frontend {

class ArithExprParser : NullParser {
 public:
   virtual ParseStatus parse(std::string inputProgram, std::string errorType = "");
};

} // namespace frontend
} // namespace cs160

#endif // AE_H_
