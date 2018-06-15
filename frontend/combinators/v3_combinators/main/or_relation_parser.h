#ifndef OR_RELATION_H_
#define OR_RELATION_H_

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/null.h"

namespace cs160 {
namespace frontend {

class OrRelationParser : public NullParser {
 public:
  virtual ParseStatus do_parse(std::string inputProgram, int startCharacter);
  std::unique_ptr<const LogicalOrExpr> make_node(
    std::unique_ptr<const RelationalExpr> first_leaf,
    std::unique_ptr<const RelationalExpr> second_leaf);
};

}  // namespace frontend
}  // namespace cs160

#endif  // OR_RELATION_H_
