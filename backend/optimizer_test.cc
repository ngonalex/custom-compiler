#include <vector>
#include <fstream>

#include "backend/control_flow_graph.h"
#include "backend/code_gen.h"
#include "backend/lowerer_visitor.h"
#include "backend/ir.h"
#include "abstract_syntax/abstract_syntax.h"
#include "utility/memory.h"
#include "gtest/gtest.h"

using cs160::abstract_syntax::backend::AstVisitor;
using cs160::abstract_syntax::backend::ArithmeticExpr;
using cs160::abstract_syntax::backend::IntegerExpr;
using cs160::abstract_syntax::backend::AddExpr;
using cs160::abstract_syntax::backend::AstVisitor;
using cs160::abstract_syntax::backend::Conditional;
using cs160::abstract_syntax::backend::DivideExpr;
using cs160::abstract_syntax::backend::EqualToExpr;
using cs160::abstract_syntax::backend::GreaterThanEqualToExpr;
using cs160::abstract_syntax::backend::GreaterThanExpr;
using cs160::abstract_syntax::backend::IntegerExpr;
using cs160::abstract_syntax::backend::LessThanEqualToExpr;
using cs160::abstract_syntax::backend::LessThanExpr;
using cs160::abstract_syntax::backend::LogicalAndExpr;
using cs160::abstract_syntax::backend::LogicalBinaryOperator;
using cs160::abstract_syntax::backend::LogicalNotExpr;
using cs160::abstract_syntax::backend::LogicalOrExpr;
using cs160::abstract_syntax::backend::AssignmentFromArithExp;
using cs160::abstract_syntax::backend::AssignmentFromNewTuple;
using cs160::abstract_syntax::backend::Dereference;
using cs160::abstract_syntax::backend::Loop;
using cs160::abstract_syntax::backend::MultiplyExpr;
using cs160::abstract_syntax::backend::Statement;
using cs160::abstract_syntax::backend::FunctionCall;
using cs160::abstract_syntax::backend::FunctionDef;
using cs160::abstract_syntax::backend::FunctionDef;
using cs160::backend::ThreeAddressCode;
using cs160::backend::LowererVisitor;
using cs160::backend::ControlFlowGraph;
using cs160::backend::ControlFlowGraphNode;
using cs160::backend::CodeGen;
using cs160::backend::PrintFlag::PRINT_DEBUG;
using cs160::backend::PrintFlag::PRINT_LAST_ARITHMETIC_EXPR;
using cs160::backend::PrintFlag::PRINT_ONLY_RESULT;
using cs160::make_unique;

class OptimizerTest : public ::testing::Test {
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
  int filesize(const char* filename) {
    std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
    int returnvalue = in.tellg();
    return returnvalue;
  }
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
  ControlFlowGraph grapher_;
};

TEST_F(OptimizerTest, ConditionalWithNestedLogicalsWithVariables) {
  Statement::Block statements;

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<SubtractExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("bob"),
      make_unique<AddExpr>(
          make_unique<VariableExpr>("y"),
          make_unique<VariableExpr>("x")))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<LogicalOrExpr>(
          make_unique<const LogicalAndExpr>(
              make_unique<const LessThanExpr>(
                  make_unique<const VariableExpr>("x"),
                  make_unique<const IntegerExpr>(100)),
              make_unique<const GreaterThanExpr>(
                  make_unique<const VariableExpr>("y"),
                  make_unique<const VariableExpr>("x"))),
          make_unique<const LogicalAndExpr>(
              make_unique<const LessThanEqualToExpr>(
                  make_unique<const VariableExpr>("bob"),
                  make_unique<const IntegerExpr>(100)),
              make_unique<const GreaterThanEqualToExpr>(
                  make_unique<const VariableExpr>("bob"),
                  make_unique<const IntegerExpr>(0)))),
      Statement::Block(), Statement::Block())));

  auto arithexpr = make_unique<SubtractExpr>(make_unique<IntegerExpr>(7),
    make_unique<IntegerExpr>(5));

  FunctionDef::Block function_defs;
  auto func_def = GenerateFuncDef();
  function_defs.push_back(std::move(func_def));

  auto expr = make_unique<Program>(std::move(function_defs),
    std::move(statements), std::move(arithexpr));

  expr->Visit(&lowerer_);

  auto lowerer_tac = lowerer_.GetIR();
  std::vector<std::unique_ptr<struct ThreeAddressCode>> optimizercopy;

  for (const auto& block : lowerer_tac) {
    optimizercopy.push_back(make_unique<ThreeAddressCode>(*block));
  }

  grapher_.CreateCFG(std::move(optimizercopy));
  grapher_.Optimize();

  std::ofstream file = std::ofstream("notoptimized.s");
  CodeGen nonoptimized = CodeGen(file, PRINT_LAST_ARITHMETIC_EXPR);
  nonoptimized.GenerateData(lowerer_.totalset());
  nonoptimized.Generate(std::move(lowerer_tac));

  std::ofstream file2 = std::ofstream("optimized.s");
  CodeGen optimized = CodeGen(file2, PRINT_LAST_ARITHMETIC_EXPR);
  optimized.GenerateData(lowerer_.totalset());
  optimized.Generate(grapher_.MakeThreeAddressCode());
  std::cout << "Not Optimized Size: " << filesize("notoptimized.s")
    << "\nOptimized Size: " << filesize("optimized.s") << std::endl;
  EXPECT_LE(filesize("optimized.s"), filesize("notoptimized.s"));
}

TEST_F(OptimizerTest, LoopWithNestedLogicalsWithVariables) {
  Statement::Block statements;

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<SubtractExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("bob"),
      make_unique<AddExpr>(
          make_unique<VariableExpr>("y"),
          make_unique<VariableExpr>("x")))));

  statements.push_back(std::move(make_unique<const Loop>(
      make_unique<LogicalOrExpr>(make_unique<const LogicalAndExpr>(
          make_unique<const LessThanExpr>(
              make_unique<const VariableExpr>("x"),
              make_unique<const IntegerExpr>(100)),
          make_unique<const GreaterThanExpr>(
              make_unique<const VariableExpr>("y"),
              make_unique<const VariableExpr>("x"))),
      make_unique<const LogicalAndExpr>(
          make_unique<const LessThanEqualToExpr>(
              make_unique<const VariableExpr>("bob"),
              make_unique<const IntegerExpr>(100)),
          make_unique<const GreaterThanEqualToExpr>(
              make_unique<const VariableExpr>("bob"),
              make_unique<const IntegerExpr>(0)))),
      Statement::Block())));

  auto arithexpr = make_unique<SubtractExpr>(make_unique<IntegerExpr>(7),
                                             make_unique<IntegerExpr>(5));
  FunctionDef::Block function_defs;
  auto func_def = GenerateFuncDef();
  function_defs.push_back(std::move(func_def));


  auto expr = make_unique<Program>(std::move(function_defs),
                                   std::move(statements),
                                   std::move(arithexpr));

  expr->Visit(&lowerer_);

  auto lowerer_tac = lowerer_.GetIR();
  std::vector<std::unique_ptr<struct ThreeAddressCode>> optimizercopy;

  for (const auto& block : lowerer_tac) {
    optimizercopy.push_back(make_unique<ThreeAddressCode>(*block));
  }

  grapher_.CreateCFG(std::move(optimizercopy));
  grapher_.Optimize();

  std::ofstream file = std::ofstream("notoptimized.s");
  CodeGen nonoptimized = CodeGen(file, PRINT_LAST_ARITHMETIC_EXPR);
  nonoptimized.GenerateData(lowerer_.totalset());
  nonoptimized.Generate(std::move(lowerer_tac));

  std::ofstream file2 = std::ofstream("optimized.s");
  CodeGen optimized = CodeGen(file2, PRINT_LAST_ARITHMETIC_EXPR);
  optimized.GenerateData(lowerer_.totalset());
  optimized.Generate(grapher_.MakeThreeAddressCode());
  std::cout << "Not Optimized Size: " << filesize("notoptimized.s")
    << "\nOptimized Size: " << filesize("optimized.s") << std::endl;
  EXPECT_LE(filesize("optimized.s"), filesize("notoptimized.s"));
}











TEST_F(OptimizerTest,
       LoopWithNestedLogicalsWithVariablesMultiAssignment) {
  Statement::Block statements;

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(make_unique<IntegerExpr>(5),
                           make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<SubtractExpr>(make_unique<IntegerExpr>(5),
                                make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(make_unique<IntegerExpr>(5),
                           make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<SubtractExpr>(make_unique<IntegerExpr>(5),
                                make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(make_unique<IntegerExpr>(5),
                           make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<SubtractExpr>(make_unique<IntegerExpr>(5),
                                make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("bob"),
      make_unique<AddExpr>(make_unique<VariableExpr>("y"),
                           make_unique<VariableExpr>("x")))));

  statements.push_back(std::move(make_unique<const Loop>(
      make_unique<LogicalOrExpr>(make_unique<const LogicalAndExpr>(
          make_unique<const LessThanExpr>(
              make_unique<const VariableExpr>("x"),
              make_unique<const IntegerExpr>(100)),
          make_unique<const GreaterThanExpr>(
              make_unique<const VariableExpr>("y"),
              make_unique<const VariableExpr>("x"))),
      make_unique<const LogicalAndExpr>(
          make_unique<const LessThanEqualToExpr>(
              make_unique<const VariableExpr>("bob"),
              make_unique<const IntegerExpr>(100)),
          make_unique<const GreaterThanEqualToExpr>(
              make_unique<const VariableExpr>("bob"),
              make_unique<const IntegerExpr>(0)))),
      Statement::Block())));

  auto arithexpr = make_unique<SubtractExpr>(make_unique<IntegerExpr>(7),
                                             make_unique<IntegerExpr>(5));
  FunctionDef::Block function_defs;
  auto func_def = GenerateFuncDef();
  function_defs.push_back(std::move(func_def));

  auto expr = make_unique<Program>(std::move(function_defs),
                                   std::move(statements), std::move(arithexpr));

  expr->Visit(&lowerer_);
  auto lowerer_tac = lowerer_.GetIR();
  std::vector<std::unique_ptr<struct ThreeAddressCode>> optimizercopy;

  for (const auto& block : lowerer_tac) {
    optimizercopy.push_back(make_unique<ThreeAddressCode>(*block));
  }

  grapher_.CreateCFG(std::move(optimizercopy));
  grapher_.Optimize();

  std::ofstream file = std::ofstream("notoptimized.s");
  CodeGen nonoptimized = CodeGen(file, PRINT_LAST_ARITHMETIC_EXPR);
  nonoptimized.GenerateData(lowerer_.totalset());
  nonoptimized.Generate(std::move(lowerer_tac));

  std::ofstream file2 = std::ofstream("optimized.s");
  CodeGen optimized = CodeGen(file2, PRINT_LAST_ARITHMETIC_EXPR);
  optimized.GenerateData(lowerer_.totalset());
  optimized.Generate(grapher_.MakeThreeAddressCode());
  std::cout << "Not Optimized Size: " << filesize("notoptimized.s")
    << "\nOptimized Size: " << filesize("optimized.s") << std::endl;
  EXPECT_LE(filesize("optimized.s"), filesize("notoptimized.s"));
}

TEST_F(OptimizerTest, ConditionalsWithBothBranch) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(make_unique<IntegerExpr>(5),
                           make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<SubtractExpr>(make_unique<IntegerExpr>(5),
                                make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("bob"),
      make_unique<AddExpr>(make_unique<VariableExpr>("y"),
                           make_unique<VariableExpr>("x")))));

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("bob"),
      make_unique<AddExpr>(make_unique<VariableExpr>("y"),
                           make_unique<VariableExpr>("x")))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("bob"),
      make_unique<SubtractExpr>(make_unique<VariableExpr>("y"),
                                make_unique<VariableExpr>("x")))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<LogicalOrExpr>(make_unique<const LogicalAndExpr>(
          make_unique<const LessThanExpr>(
              make_unique<const VariableExpr>("x"),
              make_unique<const IntegerExpr>(100)),
          make_unique<const GreaterThanExpr>(
              make_unique<const VariableExpr>("y"),
              make_unique<const VariableExpr>("x"))),
      make_unique<const LogicalAndExpr>(
          make_unique<const LessThanEqualToExpr>(
              make_unique<const VariableExpr>("bob"),
              make_unique<const IntegerExpr>(100)),
          make_unique<const GreaterThanEqualToExpr>(
              make_unique<const VariableExpr>("bob"),
              make_unique<const IntegerExpr>(0)))),
      std::move(trueblock), std::move(falseblock))));

  auto arithexpr = make_unique<SubtractExpr>(make_unique<IntegerExpr>(7),
                                             make_unique<IntegerExpr>(5));

  FunctionDef::Block function_defs;
  auto func_def = GenerateFuncDef();
  function_defs.push_back(std::move(func_def));

  auto expr = make_unique<Program>(std::move(function_defs),
                                   std::move(statements), std::move(arithexpr));

  expr->Visit(&lowerer_);

  auto lowerer_tac = lowerer_.GetIR();
  std::vector<std::unique_ptr<struct ThreeAddressCode>> optimizercopy;

  for (const auto& block : lowerer_tac) {
    optimizercopy.push_back(make_unique<ThreeAddressCode>(*block));
  }

  grapher_.CreateCFG(std::move(optimizercopy));
  grapher_.Optimize();

  std::ofstream file = std::ofstream("notoptimized.s");
  CodeGen nonoptimized = CodeGen(file, PRINT_LAST_ARITHMETIC_EXPR);
  nonoptimized.GenerateData(lowerer_.totalset());
  nonoptimized.Generate(std::move(lowerer_tac));

  std::ofstream file2 = std::ofstream("optimized.s");
  CodeGen optimized = CodeGen(file2, PRINT_LAST_ARITHMETIC_EXPR);
  optimized.GenerateData(lowerer_.totalset());
  optimized.Generate(grapher_.MakeThreeAddressCode());
  std::cout << "Not Optimized Size: " << filesize("notoptimized.s")
    << "\nOptimized Size: " << filesize("optimized.s") << std::endl;
  EXPECT_LE(filesize("optimized.s"), filesize("notoptimized.s"));
}

TEST_F(OptimizerTest, NestedLoop) {
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
        make_unique<IntegerExpr>(3)),
    std::move(nested_loop_body));

  auto loopbody = make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<VariableExpr>("x"),
          make_unique<IntegerExpr>(1)));
  loop_body.push_back(std::move(loop));
  loop_body.push_back(std::move(loopbody));

  auto ast = make_unique<const Loop>(
      make_unique<LessThanExpr>(
          make_unique<VariableExpr>("x"),
          make_unique<IntegerExpr>(5)),
      std::move(loop_body));

  assign->Visit(&lowerer_);
  assign1->Visit(&lowerer_);
  ast->Visit(&lowerer_);

  auto lowerer_tac = lowerer_.GetIR();
  std::vector<std::unique_ptr<struct ThreeAddressCode>> optimizercopy;

  for (const auto& block : lowerer_tac) {
    optimizercopy.push_back(make_unique<ThreeAddressCode>(*block));
  }

  grapher_.CreateCFG(std::move(optimizercopy));
  grapher_.Optimize();

  std::ofstream file = std::ofstream("notoptimized.s");
  CodeGen nonoptimized = CodeGen(file, PRINT_LAST_ARITHMETIC_EXPR);
  nonoptimized.GenerateData(lowerer_.totalset());
  nonoptimized.Generate(std::move(lowerer_tac));

  std::ofstream file2 = std::ofstream("optimized.s");
  CodeGen optimized = CodeGen(file2, PRINT_LAST_ARITHMETIC_EXPR);
  optimized.GenerateData(lowerer_.totalset());
  optimized.Generate(grapher_.MakeThreeAddressCode());
  std::cout << "Not Optimized Size: " << filesize("notoptimized.s")
    << "\nOptimized Size: " << filesize("optimized.s") << std::endl;
  EXPECT_LE(filesize("optimized.s"), filesize("notoptimized.s"));
}
