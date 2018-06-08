#include "frontend/combinators/basic_combinators/null.h"
#include "frontend/combinators/basic_combinators/parsestatus.h"
#include <map>
#include <utility>

using namespace cs160::frontend;

ParseStatus NullParser::fail(std::string inputProgram, int startCharacter, std::string errorMessage) {
  ParseStatus status;
  status.status = false;
  status.remainingCharacters = inputProgram;
  status.parsedCharacters = "";
  status.startCharacter = startCharacter;
  status.endCharacter = startCharacter;
  status.ast = nullptr;
  // status.second_ast = nullptr;
  if (errorMessage != "") {
    status.errorType = errorMessage;
  }
  //cache.insert(std::pair<int, ParseStatus>(startCharacter, std::move(status)));
  return status;
}

ParseStatus NullParser::do_parse(std::string inputProgram, int startCharacter) {
  return fail(inputProgram, startCharacter);
}

ParseStatus NullParser::parse(std::string inputProgram, int startCharacter, std::string errorMessage) {
  std::map<int, ParseStatus>::iterator iter;
  iter = cache.find(startCharacter);

  if(iter != cache.end()) {
    ParseStatus result = std::move(iter->second);
    return result;
  } else {
    return do_parse(inputProgram, startCharacter);
  }
}