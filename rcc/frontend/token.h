#ifndef TOKEN_H_
#define TOKEN_H_

namespace cs160{
namespace token {
  enum NUM, PAREN, ARITH_OP;

  struct Token {
    char* token_val;
    std::pair<int, int> token_pos; //line number, character number
    enum type;
  };

}
}

#endif	// TOKEN_H_
