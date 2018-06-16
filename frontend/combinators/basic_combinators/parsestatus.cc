#include "frontend/combinators/basic_combinators/parsestatus.h"

namespace cs160 {
namespace frontend {

ParseStatus::ParseStatus() {}

ParseStatus::ParseStatus(ParseStatus &&other)
    : astNodes(std::move(other.astNodes)),
      ast(std::move(other.ast)),
      second_ast(std::move(other.second_ast)) {
  this->status = other.status;
  this->remainingCharacters = other.remainingCharacters;
  this->parsedCharacters = other.parsedCharacters;
  this->firstParsedCharacters = other.firstParsedCharacters;
  this->secondParsedCharacters = other.secondParsedCharacters;
  this->firstOrSecond = other.firstOrSecond;
  this->errorType = other.errorType;
  this->startCharacter = other.startCharacter;
  this->endCharacter = other.endCharacter;
  this->parsedCharactersArray.erase(std::begin(this->parsedCharactersArray),
                                    std::end(this->parsedCharactersArray));
  this->parsedCharactersArray.insert(std::end(this->parsedCharactersArray),
                                     std::begin(other.parsedCharactersArray),
                                     std::end(other.parsedCharactersArray));
}

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

}  // namespace frontend
}  // namespace cs160
