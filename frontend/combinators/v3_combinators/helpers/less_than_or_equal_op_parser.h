#ifndef LESSTHANOREQUAL_SYMBOLS_H_
#define LESSTHANOREQUAL_SYMBOLS_H_

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/null.h"

namespace cs160 {
namespace frontend {

class LessThanOrEqualToParser : NullParser {
public:
	virtual ParseStatus parse(std::string inputProgram);
};

} // namespace frontend
} // namespace cs160

#endif // LESSTHANOREQUAL_SYMBOLS_H_
