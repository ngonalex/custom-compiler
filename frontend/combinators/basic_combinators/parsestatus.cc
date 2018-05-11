#include "frontend/combinators/basic_combinators/parsestatus.h"

bool ParseStatus::operator==(const ParseStatus &b) const {
	return (this->status == b.status && this->remainingCharacters == b.remainingCharacters);
}

bool ParseStatus::operator!=(const ParseStatus &b) const {
	return (this->status != b.status || this->remainingCharacters != b.remainingCharacters);
}
