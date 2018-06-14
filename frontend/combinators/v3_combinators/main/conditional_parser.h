#ifndef CONDITIONAL_PARSER_H_
#define CONDITIONAL_PARSER_H_

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/null.h"

namespace cs160 {
namespace frontend {

class ConditionalParser : NullParser {
 public:
   virtual ParseStatus do_parse(std::string inputProgram, int startCharacter);
};

} // namespace frontend
} // namespace cs160

#endif // CONDITIONAL_PARSER_H_