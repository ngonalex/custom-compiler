#ifndef RELATION_BODY_H_
#define RELATION_BODY_H_

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/null.h"

namespace cs160 {
namespace frontend {

class RelationBodyParser : public NullParser {
 public:
  virtual ParseStatus do_parse(std::string inputProgram, int startCharacter);

  std::unique_ptr<const RelationalExpr> make_node(
      std::unique_ptr<const ArithmeticExpr> first_ae, std::string rop,
      std::unique_ptr<const ArithmeticExpr> second_ae);
};

}  // namespace frontend
}  // namespace cs160

#endif  // RELATION_BODY_H_
