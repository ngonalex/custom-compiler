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

class CodeGenTestV5 : public ::testing::Test {
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

// V5 tests

// Function def skipped as it is not used
// bob = tuple(3)
TEST_F(CodeGenTestV5, CanCreateEmptyTuple) {
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
TEST_F(CodeGenTestV5, CanCreateTupleWithOneIntegerAssignment) {
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
TEST_F(CodeGenTestV5, CanCreateTupleWithOneTupleAssignment) {
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
TEST_F(CodeGenTestV5, CanCreateTupleWithMixedTypes) {
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
TEST_F(CodeGenTestV5, TupleCreationErrorsOutWhenArgumentSuppliedisZero) {
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
TEST_F(CodeGenTestV5, TupleCreationErrorsOutWhenArgumentSuppliedisNeg) {
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
TEST_F(CodeGenTestV5, DereferenceAccessErrorsOutWhenArgumentIsAboveMaxSize) {
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
TEST_F(CodeGenTestV5, DereferenceAccessErrorsOutWhenArgumentIsZero) {
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
TEST_F(CodeGenTestV5, DereferenceAccessErrorsOutWhenArgumentIsNegative) {
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
TEST_F(CodeGenTestV5, TupleCreationErrorsOutCorrectlyWhenTheRHSIsATuple) {
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
TEST_F(CodeGenTestV5, TupleCanBeAssignedCorrectlyWhenTheRHSIsATuple) {
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
TEST_F(CodeGenTestV5, NestedTupleCreationErrorsOutCorrectlyWhenTheRHSIsATuple) {
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
TEST_F(CodeGenTestV5, NestedTupleAssignedCorrectlyWhenTheRHSIsATuple) {
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
TEST_F(CodeGenTestV5, DanglingPointersDoNotOccurWhenReassigningATuple) {
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
TEST_F(CodeGenTestV5, DoubleNestedDereferencesAreAssignedCorrectly) {
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
TEST_F(CodeGenTestV5, RHSDoubleNestedDereferencesCanBeAccessedCorrectly) {
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
TEST_F(CodeGenTestV5, NestedDereferencesErrorOutIfObjectAccessedIsNotTuple) {
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
TEST_F(CodeGenTestV5, CannotAccessAnIndexThatDoesNotExist) {
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
TEST_F(CodeGenTestV5, ArithmeticExprWorksOnTuplesIndicesIfTheyAreInts) {
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
TEST_F(CodeGenTestV5, GreaterThanLessThanWorksOnTuplesIndicesIfTheyAreInts) {
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
TEST_F(CodeGenTestV5,
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
TEST_F(CodeGenTestV5, CannotUseATupleAsAnArgumentForRelationals) {
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
TEST_F(CodeGenTestV5, ArithExprFailsIfArgumentIsTuple) {
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

// def foo(bob)
//  return bob
//
// call foo(bob)
TEST_F(CodeGenTestV5, UseTuplesAsArguments) {
  Statement::Block foo_statements;
  Statement::Block statements;

  auto foo_retval = make_unique<VariableExpr>("bob");

  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();
  foo_params.push_back(std::move(make_unique<VariableExpr>("bob")));

  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();
  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("bob"),
      make_unique<IntegerExpr>(3))));
  arguments.push_back(std::move(make_unique<VariableExpr>("bob")));
  statements.push_back(std::move(make_unique<const FunctionCall>(
      make_unique<const VariableExpr>("foo_retval"), "foo",
      std::move(arguments))));

  auto ae = make_unique<const VariableExpr>("foo_retval");
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

// def foo()
//  bob = tuple(3)
//  return bob
// call foo()
TEST_F(CodeGenTestV5, CanCreateEmptyTupleAndReturnItFunction) {
  Statement::Block foo_statements;
  Statement::Block statements;

  foo_statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("bob"),
      make_unique<IntegerExpr>(3))));

  auto foo_retval = make_unique<VariableExpr>("bob");

  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();

  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();
  statements.push_back(std::move(make_unique<const FunctionCall>(
      make_unique<const VariableExpr>("foo_retval"), "foo",
      std::move(arguments))));

  auto ae = make_unique<const VariableExpr>("foo_retval");
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

// def foo()
//  bob = tuple(3)
//  bob->2 = 10
//  return bob
// call foo()
TEST_F(CodeGenTestV5, CanCreateTupleWithOneIntegerAssignmentFunction) {
  Statement::Block foo_statements;
  Statement::Block statements;

  foo_statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("bob"),
      make_unique<IntegerExpr>(3))));
  foo_statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(2)),
      make_unique<IntegerExpr>(10))));

  auto foo_retval = make_unique<VariableExpr>("bob");
  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();
  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();
  statements.push_back(std::move(make_unique<const FunctionCall>(
      make_unique<const VariableExpr>("foo_retval"), "foo",
      std::move(arguments))));

  auto ae = make_unique<const VariableExpr>("foo_retval");
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

// def foo()
//  bob = tuple(3)
//  bob->2 = -500
//  bob->3 = tuple(20)
//  return bob
// call foo()
TEST_F(CodeGenTestV5, CanCreateTupleWithMixedTypesFunction) {
  Statement::Block foo_statements;
  Statement::Block statements;

  foo_statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("bob"),
      make_unique<IntegerExpr>(3))));
  foo_statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(2)),
      make_unique<IntegerExpr>(-500))));
  foo_statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(3)),
      make_unique<IntegerExpr>(10))));

  auto foo_retval = make_unique<VariableExpr>("bob");

  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();

  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();
  auto ae = make_unique<const VariableExpr>("foo_retval");
  statements.push_back(std::move(make_unique<const FunctionCall>(
      make_unique<const VariableExpr>("foo_retval"), "foo",
      std::move(arguments))));
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
            "3: Tuple with size: 10\n");
}

// def foo()
//  bob = tuple(0)
//  return bob
// call foo()
TEST_F(CodeGenTestV5,
       TupleCreationErrorsOutWhenArgumentSuppliedisZeroFunction) {
  Statement::Block foo_statements;
  Statement::Block statements;

  foo_statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("bob"),
      make_unique<IntegerExpr>(0))));

  auto foo_retval = make_unique<VariableExpr>("bob");

  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();

  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();
  auto ae = make_unique<const VariableExpr>("foo_retval");
  statements.push_back(std::move(make_unique<const FunctionCall>(
      make_unique<const VariableExpr>("foo_retval"), "foo",
      std::move(arguments))));
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

// def foo()
//  bob = tuple(-10)
//  return bob
// call foo()
TEST_F(CodeGenTestV5, TupleCreationErrorsOutWhenArgumentSuppliedisNegFunc) {
  Statement::Block foo_statements;
  Statement::Block statements;

  foo_statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("bob"),
      make_unique<IntegerExpr>(-10))));

  auto foo_retval = make_unique<VariableExpr>("bob");

  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();

  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();
  auto ae = make_unique<const VariableExpr>("foo_retval");
  statements.push_back(std::move(make_unique<const FunctionCall>(
      make_unique<const VariableExpr>("foo_retval"), "foo",
      std::move(arguments))));
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

// def foo()
//  bob = tuple(3)
//  bob->4 = 50
//  return bob
// call foo()
TEST_F(CodeGenTestV5,
       DereferenceAccessErrorsOutWhenArgumentIsAboveMaxSizeFunc) {
  Statement::Block foo_statements;
  Statement::Block statements;

  foo_statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("bob"),
      make_unique<IntegerExpr>(3))));
  foo_statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(4)),
      make_unique<IntegerExpr>(50))));

  auto foo_retval = make_unique<VariableExpr>("bob");

  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();

  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();
  auto ae = make_unique<const VariableExpr>("foo_retval");
  statements.push_back(std::move(make_unique<const FunctionCall>(
      make_unique<const VariableExpr>("foo_retval"), "foo",
      std::move(arguments))));
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

// def foo()
//  bob = tuple(3)
//  bob->0 = 50
//  return bob
// call foo()
TEST_F(CodeGenTestV5, DereferenceAccessErrorsOutWhenArgumentIsZeroFunc) {
  Statement::Block foo_statements;
  Statement::Block statements;

  foo_statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("bob"),
      make_unique<IntegerExpr>(3))));
  foo_statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(0)),
      make_unique<IntegerExpr>(50))));

  auto foo_retval = make_unique<VariableExpr>("bob");

  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();

  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();
  auto ae = make_unique<const VariableExpr>("foo_retval");
  statements.push_back(std::move(make_unique<const FunctionCall>(
      make_unique<const VariableExpr>("foo_retval"), "foo",
      std::move(arguments))));
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

// def foo()
//  bob = tuple(3)
//  bob->-1 = 50
//  return bob
// call foo()
TEST_F(CodeGenTestV5, DereferenceAccessErrorsOutWhenArgumentIsNegFunc) {
  Statement::Block foo_statements;
  Statement::Block statements;

  foo_statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("bob"),
      make_unique<IntegerExpr>(3))));
  foo_statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(-1)),
      make_unique<IntegerExpr>(50))));

  auto foo_retval = make_unique<VariableExpr>("bob");

  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();

  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();
  auto ae = make_unique<const VariableExpr>("foo_retval");
  statements.push_back(std::move(make_unique<const FunctionCall>(
      make_unique<const VariableExpr>("foo_retval"), "foo",
      std::move(arguments))));
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

// def foo()
//  bob = tuple(3)
//  rob = tuple(2)
//  bob = rob
//  return bob
// call foo()
TEST_F(CodeGenTestV5, TupleCanBeAssignedCorrectlyWhenTheRHSIsATupleFunc) {
  Statement::Block foo_statements;
  Statement::Block statements;

  foo_statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("bob"),
      make_unique<IntegerExpr>(3))));
  foo_statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("rob"),
      make_unique<IntegerExpr>(2))));
  foo_statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("bob"),
      make_unique<VariableExpr>("rob"))));

  auto foo_retval = make_unique<VariableExpr>("bob");
  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();
  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();
  auto ae = make_unique<const VariableExpr>("foo_retval");
  statements.push_back(std::move(make_unique<const FunctionCall>(
      make_unique<const VariableExpr>("foo_retval"), "foo",
      std::move(arguments))));
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

// def foo()
//  bob = tuple(3)
//  rob = tuple(2)
//  bob = rob
//  return bob
// call foo()
TEST_F(CodeGenTestV5, ArithAssignedCorrectlyWhenTheRHSIsATupleFunc) {
  Statement::Block foo_statements;
  Statement::Block statements;

  foo_statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("bob"),
      make_unique<IntegerExpr>(3))));
  foo_statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("rob"),
      make_unique<IntegerExpr>(2))));
  foo_statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("bob"),
      make_unique<VariableExpr>("rob"))));

  auto foo_retval = make_unique<VariableExpr>("bob");
  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();
  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();
  auto ae = make_unique<const VariableExpr>("foo_retval");
  statements.push_back(std::move(make_unique<const FunctionCall>(
      make_unique<const VariableExpr>("foo_retval"), "foo",
      std::move(arguments))));
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

// def foo()
//  bob = tuple(3)
//  rob = tuple(2)
//  bob->1 = rob
//  return bob
// call foo()
TEST_F(CodeGenTestV5,
       NestedTupleCreationErrOutCorrectlyWhenTheRHSIsATupleFunc) {
  Statement::Block foo_statements;
  Statement::Block statements;

  foo_statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("bob"),
      make_unique<IntegerExpr>(3))));
  foo_statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("rob"),
      make_unique<IntegerExpr>(2))));
  foo_statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(1)),
      make_unique<VariableExpr>("rob"))));

  auto foo_retval = make_unique<VariableExpr>("bob");
  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();
  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();
  auto ae = make_unique<const VariableExpr>("foo_retval");
  statements.push_back(std::move(make_unique<const FunctionCall>(
      make_unique<const VariableExpr>("foo_retval"), "foo",
      std::move(arguments))));
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

// def foo()
//  bob = tuple(3)
//  rob = tuple(2)
//  bob->1 = rob
//  return bob
// call foo()
TEST_F(CodeGenTestV5, NestedTupleAssignedCorrectlyWhenTheRHSIsATupleFunc) {
  Statement::Block foo_statements;
  Statement::Block statements;

  foo_statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("bob"),
      make_unique<IntegerExpr>(3))));
  foo_statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("rob"),
      make_unique<IntegerExpr>(2))));
  foo_statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(1)),
      make_unique<VariableExpr>("rob"))));

  auto foo_retval = make_unique<VariableExpr>("bob");
  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();
  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();
  auto ae = make_unique<const VariableExpr>("foo_retval");
  statements.push_back(std::move(make_unique<const FunctionCall>(
      make_unique<const VariableExpr>("foo_retval"), "foo",
      std::move(arguments))));
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

// def foo()
//  bob = tuple(3)
//  rob = tuple(2)
//  bob->1 = rob
//  rob = 10
//  return bob
// call foo()
TEST_F(CodeGenTestV5, DanglingPointersDoNotOccurWhenReassigningATupleFunc) {
  Statement::Block foo_statements;
  Statement::Block statements;

  foo_statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("bob"),
      make_unique<IntegerExpr>(3))));
  foo_statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("rob"),
      make_unique<IntegerExpr>(2))));
  foo_statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(1)),
      make_unique<VariableExpr>("rob"))));
  foo_statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("rob"),
      make_unique<IntegerExpr>(10))));
  auto foo_retval = make_unique<VariableExpr>("bob");
  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();
  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();
  auto ae = make_unique<const VariableExpr>("foo_retval");
  statements.push_back(std::move(make_unique<const FunctionCall>(
      make_unique<const VariableExpr>("foo_retval"), "foo",
      std::move(arguments))));
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

// def foo()
//  bob = tuple(3)
//  bob->1 = tuple(4)
//  bob->1->1 = tuple(1)
//  bob->1->2 = -100
//  return bob->1
// call foo()
TEST_F(CodeGenTestV5, DoubleNestedDereferencesAreAssignedCorrectlyFunc) {
  Statement::Block foo_statements;
  Statement::Block statements;

  foo_statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("bob"),
      make_unique<IntegerExpr>(3))));
  foo_statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(1)),
      make_unique<IntegerExpr>(4))));
  foo_statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<Dereference>(
          make_unique<Dereference>(
              make_unique<VariableExpr>("bob"),
              make_unique<IntegerExpr>(1)),
          make_unique<IntegerExpr>(1)),
      make_unique<IntegerExpr>(1))));
  foo_statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<Dereference>(
              make_unique<VariableExpr>("bob"),
              make_unique<IntegerExpr>(1)),
          make_unique<IntegerExpr>(2)),
      make_unique<IntegerExpr>(-100))));

  auto foo_retval = make_unique<const Dereference>(
      make_unique<const VariableExpr>("bob"),
      make_unique<const IntegerExpr>(1));

  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();

  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();
  auto ae = make_unique<const VariableExpr>("foo_retval");
  statements.push_back(std::move(make_unique<const FunctionCall>(
      make_unique<const VariableExpr>("foo_retval"), "foo",
      std::move(arguments))));
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

// def foo()
//  bob = tuple(3)
//  bob->1 = tuple(4)
//  bob->1->1 = tuple(1)
//  bob->1->2 = -100
//  rob = bob->1->1
//  return rob
// call foo()
TEST_F(CodeGenTestV5, RHSDoubleNestedDereferencesCanBeAccessedCorrectlyFunc) {
  Statement::Block foo_statements;
  Statement::Block statements;

  foo_statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("bob"),
      make_unique<IntegerExpr>(3))));
  foo_statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(1)),
      make_unique<IntegerExpr>(4))));
  foo_statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<Dereference>(
          make_unique<Dereference>(
              make_unique<VariableExpr>("bob"),
              make_unique<IntegerExpr>(1)),
          make_unique<IntegerExpr>(1)),
      make_unique<IntegerExpr>(2))));
  foo_statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<Dereference>(
              make_unique<VariableExpr>("bob"),
              make_unique<IntegerExpr>(1)),
          make_unique<IntegerExpr>(2)),
      make_unique<IntegerExpr>(-100))));
  foo_statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("rob"),
      make_unique<Dereference>(
          make_unique<Dereference>(
              make_unique<VariableExpr>("bob"),
              make_unique<IntegerExpr>(1)),
          make_unique<IntegerExpr>(1)))));

  auto foo_retval = make_unique<const VariableExpr>("rob");
  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();
  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();

  auto ae = make_unique<const VariableExpr>("foo_retval");
  statements.push_back(std::move(make_unique<const FunctionCall>(
      make_unique<const VariableExpr>("foo_retval"), "foo",
      std::move(arguments))));
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

// def foo()
//  bob = tuple(3)
//  bob->1 = tuple(4)
//  bob->2 = 4
//  bob->1->1 = tuple(2)
//  bob->1->2 = -100
//  rob = bob->2->1
//  return rob
// call foo()
TEST_F(CodeGenTestV5,
       NestedDereferencesErrorOutIfObjectAccessedIsNotTupleFunc) {
  Statement::Block foo_statements;
  Statement::Block statements;
  foo_statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("bob"),
      make_unique<IntegerExpr>(3))));
  foo_statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(1)),
      make_unique<IntegerExpr>(4))));
  foo_statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(2)),
      make_unique<IntegerExpr>(4))));
  foo_statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<Dereference>(
          make_unique<Dereference>(
              make_unique<VariableExpr>("bob"),
              make_unique<IntegerExpr>(1)),
          make_unique<IntegerExpr>(1)),
      make_unique<IntegerExpr>(2))));
  foo_statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<Dereference>(
              make_unique<VariableExpr>("bob"),
              make_unique<IntegerExpr>(1)),
          make_unique<IntegerExpr>(2)),
      make_unique<IntegerExpr>(-100))));
  foo_statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("rob"),
      make_unique<Dereference>(
          make_unique<Dereference>(
              make_unique<VariableExpr>("bob"),
              make_unique<IntegerExpr>(2)),
          make_unique<IntegerExpr>(1)))));

  auto foo_retval = make_unique<const VariableExpr>("rob");
  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();
  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();
  auto ae = make_unique<const VariableExpr>("foo_retval");
  statements.push_back(std::move(make_unique<const FunctionCall>(
      make_unique<const VariableExpr>("foo_retval"), "foo",
      std::move(arguments))));
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

// def foo()
//  bob = tuple(3)
//  bob->1 = tuple(4)
//  rob = bob->2->1
//  return rob
// call foo()
TEST_F(CodeGenTestV5, CannotAccessAnIndexThatDoesNotExistFunc) {
  Statement::Block foo_statements;
  Statement::Block statements;

  foo_statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("bob"),
      make_unique<IntegerExpr>(3))));
  foo_statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(1)),
      make_unique<IntegerExpr>(4))));
  foo_statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("rob"),
      make_unique<Dereference>(
          make_unique<Dereference>(
              make_unique<VariableExpr>("bob"),
              make_unique<IntegerExpr>(2)),
          make_unique<IntegerExpr>(1)))));

  auto foo_retval = make_unique<const VariableExpr>("rob");
  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();
  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();
  auto ae = make_unique<const VariableExpr>("foo_retval");
  statements.push_back(std::move(make_unique<const FunctionCall>(
      make_unique<const VariableExpr>("foo_retval"), "foo",
      std::move(arguments))));
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

// def foo()
//  bob = tuple(10)
//  bob->1 = 1
//  bob->2 = 4
//  bob->3 = 5
//  bob->4 = 10
//  rob = (bob->4 / bob->3) + (bob->1 * bob->2)
//  rob = rob - bob->4
//  return rob
// call foo()
TEST_F(CodeGenTestV5, ArithmeticExprWorksOnTuplesIndicesIfTheyAreIntsFunc) {
  Statement::Block foo_statements;
  Statement::Block statements;

  foo_statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("bob"),
      make_unique<IntegerExpr>(4))));
  foo_statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(1)),
      make_unique<IntegerExpr>(1))));
  foo_statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(2)),
      make_unique<IntegerExpr>(4))));
  foo_statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(3)),
      make_unique<IntegerExpr>(5))));
  foo_statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(4)),
      make_unique<IntegerExpr>(10))));
  foo_statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
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
  foo_statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("rob"),
      make_unique<SubtractExpr>(
          make_unique<VariableExpr>("rob"),
          make_unique<Dereference>(
              make_unique<VariableExpr>("bob"),
              make_unique<IntegerExpr>(4))))));

  auto foo_retval = make_unique<const VariableExpr>("rob");
  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();
  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();
  auto ae = make_unique<const VariableExpr>("foo_retval");
  statements.push_back(std::move(make_unique<const FunctionCall>(
      make_unique<const VariableExpr>("foo_retval"), "foo",
      std::move(arguments))));
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

// def foo()
//  bob = tuple(10)
//  bob->1 = 1
//  bob->2 = 4
//  bob->3 = 5
//  bob->4 = 10
//  if(bob->1 < bob->2 || bob->3 > bob->4)
//    rob = 1
//  else
//    rob = 0
//  return rob
// call foo()
TEST_F(CodeGenTestV5,
       GreaterThanLessThanWorksOnTuplesIndicesIfTheyAreIntsFunc) {
  Statement::Block foo_statements;
  Statement::Block statements;

  foo_statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("bob"),
      make_unique<IntegerExpr>(4))));
  foo_statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(1)),
      make_unique<IntegerExpr>(1))));
  foo_statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(2)),
      make_unique<IntegerExpr>(4))));
  foo_statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(3)),
      make_unique<IntegerExpr>(5))));
  foo_statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
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

  foo_statements.push_back(make_unique<Conditional>(
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

  auto foo_retval = make_unique<const VariableExpr>("rob");
  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();
  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();
  auto ae = make_unique<const VariableExpr>("foo_retval");
  statements.push_back(std::move(make_unique<const FunctionCall>(
      make_unique<const VariableExpr>("foo_retval"), "foo",
      std::move(arguments))));
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

// def foo()
//  bob = tuple(10)
//  bob->1 = 1
//  bob->2 = 4
//  bob->3 = 5
//  bob->4 = 10
//  if(bob->1 <= bob->2 && bob->3 >= bob->4)
//    rob = 1
//  else
//    rob = 0
//  return rob
// call foo()
TEST_F(CodeGenTestV5,
       GreaterThanEqualToLessThanEqualToWorksOnTupleIndicesIfTheyAreIntsFunc) {
  Statement::Block foo_statements;
  Statement::Block statements;

  foo_statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("bob"),
      make_unique<IntegerExpr>(4))));
  foo_statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(1)),
      make_unique<IntegerExpr>(1))));
  foo_statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(2)),
      make_unique<IntegerExpr>(4))));
  foo_statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(3)),
      make_unique<IntegerExpr>(5))));
  foo_statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
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

  foo_statements.push_back(make_unique<Conditional>(
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

  auto foo_retval = make_unique<const VariableExpr>("rob");
  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();
  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();
  auto ae = make_unique<const VariableExpr>("foo_retval");
  statements.push_back(std::move(make_unique<const FunctionCall>(
      make_unique<const VariableExpr>("foo_retval"), "foo",
      std::move(arguments))));
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

// def foo()
//  bob = tuple(10)
//  bob->1 = 1
//  bob->2 = 4
//  bob->3 = tuple(5)
//  bob->4 = 10
//  if(bob->1 <= bob->2 && bob->3 >= bob->4)
//    rob = 1
//  else
//    rob = 0
//  return rob
// call foo()
TEST_F(CodeGenTestV5, CannotUseATupleAsAnArgumentForRelationalsFunc) {
  Statement::Block foo_statements;
  Statement::Block statements;

  foo_statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("bob"),
      make_unique<IntegerExpr>(4))));
  foo_statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(1)),
      make_unique<IntegerExpr>(1))));
  foo_statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(2)),
      make_unique<IntegerExpr>(4))));
  foo_statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(3)),
      make_unique<IntegerExpr>(5))));
  foo_statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
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

  foo_statements.push_back(make_unique<Conditional>(
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

  auto foo_retval = make_unique<const VariableExpr>("rob");
  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();
  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();
  auto ae = make_unique<const VariableExpr>("foo_retval");
  statements.push_back(std::move(make_unique<const FunctionCall>(
      make_unique<const VariableExpr>("foo_retval"), "foo",
      std::move(arguments))));
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

// def foo()
//  bob = tuple(10)
//  bob->1 = 1
//  bob->2 = tuple(2)
//  bob->3 = 5
//  bob->4 = 10
//  rob = (bob->4 / bob->3) + (bob->1 * bob->2)
//  rob = rob - bob->4
//  return rob
// call foo()
TEST_F(CodeGenTestV5, ArithExprFailsIfArgumentIsTupleFunc) {
  Statement::Block foo_statements;
  Statement::Block statements;

  foo_statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("bob"),
      make_unique<IntegerExpr>(4))));
  foo_statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(1)),
      make_unique<IntegerExpr>(1))));
  foo_statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(2)),
      make_unique<IntegerExpr>(4))));
  foo_statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(3)),
      make_unique<IntegerExpr>(5))));
  foo_statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(4)),
      make_unique<IntegerExpr>(10))));
  foo_statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
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
  foo_statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("rob"),
      make_unique<SubtractExpr>(
          make_unique<VariableExpr>("rob"),
          make_unique<Dereference>(
              make_unique<VariableExpr>("bob"),
              make_unique<IntegerExpr>(4))))));

  auto foo_retval = make_unique<const VariableExpr>("rob");
  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();
  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();
  auto ae = make_unique<const VariableExpr>("foo_retval");
  statements.push_back(std::move(make_unique<const FunctionCall>(
      make_unique<const VariableExpr>("foo_retval"), "foo",
      std::move(arguments))));
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
