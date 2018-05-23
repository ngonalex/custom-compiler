#ifndef ADD_SUB_OP_H_
#define ADD_SUB_OP_H_

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/null.h"

namespace cs160 {
namespace frontend {

class AddSubOpParser : NullParser {
 public:
	virtual ParseStatus parse(std::string inputProgram);
};

} // namespace frontend
} // namespace cs160

#endif // ADD_SUB_OP_H_
