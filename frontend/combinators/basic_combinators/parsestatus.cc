#include "frontend/combinators/basic_combinators/parsestatus.h"

using namespace cs160::frontend;

bool ParseStatus::operator==(const ParseStatus &b) const {
	return (this->status == b.status && 
					this->remainingCharacters == b.remainingCharacters && 
					this->parsedCharacters == b.parsedCharacters);
}

bool ParseStatus::operator!=(const ParseStatus &b) const {
	return (this->status != b.status || this->remainingCharacters != b.remainingCharacters);
}
