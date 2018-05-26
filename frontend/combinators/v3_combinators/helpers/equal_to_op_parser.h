#ifndef EQUAL_SYMBOL_H_
#define EQUAL_SYMBOL_H_

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/null.h"

namespace cs160 {
namespace frontend {

class EqualToParser : NullParser {
public:
	virtual ParseStatus parse(std::string inputProgram);
};

} // namespace frontend
} // namespace cs160

#endif // EQUAL_SYMBOL_H_
