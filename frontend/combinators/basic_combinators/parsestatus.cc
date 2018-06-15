#include "frontend/combinators/basic_combinators/parsestatus.h"

using namespace cs160::frontend;
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
/*
ParseStatus& ParseStatus::operator=(ParseStatus&& other) {
    parsedCharactersArray.erase(std::begin(parsedCharactersArray),
std::end(parsedCharactersArray));
    parsedCharactersArray.insert(std::end(parsedCharactersArray),
std::begin(other.parsedCharactersArray), std::end(other.parsedCharactersArray));
  ast = std::move(other.ast);
  astNodes = std::move(other.astNodes);
  second_ast = std::move(other.second_ast);
  return *this;
}*/

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
