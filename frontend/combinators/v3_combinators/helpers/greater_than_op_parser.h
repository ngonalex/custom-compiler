#ifndef GREATERTHAN_SYMBOL_H_
#define GREATERTHAN_SYMBOL_H_

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/null.h"

namespace cs160 {
namespace frontend {

class GreaterThanParser : NullParser {
public:
	virtual ParseStatus parse(std::string inputProgram);
};

} // namespace frontend
} // namespace cs160

#endif // GREATERTHAN_SYMBOL_H_
