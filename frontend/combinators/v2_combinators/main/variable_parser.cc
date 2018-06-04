#include "frontend/combinators/v2_combinators/main/variable_parser.h"
#include "frontend/combinators/v2_combinators/main/word_parser.h"
#include "frontend/combinators/v2_combinators/helpers/var_helper.h"
#include "frontend/combinators/v1_combinators/term_expr.h"
#include "frontend/combinators/v1_combinators/helpers/v1_helpers.h"


#include <string> // std::string, std::stoi

#define super NullParser
/*
using namespace cs160::frontend;
using namespace std;

ParseStatus VariableParser::parse(std::string inputProgram, int startCharacter, std::string errorType) {
  int endCharacter = startCharacter;
  endCharacter += trim(inputProgram);

  if (inputProgram.size() == 0) {
    return super::parse(inputProgram, endCharacter);
  }

  VarKeywordParser varParser;
  WordParser wordParser;
  ColonParser colonParser;
  TypeParser typeParser;
  SemiColonParser semiColonP;

  // Parse the first character
  ParseStatus result = varParser.parse(inputProgram);

  if (result.status) {
    ParseStatus wordResult = wordParser.parse(result.remainingCharacters);
    if (wordResult.status) {
      result.parsedCharacters += (" " + wordResult.parsedCharacters);
      result.remainingCharacters = wordResult.remainingCharacters;
      ParseStatus colonStatus = colonParser.parse(result.remainingCharacters);

      if (colonStatus.status) {
        result.parsedCharacters += (" " + colonStatus.parsedCharacters);
        result.remainingCharacters = colonStatus.remainingCharacters;
        ParseStatus typeStatus =
            typeParser.parse(result.remainingCharacters);
        if (typeStatus.status) {
          result.parsedCharacters += (" " + typeStatus.parsedCharacters);
          result.remainingCharacters = typeStatus.remainingCharacters;

          ParseStatus semiColonResult = semiColonP.parse(result.remainingCharacters);
          result.status = semiColonResult.status;    
          if(semiColonResult.status) {
            result.remainingCharacters = semiColonResult.remainingCharacters;
            result.parsedCharacters += semiColonResult.parsedCharacters;
            result.ast = std::move(make_unique<const VariableExpr>(wordResult.parsedCharacters));
          }
          else {
            result.errorType = semiColonResult.errorType; 
          }
				}
				else {
					result.status = typeStatus.status;
					result.errorType = typeStatus.errorType; 
				}
      }
			else {
				result.status = colonStatus.status;
				result.errorType = colonStatus.errorType;
			}
    }
		else {
			result.status = wordResult.status;
			result.errorType = wordResult.errorType;
		}
  }

  return result;
}*/
