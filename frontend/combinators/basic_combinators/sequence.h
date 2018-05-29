#ifndef SEQUENCE_H
#define SEQUENCE_H

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/null.h"

namespace cs160 {
namespace frontend {
	
class Sequence : NullParser {
 public:
	virtual ParseStatus parse(std::string inputProgram);
	NullParser* firstParser;
	NullParser* secondParser;
};

}	// namespace frontend
}	// namespace cs160

#endif // SEQUENCE_H
