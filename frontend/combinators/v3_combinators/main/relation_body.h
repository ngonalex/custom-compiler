#ifndef RELATION_BODY_H_
#define RELATION_BODY_H_

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/null.h"

namespace cs160 {
namespace frontend {

class RelationBodyParser : NullParser {
 public:
   virtual ParseStatus do_parse(std::string inputProgram, int startCharacter);
   
   std::unique_ptr<const RelationalBinaryOperator> make_node(
     std::unique_ptr<const ArithmeticExpr> first_ae, 
     std::string rop, std::unique_ptr<const ArithmeticExpr> second_ae);

  std::unique_ptr<const LogicalBinaryOperator> make_node(
     std::unique_ptr<const RelationalExpr> first_re, 
     std::string lop, std::unique_ptr<const RelationalExpr> second_re);
};

} // namespace frontend
} // namespace cs160

#endif // RELATION_BODY_H_