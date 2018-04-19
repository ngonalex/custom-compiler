#include <iostream>
#include "tokenizer.h"

using namespace cs160::frontend;

int main() {
  Tokenizer lexer("(3+3) * 4 / 2 - 1");
  std::vector<Token> tokens;
  tokens.push_back(Token(OPEN_PAREN));
  tokens.push_back(Token(NUM, 3));
  tokens.push_back(Token(ADD_OP));
  tokens.push_back(Token(NUM, 3));
  tokens.push_back(Token(CLOSE_PAREN));
  tokens.push_back(Token(MUL_OP));
  tokens.push_back(Token(NUM, 4));
  tokens.push_back(Token(DIV_OP));
  tokens.push_back(Token(NUM, 2));
  tokens.push_back(Token(SUB_OP));
  tokens.push_back(Token(NUM, 1));

  int i = 0;
    int count = 0;
  for(Token a : lexer.tokens()){
      if(a == tokens[i])
          count++;
//      std::cout << "Type: " << a.type();
//      if(a.type() == NUM)
//        std::cout << " Value: " << a.val();
      std::cout << std::endl;
      i++;
  }
    std::cout << count << std::endl;
  return 0;
}
