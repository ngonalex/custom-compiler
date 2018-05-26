#ifndef GREATERTHANOREQUAL_SYMBOLS_H_
#define GREATERTHANOREQUAL_SYMBOLS_H_

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/null.h"

namespace cs160 {
namespace frontend {

class GreaterThanOrEqualToParser : NullParser {
public:
	virtual ParseStatus parse(std::string inputProgram);
};

} // namespace frontend
} // namespace cs160

#endif // GREATERTHANOREQUAL_SYMBOLS_H_
