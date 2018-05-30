#include "frontend/combinators/basic_combinators/parsestatus.h"

using namespace cs160::frontend;

bool ParseStatus::operator==(const ParseStatus &b) const {
  if (this->status) {
    return (this->status == b.status &&
	    this->remainingCharacters == b.remainingCharacters &&
	    this->parsedCharacters == b.parsedCharacters);
  } else {
    return (this->status == b.status);
  }
}

bool ParseStatus::operator!=(const ParseStatus &b) const {
  if (this->status) {
    return (this->status != b.status ||
	    this->remainingCharacters != b.remainingCharacters ||
	    this->parsedCharacters != b.parsedCharacters);
  } else {
    return (this->status != b.status);
  }
}

/*
bool AndCombParseStatus::operator==(const ParseStatus &b) const {
	if(this->status) {
		return (this->status == b.status &&
						this->remainingCharacters ==
b.remainingCharacters && this->parsedCharacters == b.parsedCharacters);
	}
	else {
		return (this->status == b.status);
	}
}

bool AndCombParseStatus::operator!=(const ParseStatus &b) const {
	if(this->status) {
		return (this->status != b.status ||
						this->remainingCharacters !=
b.remainingCharacters || this->parsedCharacters != b.parsedCharacters);
	}
	else {
		return (this->status == b.status);
	}
}
*/
