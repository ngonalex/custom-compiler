#include "backend/lowerer_visitor.h"

#include <vector>

#include "abstract_syntax/abstract_syntax.h"
#include "gtest/gtest.h"
#include "utility/memory.h"

using cs160::abstract_syntax::backend::AstVisitor;
using cs160::abstract_syntax::backend::IntegerExpr;
using cs160::abstract_syntax::backend::ArithmeticExpr;
using cs160::abstract_syntax::backend::AssignmentFromArithExp;
using cs160::backend::LowererVisitor;
using cs160::abstract_syntax::backend::AddExpr;
using cs160::abstract_syntax::backend::Statement;

class LowererTestV4 : public ::testing::Test {
  public:
    std::unique_ptr<const FunctionDef> GenerateFuncDef() {
    // empty params
    auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();

  // empty fact_body
    Statement::Block fact_body;

  // return value
    auto foo_retval = make_unique<const IntegerExpr>(0);

    auto foo_def = make_unique<const FunctionDef>("func", std::move(foo_params),
                                                std::move(fact_body),
                                                std::move(foo_retval));
    return foo_def;
  }

  std::string GenerateFuncDefOutPut(int blocksize) {
    return  " <-  PRINTARITH \n <-  FUNCTIONDEF \nMkLabel func\n <-"
    "FUNPROLOGUE \nt_" + std::to_string(blocksize) + " <- 0\n <-"
    "  FUNEPILOGUE \n";
  }

  protected:
   LowererVisitor lowerer_;
};

TEST_F(LowererTestV4, FunctionDefTest) {
  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();

  // empty fact_body
  Statement::Block fact_body;

  // return value
  auto foo_retval = make_unique<const AddExpr>(
    make_unique<const IntegerExpr>(1),
    make_unique<const IntegerExpr>(0));

  auto foo_def = make_unique<const FunctionDef>("func", std::move(foo_params),
                                                std::move(fact_body),
                                                std::move(foo_retval));

  foo_def->Visit(&lowerer_);
  EXPECT_EQ(lowerer_.GetOutput(), " <-  FUNCTIONDEF \nMkLabel func\n"
  " <-  FUNPROLOGUE \nt_0 <- 1\nt_1 <- 0\n"
  "t_2 <- t_0 + t_1\n <-  FUNEPILOGUE \n");
}


TEST_F(LowererTestV4, FunctionCallTest) {
  Statement::Block statements;

  // bob = 10
  // int returnval = fact(bob)
  // func fact (int bob) {
  //    returnval = 1
  //    return retunval + 0
  // }
  statements.push_back(make_unique<const AssignmentFromArithExp>(
    make_unique<const VariableExpr>("bob"),
    make_unique<const IntegerExpr>(10)));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();
  arguments.push_back(std::move(make_unique<const VariableExpr>("bob")));

  // call function fact
  statements.push_back(std::move(make_unique<const FunctionCall>(
      make_unique<const VariableExpr>("foo_retval"), "fact",
      std::move(arguments))));

  // getting the return value
  auto ae = make_unique<const VariableExpr>("foo_retval");

  auto foo_retval = make_unique<const AddExpr>(
    make_unique<const VariableExpr>("foo_retval"),
    make_unique<const IntegerExpr>(0));

  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();
  foo_params.push_back(std::move(make_unique<const VariableExpr>("bob")));

  Statement::Block fact_body;

  fact_body.push_back(std::move(make_unique<AssignmentFromArithExp>(
    make_unique<VariableExpr>("foo_retval"),
    make_unique<IntegerExpr>(1))));

  // fact_body.push_back(std::move(make_unique<const AddExpr>(
  //  make_unique<const VariableExpr>("foo_reval"),
  //  make_unique<const VariableExpr>("bob"))));

  auto foo_def = make_unique<const FunctionDef>("fact", std::move(foo_params),
                                                std::move(fact_body),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto ast = make_unique<const Program>(std::move(function_defs),
    std::move(statements), std::move(ae));


  ast->Visit(&lowerer_);

  EXPECT_EQ(lowerer_.GetOutput(), "t_0 <- 10\nbob <- t_0\n"
    "t_1 <- bob VARLOAD \n <-  FUNCTIONCALL \n"
    "foo_retval <- FUNRETLOAD FUNRETLOAD \n <-  FUNRETURNEPILOGUE \n"
    "t_2 <- foo_retval VARLOAD \n <-  PRINTARITH \n <-  FUNCTIONDEF \n"
    "MkLabel fact\n <-  FUNPROLOGUE \nbob <- 0\nt_3 <- 1\nfoo_retval <- t_3\n"
    "t_4 <- foo_retval VARLOAD \nt_5 <- 0\nt_6 <- t_4 + t_5\n"
    " <-  FUNEPILOGUE \n");
}

TEST_F(LowererTestV4, UndefinedFunctionCallTest) {
  Statement::Block statements;

  // bob = 10
  // int returnval = fact(bob)
  // func fact (int bob) {
  //    returnval = 1
  //    return retunval + 0
  // }
  statements.push_back(make_unique<const AssignmentFromArithExp>(
    make_unique<const VariableExpr>("bob"),
    make_unique<const IntegerExpr>(10)));
  statements.push_back(make_unique<const AssignmentFromArithExp>(
    make_unique<const VariableExpr>("foo"),
    make_unique<const IntegerExpr>(3)));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();
  arguments.push_back(std::move(make_unique<const VariableExpr>("bob")));
  arguments.push_back(std::move(make_unique<const VariableExpr>("foo")));

  // call function fact
  statements.push_back(std::move(make_unique<const FunctionCall>(
      make_unique<const VariableExpr>("foo_retval"), "fact",
      std::move(arguments))));

  // getting the return value
  auto ae = make_unique<const VariableExpr>("foo_retval");

  auto foo_retval = make_unique<const AddExpr>(
    make_unique<const VariableExpr>("foo_retval"),
    make_unique<const IntegerExpr>(0));

  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();
  foo_params.push_back(std::move(make_unique<const VariableExpr>("bob")));

  Statement::Block fact_body;

  fact_body.push_back(std::move(make_unique<AssignmentFromArithExp>(
    make_unique<VariableExpr>("foo_retval"),
    make_unique<IntegerExpr>(1))));

  // fact_body.push_back(std::move(make_unique<const AddExpr>(
  //  make_unique<const VariableExpr>("foo_reval"),
  //  make_unique<const VariableExpr>("bob"))));

  auto foo_def = make_unique<const FunctionDef>("fact", std::move(foo_params),
                                                std::move(fact_body),
                                                std::move(foo_retval));
  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto ast = make_unique<const Program>(std::move(function_defs),
                                        std::move(statements), std::move(ae));

  EXPECT_EXIT(ast->Visit(&lowerer_), ::testing::ExitedWithCode(1),
              "called undefined function");
}
