#ifndef PARSER_PARSER_H_
#define PARSER_PARSER_H_

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/tokenizer/token.h"

#include <sstream>
#include <iostream>
#include <stack>

#include "utility/assert.h"
#include "utility/memory.h"

using namespace cs160::abstract_syntax::frontend;
using cs160::frontend::Token;
using namespace std;

namespace cs160 {
namespace frontend {

// Program -> Block* Expr

// Block -> Statment*

// Statement -> Assignment | Cond | Loop (should be fine given that the three are fine)

// Cond -> if Logic Block Block
// Loop -> while Logic Block

// Logic - > Rel and Rel | Rel or Rel | not Rel | Rel --> NOT is tricky for parsing....
// Rel -> Expr < Expr | Expr <= Expr | Expr > Expr | Expr >= Expr | Expr == Expr (these should all terminate)

// Assignment -> Id VarName = Expr (fine)
// Expr -> Expr + Expr | Expr - Expr | Fact (fine)
// Fact -> Fact * Fact | Fact / Fact | Term (fine)
// Term -> ( Expr ) | Num | VarName (fine)

// (fine)
// Num  -> [0, 9]+
// VarName -> String
// Id -> "int" 


class Parser {
 public:
   
  explicit Parser(std::vector<Token> program) : program_(program) {
    ASSERT(program.size() != 0, "Program cannot be empty tokens");
    std::reverse(this->program_.begin(), this->program_.end());
  }
  Parser() { }
  Token::Type Next() { return program_.back().type(); }
  Token::Type DoubleNext() {
    return program_.rbegin()[1].type();
  }
  void Consume() { program_.pop_back(); };
  void Error() {
    std::cerr << "Error!" << std::endl;
    exit(-1);
  }
  void Expect(Token::Type type) { Next() == type ? Consume() : Error(); }
  
  void ExpectID(std::string id) {
    program_.back().idVal() == id ? Consume() : Error();
  }

  // AST Expressions
  std::unique_ptr<const ArithmeticExpr> MakeArithmeticExpr(Token::Type op,
    std::unique_ptr<const ArithmeticExpr> first_leaf,
    std::unique_ptr<const ArithmeticExpr> second_leaf);
    
  std::unique_ptr<const ArithmeticExpr> MakeInteger(Token num);

<<<<<<< HEAD
  std::unique_ptr<const AstNode> Eparser();
    
  std::unique_ptr<const AstNode> ParseLine();
    
  std::unique_ptr<const AstNode> ParseAddSub();
=======
  std::unique_ptr<const VariableExpr> ParseVariable(Token curr);

  std::unique_ptr<const ArithmeticExpr> Eparser();

  std::unique_ptr<const Program> ParseProgram();

  std::unique_ptr<const Assignment> ParseAssignment();

  std::unique_ptr<const ArithmeticExpr> ParseAddSub();
>>>>>>> master

  std::unique_ptr<const ArithmeticExpr> ParseMulDiv();

  std::unique_ptr<const ArithmeticExpr> ParseExpression();

// Tokens will now be in reverse order!
 private:
  // Output from the lexer
  std::vector<Token> program_;

};


} // namespace frontend
} // namespace cs160

#endif // PARSER_PARSER_H_
