#include "./parser.h"

using namespace cs160::frontend;

namespace cs160 {
namespace frontend {
  
  Parser::Parser(std::vector<Token> program) {
    this->program = program;
    this->result = new ParseResult;
  }
  
  // Ecounter parenthesis and numbers
  void Parser::start() {
    ParseResult* current = new ParseResult;
    if (program[0].type()  == Type::NUM) {
      //program[0].print();
      this->result = parseOperator(current, 1);
      printf("Result: %i \n", this->result->left.val() * this->result->right.val());
    }
  }
  
  ParseResult* Parser::parseOperator(ParseResult* p, int location) {
    switch(program[location].type()) {
      case Type::ADD_OP:
        //program[location].print();
        return (parseInt(p, ++location));
        break;
      case Type::SUB_OP:
        //program[location].print();
        return (parseInt(p, ++location));
        break;
      case Type::MUL_OP:
        //program[location].print();
        return (parseInt(p, ++location));
        break;
      case Type::DIV_OP:
        //program[location].print();
        return (parseInt(p, ++location));
        break;
    }
  }
  
  ParseResult* Parser::parseInt(ParseResult* p, int location) {
    if (program[location].type() == Type::NUM) {
      //program[location].print();
      p->left = program[0];
      p->op = program[1];
      p->right = program[2];
      return p;
    }
  }
  
  
  
} // namespace frontend
} // namespace cs160

// int main() {
//   cs160::frontend::Token firstToken(cs160::frontend::Type::NUM, 6);
//   cs160::frontend::Token secondToken(cs160::frontend::Type::MUL_OP);
//   cs160::frontend::Token thirdToken(cs160::frontend::Type::NUM, 3);
//   std::vector<cs160::frontend::Token> test_vector;
//   test_vector.push_back(firstToken);
//   test_vector.push_back(secondToken);
//   test_vector.push_back(thirdToken);
// 
//   cs160::frontend::Parser parser(test_vector);
//   parser.start();
// 
//   return 0;
// }