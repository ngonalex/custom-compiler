#ifndef PARSER_PARSER_H_
#define PARSER_PARSER_H_

#include <vector>
#include "../tokenizer/tokenizer.h"

namespace cs160 {
namespace frontend {
  // 3 + 5 -- sure
  // 3 * 5 + 8
  /*
        +
    *        8
  3   5  
  
    E -> E + T | E - T | T
    T -> T * F | T / F | F
    F -> ( E ) | Num
    
    ParseState parseInt(ParseState p, int current);
    ParseState matchChar(ParseState, char current);
    
    ParseState expect(ParseState p) {
      if(!p.isSuccess()) {
        throw 
      }
    }
      
  */
  
  enum State { ACCEPTING, REJECTED };
  // Can return an error state or an accepting state for the current branch
  // If it's an error state, then try a new branch, otherwise continue evaluation
  class ParseState {
    public:
      Token left;
      Token middle;
      Token right;
      State state;
  };
    
  class Parser {
    public:
      Parser(std::vector<Token> program);
      
      // location is the current vector location
      ParseState* parseInt(ParseState *p, int location);
      ParseState* parseOperator(ParseState *p, int location);
      ParseState* parseParen(ParseState *p, int location);
      int start();
      
    private:
      std::vector<ParseState> state_stack;
      std::vector<Token> program;
  };
  
  
} // namespace frontend
} // namespace cs160

#endif // PARSER_PARSER_H_