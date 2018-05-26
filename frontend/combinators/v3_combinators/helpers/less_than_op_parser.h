#ifndef LESSTHAN_SYMBOL_H_
#define LESSTHAN_SYMBOL_H_

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/null.h"

namespace cs160 {
namespace frontend {

class LessThanParser : NullParser {
public:
	virtual ParseStatus parse(std::string inputProgram, std::string errorType = "");
};

} // namespace frontend
} // namespace cs160

#endif // LESSTHAN_SYMBOL_H_
