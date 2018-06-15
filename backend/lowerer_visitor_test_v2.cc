#include "backend/lowerer_visitor.h"

#include <vector>

#include "abstract_syntax/abstract_syntax.h"
#include "gtest/gtest.h"
#include "utility/memory.h"

using cs160::abstract_syntax::backend::AstVisitor;
using cs160::abstract_syntax::backend::IntegerExpr;
using cs160::abstract_syntax::backend::AssignmentFromArithExp;
using cs160::backend::LowererVisitor;
using cs160::abstract_syntax::backend::AddExpr;
using cs160::abstract_syntax::backend::Statement;

class LowererTestV2 : public ::testing::Test{
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

TEST_F(LowererTestV2, SimpleAssignmentTest) {
  auto expr = make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<IntegerExpr>(5));

  expr->Visit(&lowerer_);
  // t_0 <- 5
  // hello <- t_0
  EXPECT_EQ(lowerer_.GetOutput(), "t_0 <- 5\nx <- t_0\n");
}


TEST_F(LowererTestV2, DoubleIntAssignmentTest) {
  auto expr = make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)));

  expr->Visit(&lowerer_);

  // t_0 <- 5
  // t_1 <- 10
  // t_2 <- t_0 + t_1
  // x <- t_2
  EXPECT_EQ(lowerer_.GetOutput(),
            "t_0 <- 5\nt_1 <- 10\n"
            "t_2 <- t_0 + t_1\nx <- t_2\n");
}

TEST_F(LowererTestV2, UnassignedVariable) {
  auto expr = make_unique<const LogicalAndExpr>(
      make_unique<const LessThanExpr>(make_unique<const VariableExpr>("x"),
                                      make_unique<const IntegerExpr>(100)),
      make_unique<const GreaterThanExpr>(make_unique<const VariableExpr>("x"),
                                         make_unique<const IntegerExpr>(100)));

  // t_0 <- 100
  // t_1 <- x < t_0
  // t_2 <- 100
  // t_3 <- x > t_2
  // t_4 <- t_1 && t_3
  EXPECT_EXIT(expr->Visit(&lowerer_), ::testing::ExitedWithCode(1),
              "Variable x not assigned");
}

TEST_F(LowererTestV2, VariabletoVariableAssignmentTest) {
  Statement::Block statements;

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<SubtractExpr>(
          make_unique<VariableExpr>("x"),
          make_unique<IntegerExpr>(10)))));

  auto arithexpr = make_unique<SubtractExpr>(make_unique<IntegerExpr>(7),
                                             make_unique<IntegerExpr>(5));

  FunctionDef::Block function_defs;
  auto func_def = GenerateFuncDef();
  function_defs.push_back(std::move(func_def));

  auto expr = make_unique<Program>(std::move(function_defs),
                                   std::move(statements),
                                   std::move(arithexpr));

  expr->Visit(&lowerer_);

  // t_0 <- 5
  // t_1 <- 10
  // t_2 <- t_0 + t_1
  // x <- t_2
  // t_3 <- 10
  // t_4 <- x - t_3
  // y <- t_4
  // t_5 <- 7
  // t_6 <- 5
  // t_7 <- t_5 - t_6

  EXPECT_EQ(lowerer_.GetOutput(), "t_0 <- 5\nt_1 <- 10\nt_2 <- t_0 + t_1\n"
            "x <- t_2\nt_3 <- x VARLOAD \nt_4 <- 10\nt_5 <- t_3 - t_4\n"
            "y <- t_5\nt_6 <- 7\nt_7 <- 5\nt_8 <- t_6 - t_7\n"
            " <-  PRINTARITH \n <-  FUNCTIONDEF \nMkLabel func\n"
            " <-  FUNPROLOGUE \nt_9 <- 0\n <-  FUNEPILOGUE \n");
}
