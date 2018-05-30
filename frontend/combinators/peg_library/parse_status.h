#ifndef PARSESTATUS_H_
#define PARSESTATUS_H_

#include "utility/memory.h"

#include <vector>
#include <string>

using namespace cs160::abstract_syntax::frontend;

namespace cs160 {
namespace frontend {

class ParseStatus { // Super class
 public:
	bool status;
	std::string remainingCharacters;
	std::string parsedCharacters;
};

} // namespace frontend
} // namespace cs160

#endif // PARSESTATUS_H_
