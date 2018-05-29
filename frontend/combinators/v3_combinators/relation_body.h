#ifndef RELATION_BODY_H_
#define RELATION_BODY_H_

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/null.h"

namespace cs160 {
namespace frontend {

class RelationBodyParser : NullParser {
 public:
   virtual ParseStatus parse(std::string inputProgram, std::string errorType = "");
   
   std::unique_ptr<const RelationalBinaryOperator> RelationBodyParser::make_node(
     std::unique_ptr<const ArithmeticExpr> first_ae, 
     std::string rop, std::unique_ptr<const ArithmeticExpr> second_ae);

};

} // namespace frontend
} // namespace cs160

#endif // RELATION_BODY_H_