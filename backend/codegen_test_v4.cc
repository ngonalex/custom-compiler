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

class CodeGenTestV4 : public ::testing::Test {
 public:
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
 protected:
  LowererVisitor lowerer_;
};

// V4 Tests

// int foo() {
//   return 5 + 10
// }
//
// foo_retval = foo()
TEST_F(CodeGenTestV4, BasicFunctionCallWithNoArguments) {
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
TEST_F(CodeGenTestV4, BasicFunctionCallWithManyArguments) {
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
TEST_F(CodeGenTestV4, BasicFunctionCallWithMoreComplexArithExprAsArguments) {
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
TEST_F(CodeGenTestV4, AssignmentsWithAdditionWorksInFunctions) {
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
TEST_F(CodeGenTestV4, AssignmentsWithSubtractionWorksInFunctions) {
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
TEST_F(CodeGenTestV4, AssignmentsWithMultiplicationWorksInFunctions) {
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
TEST_F(CodeGenTestV4, AssignmentsWithDivisionWorksInFunctions) {
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
TEST_F(CodeGenTestV4,
       AssignmentWithDivisionByZeroErrorsOutCorrectlyInFunctions) {
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
TEST_F(CodeGenTestV4, ConditionalThatEvalsToTrueWorksInFunctions) {
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
TEST_F(CodeGenTestV4, ConditionalThatEvalsToFalseWorksInFunctions) {
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
TEST_F(CodeGenTestV4, NestedConditionalThatEvalsTrueThenTrueFunction) {
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
TEST_F(CodeGenTestV4, NestedConditionalThatEvalsTrueThenFalseFunction) {
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
TEST_F(CodeGenTestV4, NestedConditionalThatEvalsFalseThenTrueFunction) {
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
TEST_F(CodeGenTestV4, NestedConditionalThatEvalsFalseThenFalseFunction) {
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
TEST_F(CodeGenTestV4, SimpleLoopThatIsEnteredInFunction) {
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
TEST_F(CodeGenTestV4, SimpleLoopThatIsNotEnteredInFunction) {
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
TEST_F(CodeGenTestV4, NestedLoopsInAFunction) {
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
TEST_F(CodeGenTestV4, RecursiveFibonacciFunctionWorks) {
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
TEST_F(CodeGenTestV4, RecursiveFactorialFunctionWorks) {
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
