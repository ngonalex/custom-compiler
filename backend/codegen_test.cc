#include <stdio.h>

#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

#include "backend/code_gen.h"
#include "abstract_syntax/abstract_syntax.h"
#include "backend/lowerer_visitor.h"
#include "backend/ir.h"
#include "utility/memory.h"
#include "gtest/gtest.h"

using cs160::abstract_syntax::backend::AstVisitor;
using cs160::abstract_syntax::backend::IntegerExpr;
using cs160::abstract_syntax::backend::AddExpr;
using cs160::abstract_syntax::backend::SubtractExpr;
using cs160::abstract_syntax::backend::MultiplyExpr;
using cs160::abstract_syntax::backend::DivideExpr;
using cs160::abstract_syntax::backend::ArithmeticExpr;
using cs160::backend::LowererVisitor;
using cs160::backend::ThreeAddressCode;
using cs160::backend::CodeGen;
using cs160::backend::PrintFlag::PRINT_DEBUG;
using cs160::backend::PrintFlag::PRINT_LAST_ARITHMETIC_EXPR;
using cs160::backend::PrintFlag::PRINT_ONLY_RESULT;
using cs160::abstract_syntax::version_5::Statement;
using cs160::make_unique;

class CodeGenTest : public ::testing::Test {
 protected:
  LowererVisitor lowerer_;
};

// this piece of code is from stackoverflow
// link:
// https://stackoverflow.com/questions/478898/how-to-execute-a-command-and-get-output-of-command-within-c-using-posix
std::string exec(const char* cmd) {
  std::array<char, 128> buffer;
  std::string result;
  std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
  while (!feof(pipe.get())) {
    if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
      result += buffer.data();
    }
  return result;
}

// V1 Tests

// Simple load of the integer 4
TEST_F(CodeGenTest, PositiveIntegerExprLoadedCorrectly) {
  auto expr = make_unique<IntegerExpr>(4);

  expr->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_LAST_ARITHMETIC_EXPR);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The program returned: 4\n");
}

// Simple load of the integer -4
TEST_F(CodeGenTest, NegativeIntegerExprLoadedCorrectly) {
  auto expr = make_unique<IntegerExpr>(-4);

  expr->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_LAST_ARITHMETIC_EXPR);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The program returned: -4\n");
}

// 1 + 2
TEST_F(CodeGenTest, AddExprWithTwoPositiveIntegers) {
  auto expr = make_unique<AddExpr>(
      make_unique<IntegerExpr>(1), make_unique<IntegerExpr>(2));

  expr->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_LAST_ARITHMETIC_EXPR);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The program returned: 3\n");
}

// -1 + -2
TEST_F(CodeGenTest, AddExprWithTwoNegativeIntegers) {
  auto expr = make_unique<AddExpr>(
      make_unique<IntegerExpr>(-1), make_unique<IntegerExpr>(-2));

  expr->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_LAST_ARITHMETIC_EXPR);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The program returned: -3\n");
}

// -1 + 2
TEST_F(CodeGenTest, AddExprWithDifferentSignIntegersVisited) {
  auto expr = make_unique<AddExpr>(
      make_unique<IntegerExpr>(-1), make_unique<IntegerExpr>(2));

  expr->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_LAST_ARITHMETIC_EXPR);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The program returned: 1\n");
}

// 5 - 1
TEST_F(CodeGenTest, SubtractExprWithTwoPositiveIntsVisited) {
  auto expr = make_unique<SubtractExpr>(
      make_unique<IntegerExpr>(5), make_unique<IntegerExpr>(1));

  expr->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_LAST_ARITHMETIC_EXPR);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The program returned: 4\n");
}

// -5 - (-1)
TEST_F(CodeGenTest, SubtractExprWithTwoNegativeIntsVisited) {
  auto expr = make_unique<SubtractExpr>(
      make_unique<IntegerExpr>(-5), make_unique<IntegerExpr>(-1));

  expr->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_LAST_ARITHMETIC_EXPR);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The program returned: -4\n");
}

// -5 - 1
TEST_F(CodeGenTest, SubtractExprWithTwoDifferentSignsVisited) {
  auto expr = make_unique<SubtractExpr>(
      make_unique<IntegerExpr>(-5), make_unique<IntegerExpr>(1));

  expr->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_LAST_ARITHMETIC_EXPR);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The program returned: -6\n");
}

// 10 * 5
TEST_F(CodeGenTest, MultiplyExprWithTwoPositiveIntsVisited) {
  auto expr = make_unique<MultiplyExpr>(
      make_unique<IntegerExpr>(10), make_unique<IntegerExpr>(5));

  expr->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_LAST_ARITHMETIC_EXPR);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The program returned: 50\n");
}

// -10 * -5
TEST_F(CodeGenTest, MultiplyExprWithTwoNegativeIntsVisited) {
  auto expr = make_unique<MultiplyExpr>(
      make_unique<IntegerExpr>(-10), make_unique<IntegerExpr>(-5));

  expr->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_LAST_ARITHMETIC_EXPR);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The program returned: 50\n");
}

// -10 * 5
TEST_F(CodeGenTest, MultiplyExprWithDifferentSignsVisited) {
  auto expr = make_unique<MultiplyExpr>(
      make_unique<IntegerExpr>(-10), make_unique<IntegerExpr>(5));

  expr->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_LAST_ARITHMETIC_EXPR);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The program returned: -50\n");
}

// 10 / 5
TEST_F(CodeGenTest, DivideExprWithTwoPositiveIntsVisited) {
  auto expr = make_unique<DivideExpr>(
      make_unique<IntegerExpr>(10), make_unique<IntegerExpr>(5));

  expr->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_LAST_ARITHMETIC_EXPR);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The program returned: 2\n");
}

// -10 / -5
TEST_F(CodeGenTest, DivideExprWithNegativeIntsVisited) {
  auto expr = make_unique<DivideExpr>(
      make_unique<IntegerExpr>(-10), make_unique<IntegerExpr>(-5));

  expr->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_LAST_ARITHMETIC_EXPR);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The program returned: 2\n");
}

// 10 / -5
TEST_F(CodeGenTest, DivideExprWithDifferentSignsVisited) {
  auto expr = make_unique<DivideExpr>(
      make_unique<IntegerExpr>(10), make_unique<IntegerExpr>(-5));

  expr->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_LAST_ARITHMETIC_EXPR);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The program returned: -2\n");
}

// 0 / -5
TEST_F(CodeGenTest, DivideExprWithZeroAsDividendVisited) {
  auto expr = make_unique<DivideExpr>(
      make_unique<IntegerExpr>(0), make_unique<IntegerExpr>(-5));

  expr->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_LAST_ARITHMETIC_EXPR);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The program returned: 0\n");
}

// 10 / 0
TEST_F(CodeGenTest, DivisionByZeroError) {
  auto expr = make_unique<DivideExpr>(
      make_unique<IntegerExpr>(10), make_unique<IntegerExpr>(0));

  expr->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_LAST_ARITHMETIC_EXPR);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Error: Division by zero\n");
}

// (7+5) / (4-1)
TEST_F(CodeGenTest, NestedArithmeticExprs) {
  auto expr = make_unique<DivideExpr>(
      make_unique<AddExpr>(make_unique<IntegerExpr>(7),
                           make_unique<IntegerExpr>(5)),
      make_unique<SubtractExpr>(make_unique<IntegerExpr>(4),
                                make_unique<IntegerExpr>(1)));

  expr->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_LAST_ARITHMETIC_EXPR);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The program returned: 4\n");
}

// V2 Tests

// x = 5
TEST_F(CodeGenTest, SimplePositiveAssignmentTest) {
  auto expr = make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<IntegerExpr>(5));

  expr->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable x is equal to: 5\n");
}

// x = -5
TEST_F(CodeGenTest, SimpleNegativeAssignmentTest) {
  auto expr = make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<IntegerExpr>(-5));

  expr->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable x is equal to: -5\n");
}

// x = 5 + 10
TEST_F(CodeGenTest, ArithmeticExprOnRHSAssignmentTest) {
  auto expr = make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)));

  expr->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable x is equal to: 15\n");
}

// x = 5 + 10
// y = x - 5
TEST_F(CodeGenTest, VariabletoVariableAssignmentTest) {
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

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable x is equal to: 15\nVariable y is equal to: 5\n");
}

// V3 Tests

// if( 1 < 2 ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, LessThanExprWithOnlyIntsThatEvalsToTrue) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<LessThanExpr>(
          make_unique<IntegerExpr>(1),
          make_unique<IntegerExpr>(2)),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable x is equal to: 15\n");
}

// if( 3 < 2 ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, LessThanExprWithOnlyIntsThatEvalsToFalse) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<LessThanExpr>(
          make_unique<IntegerExpr>(3),
          make_unique<IntegerExpr>(2)),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable y is equal to: 20\n");
}

// z = 10
// w = 20
// if( z < w ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, LessThanExprWithVariablesThatEvalsToTrue) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"), make_unique<IntegerExpr>(10))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("w"), make_unique<IntegerExpr>(20))));

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<LessThanExpr>(
          make_unique<VariableExpr>("z"),
          make_unique<VariableExpr>("w")),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable z is equal to: 10\n"
            "Variable w is equal to: 20\n"
            "Variable x is equal to: 15\n");
}

// z = 10
// w = 20
// if( w < z ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, LessThanExprWithVariablesThatEvalsToFalse) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"), make_unique<IntegerExpr>(10))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("w"), make_unique<IntegerExpr>(20))));

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<LessThanExpr>(
          make_unique<VariableExpr>("w"),
          make_unique<VariableExpr>("z")),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable z is equal to: 10\n"
            "Variable w is equal to: 20\n"
            "Variable y is equal to: 20\n");
}

// z = 10
// w = 20
// if( 10 + w < 10 + z ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, LessThanExprWithMoreComplexArithExprsOnBothSides) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"), make_unique<IntegerExpr>(10))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("w"), make_unique<IntegerExpr>(20))));

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<LessThanExpr>(
          make_unique<AddExpr>(
              make_unique<IntegerExpr>(10),
              make_unique<VariableExpr>("w")),
          make_unique<AddExpr>(
              make_unique<IntegerExpr>(10),
              make_unique<VariableExpr>("z"))),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable z is equal to: 10\n"
            "Variable w is equal to: 20\n"
            "Variable y is equal to: 20\n");
}

// w = 20
// if( w < w ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, LessThanExprEvalsToFalseWhenEqual) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;


  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("w"), make_unique<IntegerExpr>(20))));

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<LessThanExpr>(
          make_unique<VariableExpr>("w"),
          make_unique<VariableExpr>("w")),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable w is equal to: 20\n"
            "Variable y is equal to: 20\n");
}

// if( 1 <= 2 ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, LessThanEqualToExprWithOnlyIntsThatEvalsToTrue) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<LessThanEqualToExpr>(
          make_unique<IntegerExpr>(1),
          make_unique<IntegerExpr>(2)),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable x is equal to: 15\n");
}

// if( 3 <= 2 ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, LessThanEqualToExprWithOnlyIntsThatEvalsToFalse) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<LessThanEqualToExpr>(
          make_unique<IntegerExpr>(3),
          make_unique<IntegerExpr>(2)),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable y is equal to: 20\n");
}

// z = 10
// w = 20
// if( z <= w ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, LessThanEqualToExprWithVariablesThatEvalsToTrue) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"), make_unique<IntegerExpr>(10))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("w"), make_unique<IntegerExpr>(20))));

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<LessThanEqualToExpr>(
          make_unique<VariableExpr>("z"),
          make_unique<VariableExpr>("w")),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable z is equal to: 10\n"
            "Variable w is equal to: 20\n"
            "Variable x is equal to: 15\n");
}

// z = 10
// w = 20
// if( w <= z ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, LessThanEqualToExprWithVariablesThatEvalsToFalse) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"), make_unique<IntegerExpr>(10))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("w"), make_unique<IntegerExpr>(20))));

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<LessThanEqualToExpr>(
          make_unique<VariableExpr>("w"),
          make_unique<VariableExpr>("z")),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable z is equal to: 10\n"
            "Variable w is equal to: 20\n"
            "Variable y is equal to: 20\n");
}

// z = 10
// w = 20
// if( 10 + w <= 10 + z ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, LessThanEqualToExprWithMoreComplexArithExprsOnBothSides) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"), make_unique<IntegerExpr>(10))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("w"), make_unique<IntegerExpr>(20))));

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<LessThanEqualToExpr>(
          make_unique<AddExpr>(
              make_unique<IntegerExpr>(10),
              make_unique<VariableExpr>("w")),
          make_unique<AddExpr>(
              make_unique<IntegerExpr>(10),
              make_unique<VariableExpr>("z"))),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable z is equal to: 10\n"
            "Variable w is equal to: 20\n"
            "Variable y is equal to: 20\n");
}

// w = 20
// if( w <= w ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, LessThanEqualToExprEvalsToTrueWhenEqual) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;


  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("w"), make_unique<IntegerExpr>(20))));

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
        make_unique<IntegerExpr>(5),
        make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<LessThanEqualToExpr>(
          make_unique<VariableExpr>("w"),
          make_unique<VariableExpr>("w")),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable w is equal to: 20\n"
            "Variable x is equal to: 15\n");
}

// if( 1 > 2 ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, GreaterThanExprWithOnlyIntsThatEvalsToFalse) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<GreaterThanExpr>(
          make_unique<IntegerExpr>(1),
          make_unique<IntegerExpr>(2)),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable y is equal to: 20\n");
}

// if( 3 > 2 ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, GreaterThanExprWithOnlyIntsThatEvalsToTrue) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<GreaterThanExpr>(
          make_unique<IntegerExpr>(3),
          make_unique<IntegerExpr>(2)),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable x is equal to: 15\n");
}

// z = 10
// w = 20
// if( z > w ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, GreaterThanExprWithVariablesThatEvalsToFalse) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"), make_unique<IntegerExpr>(10))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("w"), make_unique<IntegerExpr>(20))));

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<GreaterThanExpr>(
          make_unique<VariableExpr>("z"),
          make_unique<VariableExpr>("w")),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable z is equal to: 10\n"
            "Variable w is equal to: 20\n"
            "Variable y is equal to: 20\n");
}

// z = 10
// w = 20
// if( w > z ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, GreaterThanExprWithVariablesThatEvalsToTrue) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"), make_unique<IntegerExpr>(10))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("w"), make_unique<IntegerExpr>(20))));

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<GreaterThanExpr>(
          make_unique<VariableExpr>("w"),
          make_unique<VariableExpr>("z")),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable z is equal to: 10\n"
            "Variable w is equal to: 20\n"
            "Variable x is equal to: 15\n");
}

// z = 10
// w = 20
// if( 10 + w > 10 + z ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, GreaterThanExprWithMoreComplexArithExprsOnBothSides) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"), make_unique<IntegerExpr>(10))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("w"), make_unique<IntegerExpr>(20))));

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<GreaterThanExpr>(
          make_unique<AddExpr>(
              make_unique<IntegerExpr>(10),
              make_unique<VariableExpr>("w")),
          make_unique<AddExpr>(
              make_unique<IntegerExpr>(10),
              make_unique<VariableExpr>("z"))),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable z is equal to: 10\n"
            "Variable w is equal to: 20\n"
            "Variable x is equal to: 15\n");
}

// w = 20
// if( w > w ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, GreaterThanExprEvalsToFalseWhenEqual) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;


  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("w"), make_unique<IntegerExpr>(20))));

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<GreaterThanExpr>(
          make_unique<VariableExpr>("w"),
          make_unique<VariableExpr>("w")),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable w is equal to: 20\n"
            "Variable y is equal to: 20\n");
}

// if( 1 >= 2 ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, GreaterThanEqualToExprWithOnlyIntsThatEvalsToFalse) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<GreaterThanEqualToExpr>(
          make_unique<IntegerExpr>(1),
          make_unique<IntegerExpr>(2)),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable y is equal to: 20\n");
}

// if( 3 >= 2 ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, GreaterThanEqualToExprWithOnlyIntsThatEvalsToTrue) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<GreaterThanEqualToExpr>(
          make_unique<IntegerExpr>(3),
          make_unique<IntegerExpr>(2)),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable x is equal to: 15\n");
}

// z = 10
// w = 20
// if( z >= w ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, GreaterThanEqualToExprWithVariablesThatEvalsToFalse) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"), make_unique<IntegerExpr>(10))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("w"), make_unique<IntegerExpr>(20))));

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<GreaterThanEqualToExpr>(
          make_unique<VariableExpr>("z"),
          make_unique<VariableExpr>("w")),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable z is equal to: 10\n"
            "Variable w is equal to: 20\n"
            "Variable y is equal to: 20\n");
}

// z = 10
// w = 20
// if( w >= z ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, GreaterThanEqualToExprWithVariablesThatEvalsToTrue) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"), make_unique<IntegerExpr>(10))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("w"), make_unique<IntegerExpr>(20))));

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<GreaterThanEqualToExpr>(
          make_unique<VariableExpr>("w"),
          make_unique<VariableExpr>("z")),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable z is equal to: 10\n"
            "Variable w is equal to: 20\n"
            "Variable x is equal to: 15\n");
}

// z = 10
// w = 20
// if( 10 + w >= 10 + z ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(
    CodeGenTest, GreaterThanEqualToExprWithMoreComplexArithExprsOnBothSides) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"), make_unique<IntegerExpr>(10))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("w"), make_unique<IntegerExpr>(20))));

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<GreaterThanEqualToExpr>(
          make_unique<AddExpr>(
              make_unique<IntegerExpr>(10),
              make_unique<VariableExpr>("w")),
          make_unique<AddExpr>(
              make_unique<IntegerExpr>(10),
              make_unique<VariableExpr>("z"))),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable z is equal to: 10\n"
            "Variable w is equal to: 20\n"
            "Variable x is equal to: 15\n");
}

// w = 20
// if( w >= w ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, GreaterThanEqualToExprEvalsToTrueWhenEqual) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;


  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("w"), make_unique<IntegerExpr>(20))));

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<GreaterThanEqualToExpr>(
          make_unique<VariableExpr>("w"),
          make_unique<VariableExpr>("w")),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable w is equal to: 20\n"
            "Variable x is equal to: 15\n");
}


// if( -5 == -5 ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, EqualsToCorrectlyEvalsToTrueWithInts) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<EqualToExpr>(
          make_unique<IntegerExpr>(-5),
          make_unique<IntegerExpr>(-5)),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable x is equal to: 15\n");
}

// if( 10 == -5 ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, EqualsToCorrectlyEvalsToFalseWithInts) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<EqualToExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(-5)),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable y is equal to: 20\n");
}

// if( 10 == 15 ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, EqualsToCorrectlyEvalsToFalseWithIntsTwo) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<EqualToExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(15)),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable y is equal to: 20\n");
}

// z = 10
// w = 10
// if( 10 + w == 10 + z ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, EqualsToEvaluatesTrueCorrectlyWithMoreComplexExpr) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"), make_unique<IntegerExpr>(10))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("w"), make_unique<IntegerExpr>(10))));

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<EqualToExpr>(
          make_unique<AddExpr>(
              make_unique<IntegerExpr>(10),
              make_unique<VariableExpr>("w")),
          make_unique<AddExpr>(
              make_unique<IntegerExpr>(10),
              make_unique<VariableExpr>("z"))),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable z is equal to: 10\n"
            "Variable w is equal to: 10\n"
            "Variable x is equal to: 15\n");
}

// z = 10
// w = 20
// if( 10 + w == 10 + z ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, EqualsToEvaluatesFalseCorrectlyWithMoreComplexExpr) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"), make_unique<IntegerExpr>(10))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("w"), make_unique<IntegerExpr>(20))));

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<EqualToExpr>(
          make_unique<AddExpr>(
              make_unique<IntegerExpr>(10),
              make_unique<VariableExpr>("w")),
          make_unique<AddExpr>(
              make_unique<IntegerExpr>(10),
              make_unique<VariableExpr>("z"))),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable z is equal to: 10\n"
            "Variable w is equal to: 20\n"
            "Variable y is equal to: 20\n");
}

// z = 10
// w = 10
// if( w == 10 && z <= 10 ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, LogicalAndExprEvalsToTrueWhenBothSidesAreTrue) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"), make_unique<IntegerExpr>(10))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("w"), make_unique<IntegerExpr>(10))));

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<LogicalAndExpr>(
          make_unique<EqualToExpr>(
              make_unique<VariableExpr>("w"),
              make_unique<IntegerExpr>(10)),
          make_unique<LessThanEqualToExpr>(
              make_unique<VariableExpr>("z"),
              make_unique<IntegerExpr>(10))),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable z is equal to: 10\n"
            "Variable w is equal to: 10\n"
            "Variable x is equal to: 15\n");
}

// z = 10
// w = 10
// if( w == 20 && z < 10 ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, LogicalAndExprEvalsToFalseWhenBothSidesAreFalse) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"), make_unique<IntegerExpr>(10))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("w"), make_unique<IntegerExpr>(10))));

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<LogicalAndExpr>(
          make_unique<EqualToExpr>(
              make_unique<VariableExpr>("w"),
              make_unique<IntegerExpr>(20)),
          make_unique<LessThanExpr>(
              make_unique<VariableExpr>("z"),
              make_unique<IntegerExpr>(10))),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable z is equal to: 10\n"
            "Variable w is equal to: 10\n"
            "Variable y is equal to: 20\n");
}

// z = 10
// w = 10
// if( w == 20 && z == 10 ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, LogicalAndExprEvalsToFalseIfLeftSideIsFalse) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"), make_unique<IntegerExpr>(10))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("w"), make_unique<IntegerExpr>(10))));

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<LogicalAndExpr>(
          make_unique<EqualToExpr>(
              make_unique<VariableExpr>("w"),
              make_unique<IntegerExpr>(20)),
          make_unique<EqualToExpr>(
              make_unique<VariableExpr>("z"),
              make_unique<IntegerExpr>(10))),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable z is equal to: 10\n"
            "Variable w is equal to: 10\n"
            "Variable y is equal to: 20\n");
}

// z = 10
// w = 10
// if( w == 10 && z == -10 ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, LogicalAndExprEvalsToFalseIfRightSideIsFalse) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"), make_unique<IntegerExpr>(10))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("w"), make_unique<IntegerExpr>(10))));

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<LogicalAndExpr>(
          make_unique<EqualToExpr>(
              make_unique<VariableExpr>("w"),
              make_unique<IntegerExpr>(10)),
          make_unique<EqualToExpr>(
              make_unique<VariableExpr>("z"),
              make_unique<IntegerExpr>(-10))),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable z is equal to: 10\n"
            "Variable w is equal to: 10\n"
            "Variable y is equal to: 20\n");
}

// z = 10
// w = 10
// if( w >= 10 || z < 20 ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, LogicalOrExprEvalsToTrueWhenBothSidesAreTrue) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"), make_unique<IntegerExpr>(10))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("w"), make_unique<IntegerExpr>(10))));

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<LogicalOrExpr>(
          make_unique<GreaterThanEqualToExpr>(
              make_unique<VariableExpr>("w"),
              make_unique<IntegerExpr>(10)),
          make_unique<LessThanExpr>(
              make_unique<VariableExpr>("z"),
              make_unique<IntegerExpr>(20))),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable z is equal to: 10\n"
            "Variable w is equal to: 10\n"
            "Variable x is equal to: 15\n");
}

// z = 10
// w = 10
// if( w == 20 || z < 10 ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, LogicalOrExprEvalsToFalseWhenBothSidesAreFalse) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"), make_unique<IntegerExpr>(10))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("w"), make_unique<IntegerExpr>(10))));

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<LogicalOrExpr>(
          make_unique<EqualToExpr>(
              make_unique<VariableExpr>("w"),
              make_unique<IntegerExpr>(20)),
          make_unique<LessThanExpr>(
              make_unique<VariableExpr>("z"),
              make_unique<IntegerExpr>(10))),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable z is equal to: 10\n"
            "Variable w is equal to: 10\n"
            "Variable y is equal to: 20\n");
}

// z = 10
// w = 10
// if( w >= 20 && z == 10 ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, LogicalOrExprEvalsToTrueEvenIfLeftSideIsFalse) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"), make_unique<IntegerExpr>(10))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("w"), make_unique<IntegerExpr>(10))));

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<LogicalOrExpr>(
          make_unique<GreaterThanEqualToExpr>(
              make_unique<VariableExpr>("w"),
              make_unique<IntegerExpr>(20)),
          make_unique<EqualToExpr>(
              make_unique<VariableExpr>("z"),
              make_unique<IntegerExpr>(10))),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable z is equal to: 10\n"
            "Variable w is equal to: 10\n"
            "Variable x is equal to: 15\n");
}

// z = 10
// w = 10
// if( w == 10 || z < -10 ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, LogicalOrExprEvalsToTrueEvenIfRightSideIsFalse) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"), make_unique<IntegerExpr>(10))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("w"), make_unique<IntegerExpr>(10))));

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<LogicalOrExpr>(
          make_unique<EqualToExpr>(
              make_unique<VariableExpr>("w"),
              make_unique<IntegerExpr>(10)),
          make_unique<LessThanExpr>(
              make_unique<VariableExpr>("z"),
              make_unique<IntegerExpr>(-10))),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable z is equal to: 10\n"
            "Variable w is equal to: 10\n"
            "Variable x is equal to: 15\n");
}

// z = 10
// w = 10
// if( !(w == 10 || z < -10) ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, LogicalNotExprCorrectlyNegatesATrueExpr) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"), make_unique<IntegerExpr>(10))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("w"), make_unique<IntegerExpr>(10))));

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<LogicalNotExpr>(
          make_unique<LogicalOrExpr>(
              make_unique<EqualToExpr>(
                  make_unique<VariableExpr>("w"),
                  make_unique<IntegerExpr>(10)),
              make_unique<LessThanExpr>(
                  make_unique<VariableExpr>("z"),
                  make_unique<IntegerExpr>(-10)))),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable z is equal to: 10\n"
            "Variable w is equal to: 10\n"
            "Variable y is equal to: 20\n");
}

// z = 10
// w = 10
// if( !(w == 10 && z < -10) ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, LogicalNotExprCorrectlyNegatesAFalseExpr) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"), make_unique<IntegerExpr>(10))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("w"), make_unique<IntegerExpr>(10))));

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<LogicalNotExpr>(
          make_unique<LogicalAndExpr>(
              make_unique<EqualToExpr>(
                  make_unique<VariableExpr>("w"),
                  make_unique<IntegerExpr>(10)),
              make_unique<LessThanExpr>(
                  make_unique<VariableExpr>("z"),
                  make_unique<IntegerExpr>(-10)))),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable z is equal to: 10\n"
            "Variable w is equal to: 10\n"
            "Variable x is equal to: 15\n");
}

// if( !((5 < 10 && -10 > 10) || (500 == 500 || -10000 >= 10000)) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, NestedRelationalExprThatEvalsToFalse) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<LogicalNotExpr>(
          make_unique<LogicalOrExpr>(
              make_unique<LogicalAndExpr>(
                  make_unique<LessThanExpr>(
                      make_unique<IntegerExpr>(5),
                      make_unique<IntegerExpr>(10)),
                  make_unique<GreaterThanExpr>(
                      make_unique<IntegerExpr>(-10),
                      make_unique<IntegerExpr>(10))),
              make_unique<LogicalOrExpr>(
                  make_unique<EqualToExpr>(
                      make_unique<IntegerExpr>(500),
                      make_unique<IntegerExpr>(500)),
                  make_unique<GreaterThanEqualToExpr>(
                      make_unique<IntegerExpr>(-10000),
                      make_unique<IntegerExpr>(100000))))),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable y is equal to: 20\n");
}

// if( !((5 < 10 && -10 > 10) || (-500 == 500 || -10000 >= 10000)) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, NestedRelationalExprThatEvalsTotrue) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<LogicalNotExpr>(
          make_unique<LogicalOrExpr>(
              make_unique<LogicalAndExpr>(
                  make_unique<LessThanExpr>(
                      make_unique<IntegerExpr>(5),
                      make_unique<IntegerExpr>(10)),
                  make_unique<GreaterThanExpr>(
                      make_unique<IntegerExpr>(-10),
                      make_unique<IntegerExpr>(10))),
              make_unique<LogicalOrExpr>(
                  make_unique<EqualToExpr>(
                      make_unique<IntegerExpr>(-500),
                      make_unique<IntegerExpr>(500)),
                  make_unique<GreaterThanEqualToExpr>(
                      make_unique<IntegerExpr>(-10000),
                      make_unique<IntegerExpr>(100000))))),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable x is equal to: 15\n");
}

// z = 10
// w = 10
// if( !(w == 10 && z < -10) ) {
//  x = 5 + 10
//  y = 5 + 10
// } else {
//  x = 10 + 10
//  y = 10 + 10
// }
TEST_F(CodeGenTest, BasicConditionalBlockThatEvalsToTrue) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"), make_unique<IntegerExpr>(10))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("w"), make_unique<IntegerExpr>(10))));

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<LogicalNotExpr>(
          make_unique<LogicalAndExpr>(
              make_unique<EqualToExpr>(
                  make_unique<VariableExpr>("w"),
                  make_unique<IntegerExpr>(10)),
              make_unique<LessThanExpr>(
                  make_unique<VariableExpr>("z"),
                  make_unique<IntegerExpr>(-10)))),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable z is equal to: 10\n"
            "Variable w is equal to: 10\n"
            "Variable x is equal to: 15\n"
            "Variable y is equal to: 15\n");
}

// z = 10
// w = 10
// if( !(w == 10 && z <= -10) ) {
//  x = 5 + 10
//  y = 5 + 10
// } else {
//  x = 10 + 10
//  y = 10 + 10
// }
TEST_F(CodeGenTest, BasicConditionalBlockThatEvalsToFalse) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"), make_unique<IntegerExpr>(10))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("w"), make_unique<IntegerExpr>(10))));

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<LogicalNotExpr>(
          make_unique<LogicalAndExpr>(
              make_unique<EqualToExpr>(
                  make_unique<VariableExpr>("w"),
                  make_unique<IntegerExpr>(10)),
              make_unique<LessThanEqualToExpr>(
                  make_unique<VariableExpr>("z"),
                  make_unique<IntegerExpr>(10)))),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable z is equal to: 10\n"
            "Variable w is equal to: 10\n"
            "Variable x is equal to: 20\n"
            "Variable y is equal to: 20\n");
}

// if( 15 == 15 ) {
// } else {
//   x = 5 + 10
// }
// z = 500 + 500
TEST_F(CodeGenTest, GoingToAnEmptyTrueBranchStillExitsCorrectly) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<EqualToExpr>(
          make_unique<IntegerExpr>(15),
          make_unique<IntegerExpr>(15)),
      Statement::Block(), std::move(trueblock))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(500),
          make_unique<IntegerExpr>(500)))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable z is equal to: 1000\n");
}

// if( 10 == 15 ) {
//  x = 5 + 10
// } else {
// }
// z = 500 + 500
TEST_F(CodeGenTest, GoingToAnEmptyFalseBranchStillExitsCorrectly) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<EqualToExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(15)),
      std::move(trueblock), Statement::Block())));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(500),
          make_unique<IntegerExpr>(500)))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable z is equal to: 1000\n");
}

// z = 10
// if ( z <= 10 ) {
//  z = 5 + 10
//  if ( z <= 15) {
//    z = 10 + 10
//  } else {
//    z = -10 + -10
//  }
// } else {
//   z = 10 + 10
//   if ( z <= 15) {
//     z = -50 + -50
//   } else {
//     z = -100 + 100
//   }
// }
TEST_F(CodeGenTest, NestedConditionalThatEvalsTrueThenTrue) {
  Statement::Block statements;
  Statement::Block trueouterblock;
  Statement::Block falseouterblock;
  Statement::Block truebranchinnerblock;
  Statement::Block truebranchinnerblock2;
  Statement::Block falsebranchinnerblock;
  Statement::Block falsebranchinnerblock2;

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"), make_unique<IntegerExpr>(10))));

  truebranchinnerblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  truebranchinnerblock2.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(-10),
          make_unique<IntegerExpr>(-10)))));

  falsebranchinnerblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(-50),
          make_unique<IntegerExpr>(-50)))));

  falsebranchinnerblock2.push_back(std::move(
      make_unique<AssignmentFromArithExp>(
          make_unique<VariableExpr>("z"),
          make_unique<AddExpr>(
              make_unique<IntegerExpr>(-100),
              make_unique<IntegerExpr>(100)))));

  trueouterblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  trueouterblock.push_back(std::move(make_unique<const Conditional>(
      make_unique<LessThanEqualToExpr>(
          make_unique<VariableExpr>("z"),
          make_unique<IntegerExpr>(15)),
      std::move(truebranchinnerblock), std::move(truebranchinnerblock2))));

  falseouterblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  falseouterblock.push_back(std::move(make_unique<const Conditional>(
      make_unique<LessThanEqualToExpr>(
          make_unique<VariableExpr>("z"),
          make_unique<IntegerExpr>(15)),
      std::move(falsebranchinnerblock), std::move(falsebranchinnerblock2))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<LessThanEqualToExpr>(
          make_unique<VariableExpr>("z"),
          make_unique<IntegerExpr>(10)),
      std::move(trueouterblock), std::move(falseouterblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable z is equal to: 10\n"
            "Variable z is equal to: 15\n"
            "Variable z is equal to: 20\n");
}

// z = 10
// if ( z <= 10 ) {
//  z = 5 + 10
//  if ( z < 15) {
//    z = 10 + 10
//  } else {
//    z = -10 + -10
//  }
// } else {
//   z = 10 + 10
//   if ( z <= 20) {
//     z = -50 + -50
//   } else {
//     z = -100 + 100
//   }
// }
TEST_F(CodeGenTest, NestedConditionalThatEvalsTrueThenFalse) {
  Statement::Block statements;
  Statement::Block trueouterblock;
  Statement::Block falseouterblock;
  Statement::Block truebranchinnerblock;
  Statement::Block truebranchinnerblock2;
  Statement::Block falsebranchinnerblock;
  Statement::Block falsebranchinnerblock2;

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"), make_unique<IntegerExpr>(10))));

  truebranchinnerblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  truebranchinnerblock2.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(-10),
          make_unique<IntegerExpr>(-10)))));

  falsebranchinnerblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(-50),
          make_unique<IntegerExpr>(-50)))));

  falsebranchinnerblock2.push_back(std::move(
      make_unique<AssignmentFromArithExp>(
          make_unique<VariableExpr>("z"),
          make_unique<AddExpr>(
              make_unique<IntegerExpr>(-100),
              make_unique<IntegerExpr>(100)))));

  trueouterblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  trueouterblock.push_back(std::move(make_unique<const Conditional>(
      make_unique<LessThanExpr>(
          make_unique<VariableExpr>("z"),
          make_unique<IntegerExpr>(15)),
      std::move(truebranchinnerblock), std::move(truebranchinnerblock2))));

  falseouterblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  falseouterblock.push_back(std::move(make_unique<const Conditional>(
      make_unique<LessThanEqualToExpr>(
          make_unique<VariableExpr>("z"),
          make_unique<IntegerExpr>(20)),
      std::move(falsebranchinnerblock), std::move(falsebranchinnerblock2))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<LessThanEqualToExpr>(
          make_unique<VariableExpr>("z"),
          make_unique<IntegerExpr>(10)),
      std::move(trueouterblock), std::move(falseouterblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable z is equal to: 10\n"
            "Variable z is equal to: 15\n"
            "Variable z is equal to: -20\n");
}

// z = 10
// if ( z < 10 ) {
//  z = 5 + 10
//  if ( z <= 15) {
//    z = 10 + 10
//  } else {
//    z = -10 + -10
//  }
// } else {
//   z = 10 + 10
//   if ( z <= 20) {
//     z = -50 + -50
//   } else {
//     z = -100 + 100
//   }
// }
TEST_F(CodeGenTest, NestedConditionalThatEvalsFalseThenTrue) {
  Statement::Block statements;
  Statement::Block trueouterblock;
  Statement::Block falseouterblock;
  Statement::Block truebranchinnerblock;
  Statement::Block truebranchinnerblock2;
  Statement::Block falsebranchinnerblock;
  Statement::Block falsebranchinnerblock2;

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"), make_unique<IntegerExpr>(10))));

  truebranchinnerblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  truebranchinnerblock2.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(-10),
          make_unique<IntegerExpr>(-10)))));

  falsebranchinnerblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(-50),
          make_unique<IntegerExpr>(-50)))));

  falsebranchinnerblock2.push_back(std::move(
      make_unique<AssignmentFromArithExp>(
          make_unique<VariableExpr>("z"),
          make_unique<AddExpr>(
              make_unique<IntegerExpr>(-100),
              make_unique<IntegerExpr>(100)))));

  trueouterblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  trueouterblock.push_back(std::move(make_unique<const Conditional>(
      make_unique<LessThanEqualToExpr>(
          make_unique<VariableExpr>("z"),
          make_unique<IntegerExpr>(15)),
      std::move(truebranchinnerblock), std::move(truebranchinnerblock2))));

  falseouterblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  falseouterblock.push_back(std::move(make_unique<const Conditional>(
      make_unique<LessThanEqualToExpr>(
          make_unique<VariableExpr>("z"),
          make_unique<IntegerExpr>(20)),
      std::move(falsebranchinnerblock), std::move(falsebranchinnerblock2))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<LessThanExpr>(
          make_unique<VariableExpr>("z"),
          make_unique<IntegerExpr>(10)),
      std::move(trueouterblock), std::move(falseouterblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable z is equal to: 10\n"
            "Variable z is equal to: 20\n"
            "Variable z is equal to: -100\n");
}

// z = 10
// if ( z < 10 ) {
//  z = 5 + 10
//  if ( z <= 15) {
//    z = 10 + 10
//  } else {
//    z = -10 + -10
//  }
// } else {
//   z = 10 + 10
//   if ( z < 20) {
//     z = -50 + -50
//   } else {
//     z = -100 + 100
//   }
// }
TEST_F(CodeGenTest, NestedConditionalThatEvalsFalseThenFalse) {
  Statement::Block statements;
  Statement::Block trueouterblock;
  Statement::Block falseouterblock;
  Statement::Block truebranchinnerblock;
  Statement::Block truebranchinnerblock2;
  Statement::Block falsebranchinnerblock;
  Statement::Block falsebranchinnerblock2;

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"), make_unique<IntegerExpr>(10))));

  truebranchinnerblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  truebranchinnerblock2.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(-10),
          make_unique<IntegerExpr>(-10)))));

  falsebranchinnerblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(-50),
          make_unique<IntegerExpr>(-50)))));

  falsebranchinnerblock2.push_back(std::move(
      make_unique<AssignmentFromArithExp>(
          make_unique<VariableExpr>("z"),
          make_unique<AddExpr>(
              make_unique<IntegerExpr>(-100),
              make_unique<IntegerExpr>(100)))));

  trueouterblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  trueouterblock.push_back(std::move(make_unique<const Conditional>(
      make_unique<LessThanEqualToExpr>(
          make_unique<VariableExpr>("z"),
          make_unique<IntegerExpr>(15)),
      std::move(truebranchinnerblock), std::move(truebranchinnerblock2))));

  falseouterblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  falseouterblock.push_back(std::move(make_unique<const Conditional>(
      make_unique<LessThanExpr>(
          make_unique<VariableExpr>("z"),
          make_unique<IntegerExpr>(20)),
      std::move(falsebranchinnerblock), std::move(falsebranchinnerblock2))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<LessThanExpr>(
          make_unique<VariableExpr>("z"),
          make_unique<IntegerExpr>(10)),
      std::move(trueouterblock), std::move(falseouterblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable z is equal to: 10\n"
            "Variable z is equal to: 20\n"
            "Variable z is equal to: 0\n");
}


// x = 0
// while(x < 5) {
//   x = x + 1
// }
TEST_F(CodeGenTest, SimpleLoopThatIsEntered) {
  Statement::Block loop_body;

  auto assign = make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<IntegerExpr>(0));

  auto loopbody = make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<VariableExpr>("x"),
          make_unique<IntegerExpr>(1)));

  loop_body.push_back(std::move(loopbody));

  auto ast = make_unique<const Loop>(
      make_unique<LessThanExpr>(
          make_unique<VariableExpr>("x"),
          make_unique<IntegerExpr>(5)),
      std::move(loop_body));

  assign->Visit(&lowerer_);
  ast->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable x is equal to: 0\n"
            "Variable x is equal to: 1\n"
            "Variable x is equal to: 2\n"
            "Variable x is equal to: 3\n"
            "Variable x is equal to: 4\n"
            "Variable x is equal to: 5\n");
}

// x = 0
// while(x < 0) {
//   x = x + 1
// }
TEST_F(CodeGenTest, SimpleLoopThatIsNeverEntered) {
  Statement::Block loop_body;

  auto assign = make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<IntegerExpr>(0));

  auto loopbody = make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<VariableExpr>("x"),
          make_unique<IntegerExpr>(1)));

  loop_body.push_back(std::move(loopbody));

  auto ast = make_unique<const Loop>(
      make_unique<LessThanExpr>(
          make_unique<VariableExpr>("x"),
          make_unique<IntegerExpr>(0)),
      std::move(loop_body));

  assign->Visit(&lowerer_);
  ast->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable x is equal to: 0\n");
}

// x = 0
// while (x < 5 && !((5 < 10 && -10 > 10) || (-500 == 500 || -10000 >= 10000)) {
//   x = x + 1
// }
TEST_F(CodeGenTest, LoopWithMoreComplicatedGuard) {
  Statement::Block loop_body;

  auto assign = make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<IntegerExpr>(0));

  auto loopbody = make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<VariableExpr>("x"),
          make_unique<IntegerExpr>(1)));

  loop_body.push_back(std::move(loopbody));

  auto ast = make_unique<const Loop>(
      make_unique<LogicalAndExpr>(
          make_unique<LessThanExpr>(
              make_unique<VariableExpr>("x"),
              make_unique<IntegerExpr>(5)),
          make_unique<LogicalNotExpr>(
              make_unique<LogicalOrExpr>(
                  make_unique<LogicalAndExpr>(
                      make_unique<LessThanExpr>(
                          make_unique<IntegerExpr>(5),
                          make_unique<IntegerExpr>(10)),
                      make_unique<GreaterThanExpr>(
                          make_unique<IntegerExpr>(-10),
                          make_unique<IntegerExpr>(10))),
                  make_unique<LogicalOrExpr>(
                      make_unique<EqualToExpr>(
                          make_unique<IntegerExpr>(-500),
                          make_unique<IntegerExpr>(500)),
                      make_unique<GreaterThanEqualToExpr>(
                          make_unique<IntegerExpr>(-10000),
                          make_unique<IntegerExpr>(100000)))))),
      std::move(loop_body));

  assign->Visit(&lowerer_);
  ast->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable x is equal to: 0\n"
            "Variable x is equal to: 1\n"
            "Variable x is equal to: 2\n"
            "Variable x is equal to: 3\n"
            "Variable x is equal to: 4\n"
            "Variable x is equal to: 5\n");
}

// x = 0
// y = 0
// while (x < 3) {
//   while (y < 4) {
//      y = y + 2
//   }
//   x = x + 1
//   y = 0
// }
TEST_F(CodeGenTest, NestedLoop) {
  Statement::Block loop_body;
  Statement::Block nested_loop_body;

  auto assign = make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<IntegerExpr>(0));

  auto assign1 = make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<IntegerExpr>(0));

  auto nestedloopbody = make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<VariableExpr>("y"),
          make_unique<IntegerExpr>(2)));

  nested_loop_body.push_back(std::move(nestedloopbody));

  auto loop = make_unique<const Loop>(
    make_unique<LessThanExpr>(
        make_unique<VariableExpr>("y"),
        make_unique<IntegerExpr>(4)),
    std::move(nested_loop_body));

  auto loopbody = make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<VariableExpr>("x"),
          make_unique<IntegerExpr>(1)));

  auto loopbody2 = make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<IntegerExpr>(0));

  loop_body.push_back(std::move(loop));
  loop_body.push_back(std::move(loopbody));
  loop_body.push_back(std::move(loopbody2));

  auto ast = make_unique<const Loop>(
      make_unique<LessThanExpr>(
          make_unique<VariableExpr>("x"),
          make_unique<IntegerExpr>(3)),
      std::move(loop_body));

  assign->Visit(&lowerer_);
  assign1->Visit(&lowerer_);
  ast->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable x is equal to: 0\n"
                    "Variable y is equal to: 0\n"
                    "Variable y is equal to: 2\n"
                    "Variable y is equal to: 4\n"
                    "Variable x is equal to: 1\n"
                    "Variable y is equal to: 0\n"
                    "Variable y is equal to: 2\n"
                    "Variable y is equal to: 4\n"
                    "Variable x is equal to: 2\n"
                    "Variable y is equal to: 0\n"
                    "Variable y is equal to: 2\n"
                    "Variable y is equal to: 4\n"
                    "Variable x is equal to: 3\n"
                    "Variable y is equal to: 0\n");
}

// x = 0
// y = 1
// while(x < 6) {
//   x = x + 1
//   if (0 < y) {
//     y = -1
//   } else {
//     y = 1
//   }
// }
TEST_F(CodeGenTest, LoopWithIfStatement) {
  Statement::Block loop_body;

  auto assign = make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<IntegerExpr>(0));

  auto assign2 = make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<IntegerExpr>(1));

  auto loopbody = make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<VariableExpr>("x"),
          make_unique<IntegerExpr>(1)));

  loop_body.push_back(std::move(loopbody));

  Statement::Block trueblock;
  Statement::Block falseblock;

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<IntegerExpr>(-1))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<IntegerExpr>(1))));

  loop_body.push_back(std::move(make_unique<const Conditional>(
      make_unique<LessThanExpr>(
          make_unique<IntegerExpr>(0),
          make_unique<VariableExpr>("y")),
      std::move(trueblock), std::move(falseblock))));

  auto ast = make_unique<const Loop>(
      make_unique<LessThanExpr>(
          make_unique<VariableExpr>("x"),
          make_unique<IntegerExpr>(5)),
      std::move(loop_body));

  assign->Visit(&lowerer_);
  assign2->Visit(&lowerer_);
  ast->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable x is equal to: 0\n"
            "Variable y is equal to: 1\n"
            "Variable x is equal to: 1\n"
            "Variable y is equal to: -1\n"
            "Variable x is equal to: 2\n"
            "Variable y is equal to: 1\n"
            "Variable x is equal to: 3\n"
            "Variable y is equal to: -1\n"
            "Variable x is equal to: 4\n"
            "Variable y is equal to: 1\n"
            "Variable x is equal to: 5\n"
            "Variable y is equal to: -1\n");
}

// V4 Tests

// int foo() {
//   return 5 + 10
// }
//
// foo_retval = foo()
TEST_F(CodeGenTest, BasicFunctionCallWithNoArguments) {
  Statement::Block foo_statements;

  auto foo_retval = make_unique<const AddExpr>(
      make_unique<const IntegerExpr>(5),
      make_unique<const IntegerExpr>(10));

  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();

  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();

  Statement::Block statements;
  statements.push_back(std::move(make_unique<const FunctionCall>(
      make_unique<const VariableExpr>("foo_retval"), "foo",
      std::move(arguments))));

  auto ae = make_unique<const VariableExpr>("foo_retval");

  auto ast = make_unique<const Program>(std::move(function_defs),
                                        std::move(statements), std::move(ae));

  ast->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The function returned: 15\n"
            "The program returned: 15\n");
}

// def foo(a,b,c,d,e,f,g,h,j,k) {
//    x = a + b + c + d + e + f + g + h+ j + k
//    return x
// }
// x = 10
// foo_retval = foo(1,2,3,4,5,6,7,8,9,x)
TEST_F(CodeGenTest, BasicFunctionCallWithManyArguments) {
  Statement::Block foo_statements;

  foo_statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<AddExpr>(
              make_unique<AddExpr>(
                  make_unique<VariableExpr>("a"),
                  make_unique<VariableExpr>("b")),
              make_unique<AddExpr>(
                  make_unique<VariableExpr>("c"),
                  make_unique<VariableExpr>("d"))),
          make_unique<AddExpr>(
              make_unique<AddExpr>(
                  make_unique<VariableExpr>("e"),
                  make_unique<VariableExpr>("f")),
              make_unique<AddExpr>(
                  make_unique<AddExpr>(
                      make_unique<VariableExpr>("g"),
                      make_unique<VariableExpr>("h")),
                  make_unique<AddExpr>(
                      make_unique<VariableExpr>("j"),
                      make_unique<VariableExpr>("k"))))))));
  auto foo_retval = make_unique<const VariableExpr>("x");

  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();
  foo_params.push_back(std::move(make_unique<const VariableExpr>("a")));
  foo_params.push_back(std::move(make_unique<const VariableExpr>("b")));
  foo_params.push_back(std::move(make_unique<const VariableExpr>("c")));
  foo_params.push_back(std::move(make_unique<const VariableExpr>("d")));
  foo_params.push_back(std::move(make_unique<const VariableExpr>("e")));
  foo_params.push_back(std::move(make_unique<const VariableExpr>("f")));
  foo_params.push_back(std::move(make_unique<const VariableExpr>("g")));
  foo_params.push_back(std::move(make_unique<const VariableExpr>("h")));
  foo_params.push_back(std::move(make_unique<const VariableExpr>("j")));
  foo_params.push_back(std::move(make_unique<const VariableExpr>("k")));

  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();
  arguments.push_back(std::move(make_unique<const IntegerExpr>(1)));
  arguments.push_back(std::move(make_unique<const IntegerExpr>(2)));
  arguments.push_back(std::move(make_unique<const IntegerExpr>(3)));
  arguments.push_back(std::move(make_unique<const IntegerExpr>(4)));
  arguments.push_back(std::move(make_unique<const IntegerExpr>(5)));
  arguments.push_back(std::move(make_unique<const IntegerExpr>(6)));
  arguments.push_back(std::move(make_unique<const IntegerExpr>(7)));
  arguments.push_back(std::move(make_unique<const IntegerExpr>(8)));
  arguments.push_back(std::move(make_unique<const IntegerExpr>(9)));
  arguments.push_back(std::move(make_unique<const VariableExpr>("x")));

  Statement::Block statements;

  statements.push_back(std::move(
      make_unique<const AssignmentFromArithExp>(
          make_unique<VariableExpr>("x"),
          make_unique<IntegerExpr>(10))));
  statements.push_back(std::move(make_unique<const FunctionCall>(
      make_unique<const VariableExpr>("foo_retval"), "foo",
      std::move(arguments))));

  auto ae = make_unique<const VariableExpr>("foo_retval");

  auto ast = make_unique<const Program>(std::move(function_defs),
                                        std::move(statements), std::move(ae));

  ast->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable x is equal to: 10\n"
            "The function returned: 55\n"
            "The program returned: 55\n");
}

// def foo(a,b,c,d,e,f,g,h,j,k) {
//    x = a + b + c + d + e + f + g + h+ j + k
//    return x
// }
// x = 10
// foo_retval = foo(1,2,3,4,5,6,7,8,9,x)
TEST_F(CodeGenTest, BasicFunctionCallWithMoreComplexArithExprAsArguments) {
  Statement::Block foo_statements;

  foo_statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<AddExpr>(
              make_unique<AddExpr>(
                  make_unique<VariableExpr>("a"),
                  make_unique<VariableExpr>("b")),
              make_unique<AddExpr>(
                  make_unique<VariableExpr>("c"),
                  make_unique<VariableExpr>("d"))),
          make_unique<AddExpr>(
              make_unique<AddExpr>(
                  make_unique<VariableExpr>("e"),
                  make_unique<VariableExpr>("f")),
              make_unique<AddExpr>(
                  make_unique<AddExpr>(
                      make_unique<VariableExpr>("g"),
                      make_unique<VariableExpr>("h")),
                  make_unique<AddExpr>(
                      make_unique<VariableExpr>("j"),
                      make_unique<VariableExpr>("k"))))))));
  auto foo_retval = make_unique<const VariableExpr>("x");

  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();
  foo_params.push_back(std::move(make_unique<const VariableExpr>("a")));
  foo_params.push_back(std::move(make_unique<const VariableExpr>("b")));
  foo_params.push_back(std::move(make_unique<const VariableExpr>("c")));
  foo_params.push_back(std::move(make_unique<const VariableExpr>("d")));
  foo_params.push_back(std::move(make_unique<const VariableExpr>("e")));
  foo_params.push_back(std::move(make_unique<const VariableExpr>("f")));
  foo_params.push_back(std::move(make_unique<const VariableExpr>("g")));
  foo_params.push_back(std::move(make_unique<const VariableExpr>("h")));
  foo_params.push_back(std::move(make_unique<const VariableExpr>("j")));
  foo_params.push_back(std::move(make_unique<const VariableExpr>("k")));

  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();
  arguments.push_back(std::move(make_unique<const AddExpr>(
      make_unique<IntegerExpr>(-10),
      make_unique<IntegerExpr>(11))));
  arguments.push_back(std::move(make_unique<const SubtractExpr>(
      make_unique<IntegerExpr>(25),
      make_unique<IntegerExpr>(23))));
  arguments.push_back(std::move(make_unique<const MultiplyExpr>(
      make_unique<IntegerExpr>(-3),
      make_unique<IntegerExpr>(-1))));
  arguments.push_back(std::move(make_unique<const DivideExpr>(
      make_unique<IntegerExpr>(120),
      make_unique<IntegerExpr>(25))));
  arguments.push_back(std::move(make_unique<DivideExpr>(
      make_unique<AddExpr>(make_unique<IntegerExpr>(10),
                           make_unique<IntegerExpr>(5)),
      make_unique<SubtractExpr>(make_unique<IntegerExpr>(4),
                                make_unique<IntegerExpr>(1)))));
  arguments.push_back(std::move(make_unique<const AddExpr>(
      make_unique<VariableExpr>("x"),
      make_unique<IntegerExpr>(-4))));
  arguments.push_back(std::move(make_unique<const SubtractExpr>(
      make_unique<VariableExpr>("x"),
      make_unique<IntegerExpr>(3))));
  arguments.push_back(std::move(make_unique<DivideExpr>(
      make_unique<AddExpr>(make_unique<VariableExpr>("x"),
                           make_unique<IntegerExpr>(14)),
      make_unique<SubtractExpr>(make_unique<IntegerExpr>(4),
                                make_unique<IntegerExpr>(1)))));
  arguments.push_back(std::move(make_unique<const IntegerExpr>(9)));
  arguments.push_back(std::move(make_unique<const VariableExpr>("x")));

  Statement::Block statements;

  statements.push_back(std::move(
      make_unique<const AssignmentFromArithExp>(
          make_unique<VariableExpr>("x"),
          make_unique<IntegerExpr>(10))));
  statements.push_back(std::move(make_unique<const FunctionCall>(
      make_unique<const VariableExpr>("foo_retval"), "foo",
      std::move(arguments))));

  auto ae = make_unique<const VariableExpr>("foo_retval");

  auto ast = make_unique<const Program>(std::move(function_defs),
                                        std::move(statements), std::move(ae));

  ast->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable x is equal to: 10\n"
            "The function returned: 55\n"
            "The program returned: 55\n");
}

// int foo() {
//   x = 10 + 10
//   return x
// }
//
// foo_retval = foo()
TEST_F(CodeGenTest, AssignmentsWithAdditionWorksInFunctions) {
  Statement::Block foo_statements;

  foo_statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  auto foo_retval = make_unique<const VariableExpr>("x");

  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();

  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();

  Statement::Block statements;

  statements.push_back(std::move(make_unique<const FunctionCall>(
      make_unique<const VariableExpr>("foo_retval"), "foo",
      std::move(arguments))));

  auto ae = make_unique<const VariableExpr>("foo_retval");

  auto ast = make_unique<const Program>(std::move(function_defs),
                                        std::move(statements), std::move(ae));

  ast->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The function returned: 20\n"
            "The program returned: 20\n");
}

// int foo() {
//   x = 20 - 10
//   return x
// }
//
// foo_retval = foo()
TEST_F(CodeGenTest, AssignmentsWithSubtractionWorksInFunctions) {
  Statement::Block foo_statements;

  foo_statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<SubtractExpr>(
          make_unique<IntegerExpr>(20),
          make_unique<IntegerExpr>(10)))));

  auto foo_retval = make_unique<const VariableExpr>("x");

  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();

  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();

  Statement::Block statements;

  statements.push_back(std::move(make_unique<const FunctionCall>(
      make_unique<const VariableExpr>("foo_retval"), "foo",
      std::move(arguments))));

  auto ae = make_unique<const VariableExpr>("foo_retval");

  auto ast = make_unique<const Program>(std::move(function_defs),
                                        std::move(statements), std::move(ae));

  ast->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The function returned: 10\n"
            "The program returned: 10\n");
}

// int foo() {
//   x = 20 * 10
//   return x
// }
//
// foo_retval = foo()
TEST_F(CodeGenTest, AssignmentsWithMultiplicationWorksInFunctions) {
  Statement::Block foo_statements;

  foo_statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<MultiplyExpr>(
          make_unique<IntegerExpr>(20),
          make_unique<IntegerExpr>(10)))));

  auto foo_retval = make_unique<const VariableExpr>("x");

  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();

  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();

  Statement::Block statements;

  statements.push_back(std::move(make_unique<const FunctionCall>(
      make_unique<const VariableExpr>("foo_retval"), "foo",
      std::move(arguments))));

  auto ae = make_unique<const VariableExpr>("foo_retval");

  auto ast = make_unique<const Program>(std::move(function_defs),
                                        std::move(statements), std::move(ae));

  ast->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The function returned: 200\n"
            "The program returned: 200\n");
}

// int foo() {
//   x = 20 / 10
//   return x
// }
//
// foo_retval = foo()
TEST_F(CodeGenTest, AssignmentsWithDivisionWorksInFunctions) {
  Statement::Block foo_statements;

  foo_statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<DivideExpr>(
          make_unique<IntegerExpr>(20),
          make_unique<IntegerExpr>(10)))));

  auto foo_retval = make_unique<const VariableExpr>("x");

  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();

  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();

  Statement::Block statements;

  statements.push_back(std::move(make_unique<const FunctionCall>(
      make_unique<const VariableExpr>("foo_retval"), "foo",
      std::move(arguments))));

  auto ae = make_unique<const VariableExpr>("foo_retval");

  auto ast = make_unique<const Program>(std::move(function_defs),
                                        std::move(statements), std::move(ae));

  ast->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The function returned: 2\n"
            "The program returned: 2\n");
}

// int foo() {
//   x = 20 / 10
//   return x
// }
//
// foo_retval = foo()
TEST_F(CodeGenTest, AssignmentWithDivisionByZeroErrorsOutCorrectlyInFunctions) {
  Statement::Block foo_statements;

  foo_statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<DivideExpr>(
          make_unique<IntegerExpr>(20),
          make_unique<IntegerExpr>(0)))));

  auto foo_retval = make_unique<const VariableExpr>("x");

  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();

  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();

  Statement::Block statements;

  statements.push_back(std::move(make_unique<const FunctionCall>(
      make_unique<const VariableExpr>("foo_retval"), "foo",
      std::move(arguments))));

  auto ae = make_unique<const VariableExpr>("foo_retval");

  auto ast = make_unique<const Program>(std::move(function_defs),
                                        std::move(statements), std::move(ae));

  ast->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Error: Division by zero\n");
}

// int foo() {
//  if( !((5 < 10 && -10 > 10) || (-500 == 500 || -10000 >= 10000)) {
//    x = 5 + 10
//  } else {
//    x = 10 + 10
//  }
//   return x
// }
//
// foo_retval = foo()
TEST_F(CodeGenTest, ConditionalThatEvalsToTrueWorksInFunctions) {
  Statement::Block foo_statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  foo_statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<LogicalNotExpr>(
          make_unique<LogicalOrExpr>(
              make_unique<LogicalAndExpr>(
                  make_unique<LessThanExpr>(
                      make_unique<IntegerExpr>(5),
                      make_unique<IntegerExpr>(10)),
                  make_unique<GreaterThanExpr>(
                      make_unique<IntegerExpr>(-10),
                      make_unique<IntegerExpr>(10))),
              make_unique<LogicalOrExpr>(
                  make_unique<EqualToExpr>(
                      make_unique<IntegerExpr>(-500),
                      make_unique<IntegerExpr>(500)),
                  make_unique<GreaterThanEqualToExpr>(
                      make_unique<IntegerExpr>(-10000),
                      make_unique<IntegerExpr>(100000))))),
      std::move(trueblock), std::move(falseblock))));

  auto foo_retval = make_unique<const VariableExpr>("x");

  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();

  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();

  Statement::Block statements;

  statements.push_back(std::move(make_unique<const FunctionCall>(
      make_unique<const VariableExpr>("foo_retval"), "foo",
      std::move(arguments))));

  auto ae = make_unique<const VariableExpr>("foo_retval");

  auto ast = make_unique<const Program>(std::move(function_defs),
                                        std::move(statements), std::move(ae));

  ast->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The function returned: 15\n"
           "The program returned: 15\n");
}

// int foo() {
//  if( !((5 < 10 && -10 > 10) || (500 == 500 || -10000 >= 10000)) {
//    x = 5 + 10
//  } else {
//    x = 10 + 10
//  }
//   return x
// }
//
// foo_retval = foo()
TEST_F(CodeGenTest, ConditionalThatEvalsToFalseWorksInFunctions) {
  Statement::Block foo_statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  foo_statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<LogicalNotExpr>(
          make_unique<LogicalOrExpr>(
              make_unique<LogicalAndExpr>(
                  make_unique<LessThanExpr>(
                      make_unique<IntegerExpr>(5),
                      make_unique<IntegerExpr>(10)),
                  make_unique<GreaterThanExpr>(
                      make_unique<IntegerExpr>(-10),
                      make_unique<IntegerExpr>(10))),
              make_unique<LogicalOrExpr>(
                  make_unique<EqualToExpr>(
                      make_unique<IntegerExpr>(500),
                      make_unique<IntegerExpr>(500)),
                  make_unique<GreaterThanEqualToExpr>(
                      make_unique<IntegerExpr>(-10000),
                      make_unique<IntegerExpr>(100000))))),
      std::move(trueblock), std::move(falseblock))));

  auto foo_retval = make_unique<const VariableExpr>("x");

  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();

  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();

  Statement::Block statements;

  statements.push_back(std::move(make_unique<const FunctionCall>(
      make_unique<const VariableExpr>("foo_retval"), "foo",
      std::move(arguments))));

  auto ae = make_unique<const VariableExpr>("foo_retval");

  auto ast = make_unique<const Program>(std::move(function_defs),
                                        std::move(statements), std::move(ae));

  ast->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The function returned: 20\n"
           "The program returned: 20\n");
}

// int foo() {
//  z = 10
//  if ( z <= 10 ) {
//    z = 5 + 10
//    if ( z <= 15) {
//      z = 10 + 10
//    } else {
//      z = -10 + -10
//    }
//  } else {
//    z = 10 + 10
//    if ( z <= 15) {
//      z = -50 + -50
//    } else {
//      z = -100 + 100
//    }
//  }
//  return z
// }
//
// foo_retval = foo()
TEST_F(CodeGenTest, NestedConditionalThatEvalsTrueThenTrueFunction) {
  Statement::Block foo_statements;
  Statement::Block trueouterblock;
  Statement::Block falseouterblock;
  Statement::Block truebranchinnerblock;
  Statement::Block truebranchinnerblock2;
  Statement::Block falsebranchinnerblock;
  Statement::Block falsebranchinnerblock2;

  foo_statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"), make_unique<IntegerExpr>(10))));

  truebranchinnerblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  truebranchinnerblock2.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(-10),
          make_unique<IntegerExpr>(-10)))));

  falsebranchinnerblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(-50),
          make_unique<IntegerExpr>(-50)))));

  falsebranchinnerblock2.push_back(std::move(
      make_unique<AssignmentFromArithExp>(
          make_unique<VariableExpr>("z"),
          make_unique<AddExpr>(
              make_unique<IntegerExpr>(-100),
              make_unique<IntegerExpr>(100)))));

  trueouterblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  trueouterblock.push_back(std::move(make_unique<const Conditional>(
      make_unique<LessThanEqualToExpr>(
          make_unique<VariableExpr>("z"),
          make_unique<IntegerExpr>(15)),
      std::move(truebranchinnerblock), std::move(truebranchinnerblock2))));

  falseouterblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  falseouterblock.push_back(std::move(make_unique<const Conditional>(
      make_unique<LessThanEqualToExpr>(
          make_unique<VariableExpr>("z"),
          make_unique<IntegerExpr>(15)),
      std::move(falsebranchinnerblock), std::move(falsebranchinnerblock2))));

  foo_statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<LessThanEqualToExpr>(
          make_unique<VariableExpr>("z"),
          make_unique<IntegerExpr>(10)),
      std::move(trueouterblock), std::move(falseouterblock))));

  auto foo_retval = make_unique<const VariableExpr>("z");

  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();

  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();

  Statement::Block statements;

  statements.push_back(std::move(make_unique<const FunctionCall>(
      make_unique<const VariableExpr>("foo_retval"), "foo",
      std::move(arguments))));

  auto ae = make_unique<const VariableExpr>("foo_retval");

  auto ast = make_unique<const Program>(std::move(function_defs),
                                        std::move(statements), std::move(ae));

  ast->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The function returned: 20\n"
           "The program returned: 20\n");
}

// int foo() {
//  z = 10
//  if ( z <= 10 ) {
//    z = 5 + 10
//    if ( z < 15) {
//      z = 10 + 10
//    } else {
//      z = -10 + -10
//    }
//  } else {
//    z = 10 + 10
//    if ( z <= 15) {
//      z = -50 + -50
//    } else {
//      z = -100 + 100
//    }
//  }
//  return z
// }
//
// foo_retval = foo()
TEST_F(CodeGenTest, NestedConditionalThatEvalsTrueThenFalseFunction) {
  Statement::Block foo_statements;
  Statement::Block trueouterblock;
  Statement::Block falseouterblock;
  Statement::Block truebranchinnerblock;
  Statement::Block truebranchinnerblock2;
  Statement::Block falsebranchinnerblock;
  Statement::Block falsebranchinnerblock2;

  foo_statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"), make_unique<IntegerExpr>(10))));

  truebranchinnerblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  truebranchinnerblock2.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(-10),
          make_unique<IntegerExpr>(-10)))));

  falsebranchinnerblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(-50),
          make_unique<IntegerExpr>(-50)))));

  falsebranchinnerblock2.push_back(std::move(
      make_unique<AssignmentFromArithExp>(
          make_unique<VariableExpr>("z"),
          make_unique<AddExpr>(
              make_unique<IntegerExpr>(-100),
              make_unique<IntegerExpr>(100)))));

  trueouterblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  trueouterblock.push_back(std::move(make_unique<const Conditional>(
      make_unique<LessThanExpr>(
          make_unique<VariableExpr>("z"),
          make_unique<IntegerExpr>(15)),
      std::move(truebranchinnerblock), std::move(truebranchinnerblock2))));

  falseouterblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  falseouterblock.push_back(std::move(make_unique<const Conditional>(
      make_unique<LessThanEqualToExpr>(
          make_unique<VariableExpr>("z"),
          make_unique<IntegerExpr>(15)),
      std::move(falsebranchinnerblock), std::move(falsebranchinnerblock2))));

  foo_statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<LessThanEqualToExpr>(
          make_unique<VariableExpr>("z"),
          make_unique<IntegerExpr>(10)),
      std::move(trueouterblock), std::move(falseouterblock))));

  auto foo_retval = make_unique<const VariableExpr>("z");

  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();

  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();

  Statement::Block statements;

  statements.push_back(std::move(make_unique<const FunctionCall>(
      make_unique<const VariableExpr>("foo_retval"), "foo",
      std::move(arguments))));

  auto ae = make_unique<const VariableExpr>("foo_retval");

  auto ast = make_unique<const Program>(std::move(function_defs),
                                        std::move(statements), std::move(ae));

  ast->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The function returned: -20\n"
           "The program returned: -20\n");
}

// int foo() {
//  z = 15
//  if ( z <= 10 ) {
//    z = 5 + 10
//    if ( z <= 15) {
//      z = 10 + 10
//    } else {
//      z = -10 + -10
//    }
//  } else {
//    z = 10 + 10
//    if ( z <= 20) {
//      z = -50 + -50
//    } else {
//      z = -100 + 100
//    }
//  }
//  return z
// }
//
// foo_retval = foo()
TEST_F(CodeGenTest, NestedConditionalThatEvalsFalseThenTrueFunction) {
  Statement::Block foo_statements;
  Statement::Block trueouterblock;
  Statement::Block falseouterblock;
  Statement::Block truebranchinnerblock;
  Statement::Block truebranchinnerblock2;
  Statement::Block falsebranchinnerblock;
  Statement::Block falsebranchinnerblock2;

  foo_statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"), make_unique<IntegerExpr>(15))));

  truebranchinnerblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  truebranchinnerblock2.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(-10),
          make_unique<IntegerExpr>(-10)))));

  falsebranchinnerblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(-50),
          make_unique<IntegerExpr>(-50)))));

  falsebranchinnerblock2.push_back(std::move(
      make_unique<AssignmentFromArithExp>(
          make_unique<VariableExpr>("z"),
          make_unique<AddExpr>(
              make_unique<IntegerExpr>(-100),
              make_unique<IntegerExpr>(100)))));

  trueouterblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  trueouterblock.push_back(std::move(make_unique<const Conditional>(
      make_unique<LessThanEqualToExpr>(
          make_unique<VariableExpr>("z"),
          make_unique<IntegerExpr>(15)),
      std::move(truebranchinnerblock), std::move(truebranchinnerblock2))));

  falseouterblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  falseouterblock.push_back(std::move(make_unique<const Conditional>(
      make_unique<LessThanEqualToExpr>(
          make_unique<VariableExpr>("z"),
          make_unique<IntegerExpr>(20)),
      std::move(falsebranchinnerblock), std::move(falsebranchinnerblock2))));

  foo_statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<LessThanEqualToExpr>(
          make_unique<VariableExpr>("z"),
          make_unique<IntegerExpr>(10)),
      std::move(trueouterblock), std::move(falseouterblock))));

  auto foo_retval = make_unique<const VariableExpr>("z");

  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();

  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();

  Statement::Block statements;

  statements.push_back(std::move(make_unique<const FunctionCall>(
      make_unique<const VariableExpr>("foo_retval"), "foo",
      std::move(arguments))));

  auto ae = make_unique<const VariableExpr>("foo_retval");

  auto ast = make_unique<const Program>(std::move(function_defs),
                                        std::move(statements), std::move(ae));

  ast->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The function returned: -100\n"
           "The program returned: -100\n");
}

// int foo() {
//  z = 15
//  if ( z <= 10 ) {
//    z = 5 + 10
//    if ( z <= 15) {
//      z = 10 + 10
//    } else {
//      z = -10 + -10
//    }
//  } else {
//    z = 10 + 10
//    if ( z < 20) {
//      z = -50 + -50
//    } else {
//      z = -100 + -100
//    }
//  }
//  return z
// }
//
// foo_retval = foo()
TEST_F(CodeGenTest, NestedConditionalThatEvalsFalseThenFalseFunction) {
  Statement::Block foo_statements;
  Statement::Block trueouterblock;
  Statement::Block falseouterblock;
  Statement::Block truebranchinnerblock;
  Statement::Block truebranchinnerblock2;
  Statement::Block falsebranchinnerblock;
  Statement::Block falsebranchinnerblock2;

  foo_statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"), make_unique<IntegerExpr>(15))));

  truebranchinnerblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  truebranchinnerblock2.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(-10),
          make_unique<IntegerExpr>(-10)))));

  falsebranchinnerblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(-50),
          make_unique<IntegerExpr>(-50)))));

  falsebranchinnerblock2.push_back(std::move(
      make_unique<AssignmentFromArithExp>(
          make_unique<VariableExpr>("z"),
          make_unique<AddExpr>(
              make_unique<IntegerExpr>(-100),
              make_unique<IntegerExpr>(-100)))));

  trueouterblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  trueouterblock.push_back(std::move(make_unique<const Conditional>(
      make_unique<LessThanEqualToExpr>(
          make_unique<VariableExpr>("z"),
          make_unique<IntegerExpr>(15)),
      std::move(truebranchinnerblock), std::move(truebranchinnerblock2))));

  falseouterblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  falseouterblock.push_back(std::move(make_unique<const Conditional>(
      make_unique<LessThanExpr>(
          make_unique<VariableExpr>("z"),
          make_unique<IntegerExpr>(20)),
      std::move(falsebranchinnerblock), std::move(falsebranchinnerblock2))));

  foo_statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<LessThanEqualToExpr>(
          make_unique<VariableExpr>("z"),
          make_unique<IntegerExpr>(10)),
      std::move(trueouterblock), std::move(falseouterblock))));

  auto foo_retval = make_unique<const VariableExpr>("z");

  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();

  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();

  Statement::Block statements;

  statements.push_back(std::move(make_unique<const FunctionCall>(
      make_unique<const VariableExpr>("foo_retval"), "foo",
      std::move(arguments))));

  auto ae = make_unique<const VariableExpr>("foo_retval");

  auto ast = make_unique<const Program>(std::move(function_defs),
                                        std::move(statements), std::move(ae));

  ast->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The function returned: -200\n"
           "The program returned: -200\n");
}

// int foo() {
//  x = 0
//  while(x < 5) {
//    x = x + 1
//  }
//  return x
// }
//
// foo_retval = foo()
TEST_F(CodeGenTest, SimpleLoopThatIsEnteredInFunction) {
  Statement::Block foo_statements;
  Statement::Block loop_body;

  foo_statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<IntegerExpr>(0))));
  auto loopbody = make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<VariableExpr>("x"),
          make_unique<IntegerExpr>(1)));
  loop_body.push_back(std::move(loopbody));

  foo_statements.push_back(std::move(make_unique<const Loop>(
      make_unique<LessThanExpr>(
          make_unique<VariableExpr>("x"),
          make_unique<IntegerExpr>(5)),
      std::move(loop_body))));

  auto foo_retval = make_unique<const VariableExpr>("x");
  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();
  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();
  Statement::Block statements;
  statements.push_back(std::move(make_unique<const FunctionCall>(
      make_unique<const VariableExpr>("foo_retval"), "foo",
      std::move(arguments))));

  auto ae = make_unique<const VariableExpr>("foo_retval");

  auto ast = make_unique<const Program>(std::move(function_defs),
                                        std::move(statements), std::move(ae));

  ast->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The function returned: 5\n"
           "The program returned: 5\n");
}

// int foo() {
//  x = 10
//  while(x < 5) {
//    x = x + 1
//  }
//  return x
// }
//
// foo_retval = foo()
TEST_F(CodeGenTest, SimpleLoopThatIsNotEnteredInFunction) {
  Statement::Block foo_statements;
  Statement::Block loop_body;

  foo_statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<IntegerExpr>(10))));
  auto loopbody = make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<VariableExpr>("x"),
          make_unique<IntegerExpr>(1)));
  loop_body.push_back(std::move(loopbody));

  foo_statements.push_back(std::move(make_unique<const Loop>(
      make_unique<LessThanExpr>(
          make_unique<VariableExpr>("x"),
          make_unique<IntegerExpr>(5)),
      std::move(loop_body))));

  auto foo_retval = make_unique<const VariableExpr>("x");
  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();
  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();
  Statement::Block statements;
  statements.push_back(std::move(make_unique<const FunctionCall>(
      make_unique<const VariableExpr>("foo_retval"), "foo",
      std::move(arguments))));

  auto ae = make_unique<const VariableExpr>("foo_retval");

  auto ast = make_unique<const Program>(std::move(function_defs),
                                        std::move(statements), std::move(ae));

  ast->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The function returned: 10\n"
           "The program returned: 10\n");
}

// int foo() {
//  x = 0
//  y = 0
//  z = 0
//  while (x < 3) {
//    while (y < 4) {
//        y = y + 2
//        z = z + 1
//    }
//    x = x + 1
//    y = 0
//  }
//  return z
// }
//
// foo_retval = foo()
TEST_F(CodeGenTest, NestedLoopsInAFunction) {
  Statement::Block foo_statements;
  Statement::Block loop_body;
  Statement::Block nested_loop_body;

  foo_statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<IntegerExpr>(0))));

  foo_statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<IntegerExpr>(0))));

  foo_statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"),
      make_unique<IntegerExpr>(0))));

  auto nestedloopbody = make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<VariableExpr>("y"),
          make_unique<IntegerExpr>(2)));
  auto nestedloopbody2 = make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"),
      make_unique<AddExpr>(
          make_unique<VariableExpr>("z"),
          make_unique<IntegerExpr>(1)));
  nested_loop_body.push_back(std::move(nestedloopbody));
  nested_loop_body.push_back(std::move(nestedloopbody2));

  auto loop = make_unique<const Loop>(
    make_unique<LessThanExpr>(
        make_unique<VariableExpr>("y"),
        make_unique<IntegerExpr>(4)),
    std::move(nested_loop_body));

  auto loopbody = make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<VariableExpr>("x"),
          make_unique<IntegerExpr>(1)));

  auto loopbody2 = make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<IntegerExpr>(0));

  loop_body.push_back(std::move(loop));
  loop_body.push_back(std::move(loopbody));
  loop_body.push_back(std::move(loopbody2));

  foo_statements.push_back(std::move(make_unique<const Loop>(
      make_unique<LessThanExpr>(
          make_unique<VariableExpr>("x"),
          make_unique<IntegerExpr>(3)),
      std::move(loop_body))));

  auto foo_retval = make_unique<const VariableExpr>("z");
  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();
  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();
  Statement::Block statements;
  statements.push_back(std::move(make_unique<const FunctionCall>(
      make_unique<const VariableExpr>("foo_retval"), "foo",
      std::move(arguments))));

  auto ae = make_unique<const VariableExpr>("foo_retval");

  auto ast = make_unique<const Program>(std::move(function_defs),
                                        std::move(statements), std::move(ae));

  ast->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The function returned: 6\n"
           "The program returned: 6\n");
}

// def fibo(int x) {
//  if (x == 1 || x == 2)
//    foo_retval = 1
//  else
//    y = fibo(x-1)
//    z = fibo(x-2)
//    foo_retval = y + z
//  return foo_retval
// }
//
// x = 9
// foo_retval = fibo(x)
TEST_F(CodeGenTest, RecursiveFibonacciFunctionWorks) {
  Statement::Block statements;
  statements.push_back(std::move(make_unique<const AssignmentFromArithExp>(
      make_unique<const VariableExpr>("x"),
      make_unique<const IntegerExpr>(9))));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();
  arguments.push_back(std::move(make_unique<const VariableExpr>("x")));

  statements.push_back(std::move(make_unique<const FunctionCall>(
      make_unique<const VariableExpr>("foo_retval"), "fibo",
      std::move(arguments))));

  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();
  foo_params.push_back(std::move(make_unique<const VariableExpr>("x")));

  Statement::Block fibo_body;
  Statement::Block true_fibo;
  Statement::Block false_fibo;

  auto arguments1 = std::vector<std::unique_ptr<const ArithmeticExpr>>();
  arguments1.push_back(std::move(make_unique<const SubtractExpr>(
      make_unique<VariableExpr>("x"),
      make_unique<IntegerExpr>(1))));

  auto arguments2 = std::vector<std::unique_ptr<const ArithmeticExpr>>();
  arguments2.push_back(std::move(make_unique<const SubtractExpr>(
      make_unique<VariableExpr>("x"),
      make_unique<IntegerExpr>(2))));

  true_fibo.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("foo_retval"),
      make_unique<IntegerExpr>(1))));

  false_fibo.push_back(std::move(make_unique<const FunctionCall>(
      make_unique<const VariableExpr>("y"), "fibo",
      std::move(arguments1))));

  false_fibo.push_back(std::move(make_unique<const FunctionCall>(
      make_unique<const VariableExpr>("z"), "fibo",
      std::move(arguments2))));

  false_fibo.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("foo_retval"),
      make_unique<AddExpr>(
          make_unique<VariableExpr>("y"),
          make_unique<VariableExpr>("z")))));

  fibo_body.push_back(std::move(make_unique<Conditional>(
      make_unique<LogicalOrExpr>(
          make_unique<EqualToExpr>(
              make_unique<VariableExpr>("x"),
              make_unique<IntegerExpr>(1)),
          make_unique<EqualToExpr>(
              make_unique<VariableExpr>("x"),
              make_unique<IntegerExpr>(2))),
      std::move(true_fibo), std::move(false_fibo))));

  auto ae = make_unique<const VariableExpr>("foo_retval");
  auto foo_retval = make_unique<const VariableExpr>("foo_retval");
  auto foo_def = make_unique<const FunctionDef>("fibo", std::move(foo_params),
                                                std::move(fibo_body),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto ast = make_unique<const Program>(
      std::move(function_defs),
      std::move(statements), std::move(ae));

  ast->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_ONLY_RESULT);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The program returned: 34\n");
}


// def fact(int bob) {
//  if(bob >1)
//    rob = fact(bob-1)
//    ret_val = rob * bob
//  else
//    ret_val = 1
//  return ret_val
// }
//
// bob = 10
// ret_val = fact(bob)
TEST_F(CodeGenTest, RecursiveFactorialFunctionWorks) {
  Statement::Block statements;
  statements.push_back(std::move(
      make_unique<const AssignmentFromArithExp>(
          make_unique<const VariableExpr>("bob"),
          make_unique<const IntegerExpr>(10))));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();
  arguments.push_back(std::move(make_unique<const VariableExpr>("bob")));

  statements.push_back(std::move(make_unique<const FunctionCall>(
      make_unique<const VariableExpr>("ret_val"), "fact",
      std::move(arguments))));

  auto fact_params = std::vector<std::unique_ptr<const VariableExpr>>();
  fact_params.push_back(std::move(make_unique<const VariableExpr>("bob")));

  Statement::Block fact_body;
  Statement::Block true_fact;
  Statement::Block false_fact;

  auto arguments1 = std::vector<std::unique_ptr<const ArithmeticExpr>>();
  arguments1.push_back(std::move(make_unique<const SubtractExpr>(
      make_unique<VariableExpr>("bob"),
      make_unique<IntegerExpr>(1))));

  false_fact.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("ret_val"),
      make_unique<IntegerExpr>(1))));

  true_fact.push_back(std::move(make_unique<const FunctionCall>(
      make_unique<const VariableExpr>("rob"), "fact",
      std::move(arguments1))));

  true_fact.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("ret_val"),
      make_unique<MultiplyExpr>(
          make_unique<VariableExpr>("rob"),
          make_unique<VariableExpr>("bob")))));

  fact_body.push_back(std::move(make_unique<Conditional>(
      make_unique<GreaterThanExpr>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(1)),
      std::move(true_fact), std::move(false_fact))));

  auto ae = make_unique<const VariableExpr>("ret_val");
  auto fact_retval = make_unique<const VariableExpr>("ret_val");
  auto fact_def = make_unique<const FunctionDef>("fact", std::move(fact_params),
                                                std::move(fact_body),
                                                std::move(fact_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(fact_def));
  auto ast = make_unique<const Program>(
    std::move(function_defs),
    std::move(statements), std::move(ae));

  ast->Visit(&lowerer_);
  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_ONLY_RESULT);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The program returned: 3628800\n");
}

// V5 tests

// Function def skipped as it is not used
// bob = tuple(3)
TEST_F(CodeGenTest, CanCreateEmptyTuple) {
  Statement::Block foo_statements;

  auto foo_retval = make_unique<const AddExpr>(
      make_unique<const IntegerExpr>(5),
      make_unique<const IntegerExpr>(10));

  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();

  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();

  Statement::Block statements;
  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("bob"),
      make_unique<IntegerExpr>(3))));

  auto ae = make_unique<const VariableExpr>("bob");
  auto ast = make_unique<const Program>(std::move(function_defs),
                                        std::move(statements), std::move(ae));

  ast->Visit(&lowerer_);
  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_ONLY_RESULT);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The program returned a tuple with indices:\n"
            "1: Unassigned\n"
            "2: Unassigned\n"
            "3: Unassigned\n");
}

// Function def skipped as it is not used
// bob = tuple(3)
// bob->2 = 10
TEST_F(CodeGenTest, CanCreateTupleWithOneIntegerAssignment) {
  Statement::Block foo_statements;

  auto foo_retval = make_unique<const AddExpr>(
      make_unique<const IntegerExpr>(5),
      make_unique<const IntegerExpr>(10));

  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();

  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();

  Statement::Block statements;
  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("bob"),
      make_unique<IntegerExpr>(3))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(2)),
      make_unique<IntegerExpr>(10))));

  auto ae = make_unique<const VariableExpr>("bob");
  auto ast = make_unique<const Program>(std::move(function_defs),
                                        std::move(statements), std::move(ae));

  ast->Visit(&lowerer_);
  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_ONLY_RESULT);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The program returned a tuple with indices:\n"
            "1: Unassigned\n"
            "2: Integer with value: 10\n"
            "3: Unassigned\n");
}

// Function def skipped as it is not used
// bob = tuple(3)
// bob->3 = tuple(20)
TEST_F(CodeGenTest, CanCreateTupleWithOneTupleAssignment) {
  Statement::Block foo_statements;

  auto foo_retval = make_unique<const AddExpr>(
      make_unique<const IntegerExpr>(5),
      make_unique<const IntegerExpr>(10));

  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();

  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();

  Statement::Block statements;
  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("bob"),
      make_unique<IntegerExpr>(3))));

  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(3)),
      make_unique<IntegerExpr>(20))));

  auto ae = make_unique<const VariableExpr>("bob");
  auto ast = make_unique<const Program>(std::move(function_defs),
                                        std::move(statements), std::move(ae));

  ast->Visit(&lowerer_);
  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_ONLY_RESULT);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The program returned a tuple with indices:\n"
            "1: Unassigned\n"
            "2: Unassigned\n"
            "3: Tuple with size: 20\n");
}

// Function def skipped as it is not used
// bob = tuple(3)
// bob->2 = -500
// bob->3 = tuple(20)
TEST_F(CodeGenTest, CanCreateTupleWithMixedTypes) {
  Statement::Block foo_statements;

  auto foo_retval = make_unique<const AddExpr>(
      make_unique<const IntegerExpr>(5),
      make_unique<const IntegerExpr>(10));

  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();

  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();

  Statement::Block statements;
  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("bob"),
      make_unique<IntegerExpr>(3))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(2)),
      make_unique<IntegerExpr>(-500))));

  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(3)),
      make_unique<IntegerExpr>(20))));

  auto ae = make_unique<const VariableExpr>("bob");
  auto ast = make_unique<const Program>(std::move(function_defs),
                                        std::move(statements), std::move(ae));

  ast->Visit(&lowerer_);
  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_ONLY_RESULT);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The program returned a tuple with indices:\n"
            "1: Unassigned\n"
            "2: Integer with value: -500\n"
            "3: Tuple with size: 20\n");
}

// Function def skipped as it is not used
// bob = tuple(0)
TEST_F(CodeGenTest, TupleCreationErrorsOutWhenArgumentSuppliedisZero) {
  Statement::Block foo_statements;

  auto foo_retval = make_unique<const AddExpr>(
      make_unique<const IntegerExpr>(5),
      make_unique<const IntegerExpr>(10));

  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();

  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();

  Statement::Block statements;
  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("bob"),
      make_unique<IntegerExpr>(0))));

  auto ae = make_unique<const VariableExpr>("bob");
  auto ast = make_unique<const Program>(std::move(function_defs),
                                        std::move(statements), std::move(ae));

  ast->Visit(&lowerer_);
  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_ONLY_RESULT);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Invalid size, tuple size must be greater than zero\n");
}

// Function def skipped as it is not used
// bob = tuple(-10)
TEST_F(CodeGenTest, TupleCreationErrorsOutWhenArgumentSuppliedisNeg) {
  Statement::Block foo_statements;

  auto foo_retval = make_unique<const AddExpr>(
      make_unique<const IntegerExpr>(5),
      make_unique<const IntegerExpr>(10));

  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();

  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();

  Statement::Block statements;
  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("bob"),
      make_unique<IntegerExpr>(-10))));

  auto ae = make_unique<const VariableExpr>("bob");
  auto ast = make_unique<const Program>(std::move(function_defs),
                                        std::move(statements), std::move(ae));

  ast->Visit(&lowerer_);
  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_ONLY_RESULT);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Invalid size, tuple size must be greater than zero\n");
}

// Function def skipped as it is not used
// bob = tuple(3)
// bob->4 = tuple(20)
TEST_F(CodeGenTest, DereferenceAccessErrorsOutWhenArgumentIsAboveMaxSize) {
  Statement::Block foo_statements;

  auto foo_retval = make_unique<const AddExpr>(
      make_unique<const IntegerExpr>(5),
      make_unique<const IntegerExpr>(10));

  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();

  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();

  Statement::Block statements;
  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("bob"),
      make_unique<IntegerExpr>(3))));

  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(4)),
      make_unique<IntegerExpr>(20))));

  auto ae = make_unique<const VariableExpr>("bob");
  auto ast = make_unique<const Program>(std::move(function_defs),
                                        std::move(statements), std::move(ae));

  ast->Visit(&lowerer_);
  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_ONLY_RESULT);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Out of Bounds Error\n");
}

// Function def skipped as it is not used
// bob = tuple(3)
// bob->0 = tuple(20)
TEST_F(CodeGenTest, DereferenceAccessErrorsOutWhenArgumentIsZero) {
  Statement::Block foo_statements;

  auto foo_retval = make_unique<const AddExpr>(
      make_unique<const IntegerExpr>(5),
      make_unique<const IntegerExpr>(10));

  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();

  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();

  Statement::Block statements;
  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("bob"),
      make_unique<IntegerExpr>(3))));

  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(0)),
      make_unique<IntegerExpr>(20))));

  auto ae = make_unique<const VariableExpr>("bob");
  auto ast = make_unique<const Program>(std::move(function_defs),
                                        std::move(statements), std::move(ae));

  ast->Visit(&lowerer_);
  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_ONLY_RESULT);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Out of Bounds Error\n");
}

// Function def skipped as it is not used
// bob = tuple(3)
// bob->-1 = tuple(20)
TEST_F(CodeGenTest, DereferenceAccessErrorsOutWhenArgumentIsNegative) {
  Statement::Block foo_statements;

  auto foo_retval = make_unique<const AddExpr>(
      make_unique<const IntegerExpr>(5),
      make_unique<const IntegerExpr>(10));

  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();

  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();

  Statement::Block statements;
  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("bob"),
      make_unique<IntegerExpr>(3))));

  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(-1)),
      make_unique<IntegerExpr>(20))));

  auto ae = make_unique<const VariableExpr>("bob");
  auto ast = make_unique<const Program>(std::move(function_defs),
                                        std::move(statements), std::move(ae));

  ast->Visit(&lowerer_);
  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_ONLY_RESULT);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Out of Bounds Error\n");
}

// Function def skipped as it is not used
// bob = tuple(3)
// rob = tuple(2)
// bob = rob
TEST_F(CodeGenTest, TupleCreationErrorsOutCorrectlyWhenTheRHSIsATuple) {
  Statement::Block foo_statements;

  auto foo_retval = make_unique<const AddExpr>(
      make_unique<const IntegerExpr>(5),
      make_unique<const IntegerExpr>(10));

  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();

  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();

  Statement::Block statements;
  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("bob"),
      make_unique<IntegerExpr>(3))));
  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("rob"),
      make_unique<IntegerExpr>(2))));

  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("bob"),
      make_unique<VariableExpr>("rob"))));

  auto ae = make_unique<const VariableExpr>("bob");
  auto ast = make_unique<const Program>(std::move(function_defs),
                                        std::move(statements), std::move(ae));

  ast->Visit(&lowerer_);
  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_ONLY_RESULT);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Invalid type: must be an int\n");
}

// Function def skipped as it is not used
// bob = tuple(3)
// rob = tuple(2)
// bob = rob
TEST_F(CodeGenTest, TupleCanBeAssignedCorrectlyWhenTheRHSIsATuple) {
  Statement::Block foo_statements;

  auto foo_retval = make_unique<const AddExpr>(
      make_unique<const IntegerExpr>(5),
      make_unique<const IntegerExpr>(10));

  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();

  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();

  Statement::Block statements;
  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("bob"),
      make_unique<IntegerExpr>(3))));
  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("rob"),
      make_unique<IntegerExpr>(2))));
  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("rob"),
          make_unique<IntegerExpr>(2)),
      make_unique<IntegerExpr>(10))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("bob"),
      make_unique<VariableExpr>("rob"))));

  auto ae = make_unique<const VariableExpr>("bob");
  auto ast = make_unique<const Program>(std::move(function_defs),
                                        std::move(statements), std::move(ae));

  ast->Visit(&lowerer_);
  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_ONLY_RESULT);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The program returned a tuple with indices:\n"
            "1: Unassigned\n"
            "2: Integer with value: 10\n");
}

// Function def skipped as it is not used
// bob = tuple(3)
// rob = tuple(2)
// bob->1 = rob
TEST_F(CodeGenTest, NestedTupleCreationErrorsOutCorrectlyWhenTheRHSIsATuple) {
  Statement::Block foo_statements;

  auto foo_retval = make_unique<const AddExpr>(
      make_unique<const IntegerExpr>(5),
      make_unique<const IntegerExpr>(10));

  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();

  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();

  Statement::Block statements;
  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("bob"),
      make_unique<IntegerExpr>(3))));
  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("rob"),
      make_unique<IntegerExpr>(2))));

  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(1)),
      make_unique<VariableExpr>("rob"))));

  auto ae = make_unique<const VariableExpr>("bob");
  auto ast = make_unique<const Program>(std::move(function_defs),
                                        std::move(statements), std::move(ae));

  ast->Visit(&lowerer_);
  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_ONLY_RESULT);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Invalid type: must be an int\n");
}

// Function def skipped as it is not used
// bob = tuple(3)
// rob = tuple(2)
// bob->1 = rob
TEST_F(CodeGenTest, NestedTupleAssignedCorrectlyWhenTheRHSIsATuple) {
  Statement::Block foo_statements;

  auto foo_retval = make_unique<const AddExpr>(
      make_unique<const IntegerExpr>(5),
      make_unique<const IntegerExpr>(10));

  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();

  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();

  Statement::Block statements;
  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("bob"),
      make_unique<IntegerExpr>(3))));
  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("rob"),
      make_unique<IntegerExpr>(2))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("rob"),
          make_unique<IntegerExpr>(2)),
      make_unique<IntegerExpr>(10))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(1)),
      make_unique<VariableExpr>("rob"))));

  auto ae = make_unique<const VariableExpr>("bob");
  auto ast = make_unique<const Program>(std::move(function_defs),
                                        std::move(statements), std::move(ae));

  ast->Visit(&lowerer_);
  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_ONLY_RESULT);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The program returned a tuple with indices:\n"
            "1: Tuple with size: 2\n"
            "2: Unassigned\n"
            "3: Unassigned\n");
}

// Function def skipped as it is not used
// bob = tuple(3)
// rob = tuple(2)
// bob->1 = rob
// rob = 5
TEST_F(CodeGenTest, DanglingPointersDoNotOccurWhenReassigningATuple) {
  Statement::Block foo_statements;

  auto foo_retval = make_unique<const AddExpr>(
      make_unique<const IntegerExpr>(5),
      make_unique<const IntegerExpr>(10));

  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();

  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();

  Statement::Block statements;
  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("bob"),
      make_unique<IntegerExpr>(3))));
  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("rob"),
      make_unique<IntegerExpr>(2))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("rob"),
          make_unique<IntegerExpr>(2)),
      make_unique<IntegerExpr>(10))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(1)),
      make_unique<VariableExpr>("rob"))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("rob"),
      make_unique<IntegerExpr>(10))));

  auto ae = make_unique<const VariableExpr>("bob");
  auto ast = make_unique<const Program>(std::move(function_defs),
                                        std::move(statements), std::move(ae));

  ast->Visit(&lowerer_);
  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_ONLY_RESULT);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The program returned a tuple with indices:\n"
            "1: Tuple with size: 2\n"
            "2: Unassigned\n"
            "3: Unassigned\n");
}

// Function def skipped as it is not used
// bob = tuple(3)
// bob->1 = tuple(4)
// bob->1->1 = tuple(1)
// bob->1->2 = -100
// return bob->1
TEST_F(CodeGenTest, DoubleNestedDereferencesAreAssignedCorrectly) {
  Statement::Block foo_statements;

  auto foo_retval = make_unique<const AddExpr>(
      make_unique<const IntegerExpr>(5),
      make_unique<const IntegerExpr>(10));

  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();

  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();

  Statement::Block statements;
  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("bob"),
      make_unique<IntegerExpr>(3))));

  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(1)),
      make_unique<IntegerExpr>(4))));

  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<Dereference>(
          make_unique<Dereference>(
              make_unique<VariableExpr>("bob"),
              make_unique<IntegerExpr>(1)),
          make_unique<IntegerExpr>(1)),
      make_unique<IntegerExpr>(1))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<Dereference>(
              make_unique<VariableExpr>("bob"),
              make_unique<IntegerExpr>(1)),
          make_unique<IntegerExpr>(2)),
      make_unique<IntegerExpr>(-100))));

  auto ae = make_unique<const Dereference>(
        make_unique<VariableExpr>("bob"),
        make_unique<IntegerExpr>(1));
  auto ast = make_unique<const Program>(std::move(function_defs),
                                        std::move(statements), std::move(ae));

  ast->Visit(&lowerer_);
  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_ONLY_RESULT);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The program returned a tuple with indices:\n"
            "1: Tuple with size: 1\n"
            "2: Integer with value: -100\n"
            "3: Unassigned\n"
            "4: Unassigned\n");
}

// Function def skipped as it is not used
// bob = tuple(3)
// bob->1 = tuple(4)
// bob->1->1 = tuple(2)
// bob->1->2 = -100
// rob = bob->1->1
// return rob
TEST_F(CodeGenTest, RHSDoubleNestedDereferencesCanBeAccessedCorrectly) {
  Statement::Block foo_statements;

  auto foo_retval = make_unique<const AddExpr>(
      make_unique<const IntegerExpr>(5),
      make_unique<const IntegerExpr>(10));

  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();

  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();

  Statement::Block statements;
  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("bob"),
      make_unique<IntegerExpr>(3))));

  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(1)),
      make_unique<IntegerExpr>(4))));

  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<Dereference>(
          make_unique<Dereference>(
              make_unique<VariableExpr>("bob"),
              make_unique<IntegerExpr>(1)),
          make_unique<IntegerExpr>(1)),
      make_unique<IntegerExpr>(2))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<Dereference>(
              make_unique<VariableExpr>("bob"),
              make_unique<IntegerExpr>(1)),
          make_unique<IntegerExpr>(2)),
      make_unique<IntegerExpr>(-100))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("rob"),
      make_unique<Dereference>(
          make_unique<Dereference>(
              make_unique<VariableExpr>("bob"),
              make_unique<IntegerExpr>(1)),
          make_unique<IntegerExpr>(1)))));

  auto ae = make_unique<const VariableExpr>("rob");
  auto ast = make_unique<const Program>(std::move(function_defs),
                                        std::move(statements), std::move(ae));

  ast->Visit(&lowerer_);
  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_ONLY_RESULT);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The program returned a tuple with indices:\n"
            "1: Unassigned\n"
            "2: Unassigned\n");
}

// Function def skipped as it is not used
// bob = tuple(3)
// bob->1 = tuple(4)
// bob->2 = 4
// bob->1->1 = tuple(2)
// bob->1->2 = -100
// rob = bob->2->1
// return rob
TEST_F(CodeGenTest, NestedDereferencesErrorOutIfObjectAccessedIsNotTuple) {
  Statement::Block foo_statements;

  auto foo_retval = make_unique<const AddExpr>(
      make_unique<const IntegerExpr>(5),
      make_unique<const IntegerExpr>(10));

  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();

  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();

  Statement::Block statements;
  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("bob"),
      make_unique<IntegerExpr>(3))));

  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(1)),
      make_unique<IntegerExpr>(4))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(2)),
      make_unique<IntegerExpr>(4))));

  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<Dereference>(
          make_unique<Dereference>(
              make_unique<VariableExpr>("bob"),
              make_unique<IntegerExpr>(1)),
          make_unique<IntegerExpr>(1)),
      make_unique<IntegerExpr>(2))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<Dereference>(
              make_unique<VariableExpr>("bob"),
              make_unique<IntegerExpr>(1)),
          make_unique<IntegerExpr>(2)),
      make_unique<IntegerExpr>(-100))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("rob"),
      make_unique<Dereference>(
          make_unique<Dereference>(
              make_unique<VariableExpr>("bob"),
              make_unique<IntegerExpr>(2)),
          make_unique<IntegerExpr>(1)))));

  auto ae = make_unique<const VariableExpr>("rob");
  auto ast = make_unique<const Program>(std::move(function_defs),
                                        std::move(statements), std::move(ae));

  ast->Visit(&lowerer_);
  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_ONLY_RESULT);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Invalid type: must be a tuple\n");
}

// Function def skipped as it is not used
// bob = tuple(3)
// bob->1 = tuple(4)
// rob = bob->2->1
// return rob
TEST_F(CodeGenTest, CannotAccessAnIndexThatDoesNotExist) {
  Statement::Block foo_statements;

  auto foo_retval = make_unique<const AddExpr>(
      make_unique<const IntegerExpr>(5),
      make_unique<const IntegerExpr>(10));

  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();

  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();

  Statement::Block statements;
  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("bob"),
      make_unique<IntegerExpr>(3))));

  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(1)),
      make_unique<IntegerExpr>(4))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("rob"),
      make_unique<Dereference>(
          make_unique<Dereference>(
              make_unique<VariableExpr>("bob"),
              make_unique<IntegerExpr>(2)),
          make_unique<IntegerExpr>(1)))));

  auto ae = make_unique<const VariableExpr>("rob");
  auto ast = make_unique<const Program>(std::move(function_defs),
                                        std::move(statements), std::move(ae));

  ast->Visit(&lowerer_);
  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_ONLY_RESULT);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Value does not exist in the tuple\n");
}

// Function def skipped as it is not used
// bob = tuple(10)
// bob->1 = 1
// bob->2 = 4
// bob->3 = 5
// bob->4 = 10
// rob = (bob->4 / bob->3) + (bob->1 * bob->2)
// rob = rob - bob->4
// return rob
TEST_F(CodeGenTest, ArithmeticExprWorksOnTuplesIndicesIfTheyAreInts) {
  Statement::Block foo_statements;

  auto foo_retval = make_unique<const AddExpr>(
      make_unique<const IntegerExpr>(5),
      make_unique<const IntegerExpr>(10));

  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();

  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();

  Statement::Block statements;
  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("bob"),
      make_unique<IntegerExpr>(4))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(1)),
      make_unique<IntegerExpr>(1))));
  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(2)),
      make_unique<IntegerExpr>(4))));
  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(3)),
      make_unique<IntegerExpr>(5))));
  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(4)),
      make_unique<IntegerExpr>(10))));
  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("rob"),
      make_unique<AddExpr>(
          make_unique<DivideExpr>(
              make_unique<Dereference>(
                  make_unique<VariableExpr>("bob"),
                  make_unique<IntegerExpr>(4)),
              make_unique<Dereference>(
                  make_unique<VariableExpr>("bob"),
                  make_unique<IntegerExpr>(3))),
          make_unique<MultiplyExpr>(
              make_unique<Dereference>(
                  make_unique<VariableExpr>("bob"),
                  make_unique<IntegerExpr>(2)),
              make_unique<Dereference>(
                  make_unique<VariableExpr>("bob"),
                  make_unique<IntegerExpr>(1)))))));
  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("rob"),
      make_unique<SubtractExpr>(
          make_unique<VariableExpr>("rob"),
          make_unique<Dereference>(
              make_unique<VariableExpr>("bob"),
              make_unique<IntegerExpr>(4))))));

  auto ae = make_unique<const VariableExpr>("rob");
  auto ast = make_unique<const Program>(std::move(function_defs),
                                        std::move(statements), std::move(ae));

  ast->Visit(&lowerer_);
  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_ONLY_RESULT);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The program returned: -4\n");
}

// Function def skipped as it is not used
// bob = tuple(10)
// bob->1 = 1
// bob->2 = 4
// bob->3 = 5
// bob->4 = 10
// if(bob->1 < bob->2 || bob->3 > bob->4)
//  rob = 1
// else
//  rob = 0
// return rob
TEST_F(CodeGenTest, GreaterThanLessThanWorksOnTuplesIndicesIfTheyAreInts) {
  Statement::Block foo_statements;

  auto foo_retval = make_unique<const AddExpr>(
      make_unique<const IntegerExpr>(5),
      make_unique<const IntegerExpr>(10));

  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();

  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();

  Statement::Block statements;
  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("bob"),
      make_unique<IntegerExpr>(4))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(1)),
      make_unique<IntegerExpr>(1))));
  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(2)),
      make_unique<IntegerExpr>(4))));
  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(3)),
      make_unique<IntegerExpr>(5))));
  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(4)),
      make_unique<IntegerExpr>(10))));

  Statement::Block trueblock;
  Statement::Block falseblock;

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("rob"),
      make_unique<IntegerExpr>(1))));
  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("rob"),
      make_unique<IntegerExpr>(0))));

  statements.push_back(make_unique<Conditional>(
      make_unique<LogicalOrExpr>(
          make_unique<LessThanExpr>(
              make_unique<Dereference>(
                  make_unique<VariableExpr>("bob"),
                  make_unique<IntegerExpr>(1)),
              make_unique<Dereference>(
                  make_unique<VariableExpr>("bob"),
                  make_unique<IntegerExpr>(2))),
          make_unique<GreaterThanExpr>(
              make_unique<Dereference>(
                  make_unique<VariableExpr>("bob"),
                  make_unique<IntegerExpr>(3)),
              make_unique<Dereference>(
                  make_unique<VariableExpr>("bob"),
                  make_unique<IntegerExpr>(4)))),
      std::move(trueblock), std::move(falseblock)));

  auto ae = make_unique<const VariableExpr>("rob");
  auto ast = make_unique<const Program>(std::move(function_defs),
                                        std::move(statements), std::move(ae));

  ast->Visit(&lowerer_);
  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_ONLY_RESULT);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The program returned: 1\n");
}

// Function def skipped as it is not used
// bob = tuple(10)
// bob->1 = 1
// bob->2 = 4
// bob->3 = 5
// bob->4 = 10
// if(bob->1 <= bob->2 && bob->3 >= bob->4)
//  rob = 1
// else
//  rob = 0
// return rob
TEST_F(CodeGenTest,
       GreaterThanEqualToLessThanEqualToWorksOnTuplesIndicesIfTheyAreInts) {
  Statement::Block foo_statements;

  auto foo_retval = make_unique<const AddExpr>(
      make_unique<const IntegerExpr>(5),
      make_unique<const IntegerExpr>(10));

  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();

  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();

  Statement::Block statements;
  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("bob"),
      make_unique<IntegerExpr>(4))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(1)),
      make_unique<IntegerExpr>(1))));
  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(2)),
      make_unique<IntegerExpr>(4))));
  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(3)),
      make_unique<IntegerExpr>(5))));
  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(4)),
      make_unique<IntegerExpr>(10))));

  Statement::Block trueblock;
  Statement::Block falseblock;

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("rob"),
      make_unique<IntegerExpr>(1))));
  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("rob"),
      make_unique<IntegerExpr>(0))));

  statements.push_back(make_unique<Conditional>(
      make_unique<LogicalAndExpr>(
          make_unique<LessThanEqualToExpr>(
              make_unique<Dereference>(
                  make_unique<VariableExpr>("bob"),
                  make_unique<IntegerExpr>(1)),
              make_unique<Dereference>(
                  make_unique<VariableExpr>("bob"),
                  make_unique<IntegerExpr>(2))),
          make_unique<GreaterThanEqualToExpr>(
              make_unique<Dereference>(
                  make_unique<VariableExpr>("bob"),
                  make_unique<IntegerExpr>(3)),
              make_unique<Dereference>(
                  make_unique<VariableExpr>("bob"),
                  make_unique<IntegerExpr>(4)))),
      std::move(trueblock), std::move(falseblock)));

  auto ae = make_unique<const VariableExpr>("rob");
  auto ast = make_unique<const Program>(std::move(function_defs),
                                        std::move(statements), std::move(ae));

  ast->Visit(&lowerer_);
  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_ONLY_RESULT);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The program returned: 0\n");
}

// Function def skipped as it is not used
// bob = tuple(10)
// bob->1 = 1
// bob->2 = 4
// bob->3 = tuple(5)
// bob->4 = 10
// if(bob->1 <= bob->2 && bob->3 >= bob->4)
//  rob = 1
// else
//  rob = 0
// return rob
TEST_F(CodeGenTest, CannotUseATupleAsAnArgumentForRelationals) {
  Statement::Block foo_statements;

  auto foo_retval = make_unique<const AddExpr>(
      make_unique<const IntegerExpr>(5),
      make_unique<const IntegerExpr>(10));

  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();

  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();

  Statement::Block statements;
  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("bob"),
      make_unique<IntegerExpr>(4))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(1)),
      make_unique<IntegerExpr>(1))));
  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(2)),
      make_unique<IntegerExpr>(4))));
  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(3)),
      make_unique<IntegerExpr>(5))));
  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(4)),
      make_unique<IntegerExpr>(10))));

  Statement::Block trueblock;
  Statement::Block falseblock;

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("rob"),
      make_unique<IntegerExpr>(1))));
  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("rob"),
      make_unique<IntegerExpr>(0))));

  statements.push_back(make_unique<Conditional>(
      make_unique<LogicalAndExpr>(
          make_unique<LessThanEqualToExpr>(
              make_unique<Dereference>(
                  make_unique<VariableExpr>("bob"),
                  make_unique<IntegerExpr>(1)),
              make_unique<Dereference>(
                  make_unique<VariableExpr>("bob"),
                  make_unique<IntegerExpr>(2))),
          make_unique<GreaterThanEqualToExpr>(
              make_unique<Dereference>(
                  make_unique<VariableExpr>("bob"),
                  make_unique<IntegerExpr>(3)),
              make_unique<Dereference>(
                  make_unique<VariableExpr>("bob"),
                  make_unique<IntegerExpr>(4)))),
      std::move(trueblock), std::move(falseblock)));

  auto ae = make_unique<const VariableExpr>("rob");
  auto ast = make_unique<const Program>(std::move(function_defs),
                                        std::move(statements), std::move(ae));

  ast->Visit(&lowerer_);
  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_ONLY_RESULT);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Invalid type: must be an int\n");
}

// Function def skipped as it is not used
// bob = tuple(10)
// bob->1 = 1
// bob->2 = tuple(2)
// bob->3 = 5
// bob->4 = 10
// rob = (bob->4 / bob->3) + (bob->1 * bob->2)
// rob = rob - bob->4
// return rob
TEST_F(CodeGenTest, ArithExprFailsIfArgumentIsTuple) {
  Statement::Block foo_statements;

  auto foo_retval = make_unique<const AddExpr>(
      make_unique<const IntegerExpr>(5),
      make_unique<const IntegerExpr>(10));

  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();

  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();

  Statement::Block statements;
  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("bob"),
      make_unique<IntegerExpr>(4))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(1)),
      make_unique<IntegerExpr>(1))));
  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(2)),
      make_unique<IntegerExpr>(4))));
  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(3)),
      make_unique<IntegerExpr>(5))));
  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(4)),
      make_unique<IntegerExpr>(10))));
  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("rob"),
      make_unique<AddExpr>(
          make_unique<DivideExpr>(
              make_unique<Dereference>(
                  make_unique<VariableExpr>("bob"),
                  make_unique<IntegerExpr>(4)),
              make_unique<Dereference>(
                  make_unique<VariableExpr>("bob"),
                  make_unique<IntegerExpr>(3))),
          make_unique<MultiplyExpr>(
              make_unique<Dereference>(
                  make_unique<VariableExpr>("bob"),
                  make_unique<IntegerExpr>(2)),
              make_unique<Dereference>(
                  make_unique<VariableExpr>("bob"),
                  make_unique<IntegerExpr>(1)))))));
  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("rob"),
      make_unique<SubtractExpr>(
          make_unique<VariableExpr>("rob"),
          make_unique<Dereference>(
              make_unique<VariableExpr>("bob"),
              make_unique<IntegerExpr>(4))))));

  auto ae = make_unique<const VariableExpr>("rob");
  auto ast = make_unique<const Program>(std::move(function_defs),
                                        std::move(statements), std::move(ae));

  ast->Visit(&lowerer_);
  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_ONLY_RESULT);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Invalid type: must be an int\n");
}