#ifndef WORD_PARSER_H_
#define WORD_PARSER_H_

#include "frontend/combinators/basic_combinators/null.h"
#include "abstract_syntax/abstract_syntax.h"

namespace cs160 {
namespace frontend {

class WordParser : NullParser {
 public:
   virtual ParseStatus<const VariableExpr> parse(std::string inputProgram, std::string errorType = "");
};

} // namespace frontend
} // namespace cs160

#endif // WORD_PARSER_H_
