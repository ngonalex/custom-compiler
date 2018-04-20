#ifndef PARSER_PARSER_H_
#define PARSER_PARSER_H_

#include <vector>
#include "../tokenizer/tokenizer.h"

namespace cs160 {
namespace frontend {
  
  class Parser {
    public:
      Parser(std::vector<Token> program) {
        program_ = program;
      }
      
      // Subroutines
      Type Next();
      void Consume();
      void Error();
      void Expect(Token token);
      void Erecognizer();
      void E();
      void P();
      
    // Tokens will now be in reverse order!
    private:
      std::vector<Token> program_;
  };
  
  
} // namespace frontend
} // namespace cs160

#endif // PARSER_PARSER_H_