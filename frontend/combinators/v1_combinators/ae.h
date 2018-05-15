#ifndef AE_H_
#define AE_H_

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/null.h"

namespace cs160 {
namespace frontend {

class ArithExprParser : NullParser {
 public:
   virtual ParseStatus parse(std::string inputProgram);
};

} // namespace frontend
} // namespace cs160

#endif // AE_H_
