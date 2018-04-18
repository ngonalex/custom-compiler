#ifndef PARSER_PARSER_H_
#define PARSER_PARSER_H_

#include <vector>
#include "../tokenizer/tokenizer.h"

namespace cs160 {
namespace frontend {
  // 3 + 5 -- sure
  // 3 * 5 + 8
  // /0 = 
  /*
        +
    *        8
  3   5  
  
    E -> E + T | E - T | T
    T -> T * F | T / F | F
    F -> ( E ) | Num
    
    ParseResult parseInt(ParseResult p, int current);
    ParseResult matchChar(ParseResult, char current);
    
    ParseResult expect(ParseResult p) {
      if(!p.isSuccess()) {
        throw 
      }
    }
      
  */
  
  class ParseResult {
    public:
      Token left;
      Token op;
      Token right;
  };
    
  class Parser {
    public:
      Parser(std::vector<Token> program);
      ParseResult* parseInt(ParseResult *p, int location);
      ParseResult* parseOperator(ParseResult *p, int location);
      int start();
    
    private:
      ParseResult *result;
      std::vector<Token> program;
  };
  
  
} // namespace frontend
} // namespace cs160

#endif // PARSER_PARSER_H_