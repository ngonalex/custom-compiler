#ifndef STATEMENT_PARSER_H_
#define STATEMENT_PARSER_H_

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/null.h"

namespace cs160 {
namespace frontend {

class StatementParser : NullParser {
 public:
   virtual ParseStatus do_parse(std::string inputProgram, int startCharacter);
};

} // namespace frontend
} // namespace cs160

#endif // STATEMENT_PARSER_H_