#ifndef TOKEN_H_
#define TOKEN_H_

namespace cs160{
namespace tokenizer {
  enum NUM, PAREN, ADDSUB_OP, MULDIV_OP;

  struct Token {
    std::string
    char* token_val;
    std::pair<int, int> token_pos; //line number, character number
    enum type;
  };

// This is a class that should own a member of type Item.
class Tokenizer {
 public:
  explicit Tokenizer(std::string program);

 private:
  std::string program;
  int pos;
  int line;

 vector<Token> Tokenize();

 bool HelperIsNum();
 bool Helper_is_paren();
 bool Helper_is_addsub_op();
 bool Helper_is_muldiv_op();

};

}
}

#endif	// TOKEN_H_
