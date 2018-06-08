// Relation -> Expr > Expr | Expr < Expr | Expr >= Expr | Expr <= Expr | Expr == Expr | Logic
// Logic -> Relation or Relation | Relation and Relation | not Relation

#ifndef RELATION_PARSER_H_
#define RELATION_PARSER_H_

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/null.h"

namespace cs160 {
namespace frontend {

class RelationParser : NullParser {
 public:
   virtual ParseStatus parse(std::string inputProgram, int startCharacter);
};

} // namespace frontend
} // namespace cs160

#endif // RELATION_PARSER_H_
