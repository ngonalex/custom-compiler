#ifndef FUNCTION_CALL_H_
#define FUNCTION_CALL_H_

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/null.h"

namespace cs160 {
namespace frontend {

class FunctionCallParser : public NullParser {
 public:
  virtual ParseStatus do_parse(std::string inputProgram, int startCharacter);
  /*std::unique_ptr<const FunctionDef> make_node(std::string functionName,
      std::vector<std::unique_ptr<const VariableExpr>> parameters,
              Statement::Block function_body,
              std::unique_ptr<const ArithmeticExpr> retval);*/
};

}  // namespace frontend
}  // namespace cs160

#endif  // FUNCTION_CALL_H_
