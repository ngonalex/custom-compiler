#ifndef AND_RELATION_H_
#define AND_RELATION_H_

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/null.h"

namespace cs160 {
namespace frontend {

class AndRelationParser : public NullParser {
 public:
  virtual ParseStatus do_parse(std::string inputProgram, int startCharacter);
  std::unique_ptr<const LogicalAndExpr> make_node(
      std::unique_ptr<const RelationalExpr> first_leaf,
      std::unique_ptr<const RelationalExpr> second_leaf);
};

}  // namespace frontend
}  // namespace cs160

#endif  // AND_RELATION_H_
