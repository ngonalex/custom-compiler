#ifndef LOOP_PARSER_H_
#define LOOP_PARSER_H_

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/null.h"

namespace cs160 {
namespace frontend {

class LoopParser : NullParser {
 public:
   virtual ParseStatus do_parse(std::string inputProgram, int startCharacter);
};

} // namespace frontend
} // namespace cs160

#endif // LOOP_PARSER_H_