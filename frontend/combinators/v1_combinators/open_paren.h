#ifndef OPEN_PAREN_H_
#define OPEN_PAREN_H_

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/null.h"

namespace cs160 {
namespace frontend {

class OpenParenParser : NullParser {
 public:
   virtual ParseStatus parse(std::string inputProgram); 
};

} // namespace frontend
} // namespace cs160

#endif // OPEN_PAREN_H