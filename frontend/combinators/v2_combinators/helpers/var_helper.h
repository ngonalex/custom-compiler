#ifndef VAR_HELPER_H_
#define VAR_HELPER_H_

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/null.h"

namespace cs160 {
namespace frontend {

class VarKeywordParser : NullParser {
 public:
   virtual ParseStatus parse(std::string inputProgram);
};

class ColonParser : NullParser {
 public:
   virtual ParseStatus parse(std::string inputProgram);
};

} // namespace frontend
} // namespace cs160

#endif // VAR_HELPER_H_
