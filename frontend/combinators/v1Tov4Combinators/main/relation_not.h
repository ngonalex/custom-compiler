#ifndef NOT_RELATION_H_
#define NOT_RELATION_H_

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/null.h"

namespace cs160 {
namespace frontend {

class NotRelationParser : public NullParser {
 public:
  virtual ParseStatus do_parse(std::string inputProgram, int startCharacter);
  std::unique_ptr<const LogicalNotExpr> make_node(
      std::unique_ptr<const RelationalExpr> first_ae);
};

}  // namespace frontend
}  // namespace cs160

#endif  // NOT_RELATION_H_
