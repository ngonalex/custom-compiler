#include "backend/control_flow_graph.h"

#include <vector>

#include "backend/lowerer_visitor.h"
#include "abstract_syntax/abstract_syntax.h"
#include "utility/memory.h"

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
using cs160::backend::LowererVisitor;
using cs160::backend::ControlFlowGraph;
using cs160::backend::ControlFlowGraphNode;
using cs160::make_unique;

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

int main() {
  LowererVisitor lowerer_;
  ControlFlowGraph grapher_;

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
  grapher_.CreateCFG(std::move(lowerer_.GetIR()));
  grapher_.Optimize();

  std::cout << grapher_.GetOutput() << std::endl;

  return 0;

}

// TEST_F(ControlFlowGraphTest, ConditionalWithNestedLogicalsWithVariables) {
//   Statement::Block statements;

//   statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
//       make_unique<VariableExpr>("x"),
//       make_unique<AddExpr>(
//           make_unique<IntegerExpr>(5),
//           make_unique<IntegerExpr>(10)))));

//   statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
//       make_unique<VariableExpr>("y"),
//       make_unique<SubtractExpr>(
//           make_unique<IntegerExpr>(5),
//           make_unique<IntegerExpr>(10)))));

//   statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
//       make_unique<VariableExpr>("bob"),
//       make_unique<AddExpr>(
//           make_unique<VariableExpr>("y"),
//           make_unique<VariableExpr>("x")))));

//   statements.push_back(std::move(make_unique<const Conditional>(
//       make_unique<LogicalOrExpr>(
//           make_unique<const LogicalAndExpr>(
//               make_unique<const LessThanExpr>(
//                   make_unique<const VariableExpr>("x"),
//                   make_unique<const IntegerExpr>(100)),
//               make_unique<const GreaterThanExpr>(
//                   make_unique<const VariableExpr>("y"),
//                   make_unique<const VariableExpr>("x"))),
//           make_unique<const LogicalAndExpr>(
//               make_unique<const LessThanEqualToExpr>(
//                   make_unique<const VariableExpr>("bob"),
//                   make_unique<const IntegerExpr>(100)),
//               make_unique<const GreaterThanEqualToExpr>(
//                   make_unique<const VariableExpr>("bob"),
//                   make_unique<const IntegerExpr>(0)))),
//       Statement::Block(), Statement::Block())));

//   auto arithexpr = make_unique<SubtractExpr>(make_unique<IntegerExpr>(7),
//     make_unique<IntegerExpr>(5));

//   FunctionDef::Block function_defs;
//   auto func_def = GenerateFuncDef();
//   function_defs.push_back(std::move(func_def));

//   auto expr = make_unique<Program>(std::move(function_defs),
//     std::move(statements), std::move(arithexpr));

//   expr->Visit(&lowerer_);
//   grapher_.CreateCFG(std::move(lowerer_.GetIR()));
//   grapher_.Optimize();

//   EXPECT_EQ(grapher_.GetOutput(), "t_0 <- 5\nt_1 <- 10\nt_2 <- t_0 + t_1\n"
//             "x <- t_2\nt_3 <- 5\nt_4 <- 10\nt_5 <- t_3 - t_4\ny <- t_5\n"
//             "t_6 <- y VARLOAD \nt_7 <- x VARLOAD \nt_8 <- t_6 + t_7\n"
//             "bob <- t_8\nt_9 <- x VARLOAD \nt_10 <- 100\nt_11 <- t_9 < t_10\n"
//             "t_12 <- y VARLOAD \nt_13 <- x VARLOAD \nt_14 <- t_12 > t_13\n"
//             "t_15 <- t_11 && t_14\nt_16 <- bob VARLOAD \nt_17 <- 100\n"
//             "t_18 <- t_16 <= t_17\nt_19 <- bob VARLOAD \nt_20 <- 0\n"
//             "t_21 <- t_19 >= t_20\nt_22 <- t_18 && t_21\n"
//             "t_23 <- t_15 || t_22\nif t_23 == 0\nje falsebranch0\n"
//             "jmp continue0\nMkLabel falsebranch0\njmp continue0\n"
//             "MkLabel continue0\n <-  PRINTARITH \n <-  FUNCTIONDEF \n"
//             "MkLabel func\n <-  FUNPROLOGUE \n <-  FUNEPILOGUE \n");
// }

// TEST_F(ControlFlowGraphTest, LoopWithNestedLogicalsWithVariables) {
//   Statement::Block statements;

//   statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
//       make_unique<VariableExpr>("x"),
//       make_unique<AddExpr>(
//           make_unique<IntegerExpr>(5),
//           make_unique<IntegerExpr>(10)))));

//   statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
//       make_unique<VariableExpr>("x"),
//       make_unique<AddExpr>(
//           make_unique<IntegerExpr>(5),
//           make_unique<IntegerExpr>(10)))));

//   statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
//       make_unique<VariableExpr>("x"),
//       make_unique<AddExpr>(
//           make_unique<IntegerExpr>(5),
//           make_unique<IntegerExpr>(10)))));

//   statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
//       make_unique<VariableExpr>("y"),
//       make_unique<SubtractExpr>(
//           make_unique<IntegerExpr>(5),
//           make_unique<IntegerExpr>(10)))));

//   statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
//       make_unique<VariableExpr>("bob"),
//       make_unique<AddExpr>(
//           make_unique<VariableExpr>("y"),
//           make_unique<VariableExpr>("x")))));

//   statements.push_back(std::move(make_unique<const Loop>(
//       make_unique<LogicalOrExpr>(make_unique<const LogicalAndExpr>(
//           make_unique<const LessThanExpr>(
//               make_unique<const VariableExpr>("x"),
//               make_unique<const IntegerExpr>(100)),
//           make_unique<const GreaterThanExpr>(
//               make_unique<const VariableExpr>("y"),
//               make_unique<const VariableExpr>("x"))),
//       make_unique<const LogicalAndExpr>(
//           make_unique<const LessThanEqualToExpr>(
//               make_unique<const VariableExpr>("bob"),
//               make_unique<const IntegerExpr>(100)),
//           make_unique<const GreaterThanEqualToExpr>(
//               make_unique<const VariableExpr>("bob"),
//               make_unique<const IntegerExpr>(0)))),
//       Statement::Block())));

//   auto arithexpr = make_unique<SubtractExpr>(make_unique<IntegerExpr>(7),
//                                              make_unique<IntegerExpr>(5));
//   FunctionDef::Block function_defs;
//   auto func_def = GenerateFuncDef();
//   function_defs.push_back(std::move(func_def));


//   auto expr = make_unique<Program>(std::move(function_defs),
//                                    std::move(statements),
//                                    std::move(arithexpr));

//   expr->Visit(&lowerer_);
//   grapher_.CreateCFG(std::move(lowerer_.GetIR()));
//   grapher_.Optimize();
//   // t_6 <- 5
//   // t_7 <- 10
//   // t_8 <- t_6 + t_7
//   // x <- t_8
//   // t_9 <- 5
//   // t_10 <- 10
//   // t_11 <- t_9 - t_10
//   // y <- t_11
//   // t_12 <- y VARLOAD
//   // t_13 <- x VARLOAD
//   // t_14 <- t_12 + t_13
//   // bob <- t_14
//   // MkLabel loop0
//   // t_15 <- x VARLOAD
//   // t_16 <- 100
//   // t_17 <- t_15 < t_16
//   // t_18 <- y VARLOAD
//   // t_19 <- x VARLOAD
//   // t_20 <- t_18 > t_19
//   // t_21 <- t_17 && t_20
//   // t_22 <- bob VARLOAD
//   // t_23 <- 100
//   // t_24 <- t_22 <= t_23
//   // t_25 <- bob VARLOAD
//   // t_26 <- 0
//   // t_27 <- t_25 >= t_26
//   // t_28 <- t_24 && t_27
//   // t_29 <- t_21 || t_28
//   // while t_29 == 0
//   // je continue0
//   // jmp loop0
//   // MkLabel continue0
//   // <-  PRINTARITH
//   // <-  FUNCTIONDEF
//   // MkLabel func
//   // <-  FUNPROLOGUE
//   // <-  FUNEPILOGUE
//   EXPECT_EQ(grapher_.GetOutput(), "t_6 <- 5\nt_7 <- 10\nt_8 <- t_6 + t_7\n"
//             "x <- t_8\nt_9 <- 5\nt_10 <- 10\nt_11 <- t_9 - t_10\ny <- t_11\n"
//             "t_12 <- y VARLOAD \nt_13 <- x VARLOAD \nt_14 <- t_12 + t_13\n"
//             "bob <- t_14\nMkLabel loop0\nt_15 <- x VARLOAD \nt_16 <- 100\n"
//             "t_17 <- t_15 < t_16\nt_18 <- y VARLOAD \nt_19 <- x VARLOAD \n"
//             "t_20 <- t_18 > t_19\nt_21 <- t_17 && t_20\nt_22 <- bob VARLOAD \n"
//             "t_23 <- 100\nt_24 <- t_22 <= t_23\nt_25 <- bob VARLOAD \n"
//             "t_26 <- 0\nt_27 <- t_25 >= t_26\nt_28 <- t_24 && t_27\n"
//             "t_29 <- t_21 || t_28\nwhile t_29 == 0\nje continue0\njmp loop0\n"
//             "MkLabel continue0\n <-  PRINTARITH \n <-  FUNCTIONDEF \n"
//             "MkLabel func\n <-  FUNPROLOGUE \n <-  FUNEPILOGUE \n");
// }

// TEST_F(ControlFlowGraphTest
// , LoopWithNestedLogicalsWithVariablesMultiAssignment) {
//   Statement::Block statements;

//   statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
//       make_unique<VariableExpr>("x"),
//       make_unique<AddExpr>(make_unique<IntegerExpr>(5),
//                            make_unique<IntegerExpr>(10)))));

//   statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
//       make_unique<VariableExpr>("y"),
//       make_unique<SubtractExpr>(make_unique<IntegerExpr>(5),
//                                 make_unique<IntegerExpr>(10)))));

//   statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
//       make_unique<VariableExpr>("x"),
//       make_unique<AddExpr>(make_unique<IntegerExpr>(5),
//                            make_unique<IntegerExpr>(10)))));

//   statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
//       make_unique<VariableExpr>("y"),
//       make_unique<SubtractExpr>(make_unique<IntegerExpr>(5),
//                                 make_unique<IntegerExpr>(10)))));

//   statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
//       make_unique<VariableExpr>("x"),
//       make_unique<AddExpr>(make_unique<IntegerExpr>(5),
//                            make_unique<IntegerExpr>(10)))));

//   statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
//       make_unique<VariableExpr>("y"),
//       make_unique<SubtractExpr>(make_unique<IntegerExpr>(5),
//                                 make_unique<IntegerExpr>(10)))));

//   statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
//       make_unique<VariableExpr>("bob"),
//       make_unique<AddExpr>(make_unique<VariableExpr>("y"),
//                            make_unique<VariableExpr>("x")))));

//   statements.push_back(std::move(make_unique<const Loop>(
//       make_unique<LogicalOrExpr>(make_unique<const LogicalAndExpr>(
//           make_unique<const LessThanExpr>(
//               make_unique<const VariableExpr>("x"),
//               make_unique<const IntegerExpr>(100)),
//           make_unique<const GreaterThanExpr>(
//               make_unique<const VariableExpr>("y"),
//               make_unique<const VariableExpr>("x"))),
//       make_unique<const LogicalAndExpr>(
//           make_unique<const LessThanEqualToExpr>(
//               make_unique<const VariableExpr>("bob"),
//               make_unique<const IntegerExpr>(100)),
//           make_unique<const GreaterThanEqualToExpr>(
//               make_unique<const VariableExpr>("bob"),
//               make_unique<const IntegerExpr>(0)))),
//       Statement::Block())));

//   auto arithexpr = make_unique<SubtractExpr>(make_unique<IntegerExpr>(7),
//                                              make_unique<IntegerExpr>(5));
//   FunctionDef::Block function_defs;
//   auto func_def = GenerateFuncDef();
//   function_defs.push_back(std::move(func_def));

//   auto expr = make_unique<Program>(std::move(function_defs),
//                                    std::move(statements), std::move(arithexpr));

//   expr->Visit(&lowerer_);
//   grapher_.CreateCFG(std::move(lowerer_.GetIR()));
//   grapher_.Optimize();
//   EXPECT_EQ(grapher_.GetOutput(),
//             "t_12 <- 5\nt_13 <- 10\nt_14 <- t_12 + t_13\n"
//             "x <- t_14\nt_15 <- 5\nt_16 <- 10\nt_17 <- t_15 - t_16\n"
//             "y <- t_17\nt_18 <- y VARLOAD \nt_19 <- x VARLOAD \n"
//             "t_20 <- t_18 + t_19\nbob <- t_20\nMkLabel loop0\n"
//             "t_21 <- x VARLOAD \nt_22 <- 100\nt_23 <- t_21 < t_22\n"
//             "t_24 <- y VARLOAD \nt_25 <- x VARLOAD \nt_26 <- t_24 > t_25\n"
//             "t_27 <- t_23 && t_26\nt_28 <- bob VARLOAD \nt_29 <- 100\n"
//             "t_30 <- t_28 <= t_29\nt_31 <- bob VARLOAD \nt_32 <- 0\n"
//             "t_33 <- t_31 >= t_32\n"
//             "t_34 <- t_30 && t_33\nt_35 <- t_27 || t_34\nwhile t_35 == 0\n"
//             "je continue0\njmp loop0\n"
//             "MkLabel continue0\n <-  PRINTARITH \n <-  FUNCTIONDEF \n"
//             "MkLabel func\n"
//             " <-  FUNPROLOGUE \n <-  FUNEPILOGUE \n");
// }

// TEST_F(ControlFlowGraphTest, ConditionalsWithBothBranch) {
//   Statement::Block statements;
//   Statement::Block trueblock;
//   Statement::Block falseblock;

//   statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
//       make_unique<VariableExpr>("x"),
//       make_unique<AddExpr>(make_unique<IntegerExpr>(5),
//                            make_unique<IntegerExpr>(10)))));

//   statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
//       make_unique<VariableExpr>("y"),
//       make_unique<SubtractExpr>(make_unique<IntegerExpr>(5),
//                                 make_unique<IntegerExpr>(10)))));

//   statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
//       make_unique<VariableExpr>("bob"),
//       make_unique<AddExpr>(make_unique<VariableExpr>("y"),
//                            make_unique<VariableExpr>("x")))));

//   trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
//       make_unique<VariableExpr>("bob"),
//       make_unique<AddExpr>(make_unique<VariableExpr>("y"),
//                            make_unique<VariableExpr>("x")))));

//   falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
//       make_unique<VariableExpr>("bob"),
//       make_unique<SubtractExpr>(make_unique<VariableExpr>("y"),
//                                 make_unique<VariableExpr>("x")))));

//   statements.push_back(std::move(make_unique<const Conditional>(
//       make_unique<LogicalOrExpr>(make_unique<const LogicalAndExpr>(
//           make_unique<const LessThanExpr>(
//               make_unique<const VariableExpr>("x"),
//               make_unique<const IntegerExpr>(100)),
//           make_unique<const GreaterThanExpr>(
//               make_unique<const VariableExpr>("y"),
//               make_unique<const VariableExpr>("x"))),
//       make_unique<const LogicalAndExpr>(
//           make_unique<const LessThanEqualToExpr>(
//               make_unique<const VariableExpr>("bob"),
//               make_unique<const IntegerExpr>(100)),
//           make_unique<const GreaterThanEqualToExpr>(
//               make_unique<const VariableExpr>("bob"),
//               make_unique<const IntegerExpr>(0)))),
//       std::move(trueblock), std::move(falseblock))));

//   auto arithexpr = make_unique<SubtractExpr>(make_unique<IntegerExpr>(7),
//                                              make_unique<IntegerExpr>(5));

//   FunctionDef::Block function_defs;
//   auto func_def = GenerateFuncDef();
//   function_defs.push_back(std::move(func_def));

//   auto expr = make_unique<Program>(std::move(function_defs),
//                                    std::move(statements), std::move(arithexpr));

//   expr->Visit(&lowerer_);

//   grapher_.CreateCFG(std::move(lowerer_.GetIR()));
//   grapher_.Optimize();
//   EXPECT_EQ(grapher_.GetOutput(),
//             "t_0 <- 5\nt_1 <- 10\nt_2 <- t_0 + t_1\n"
//             "x <- t_2\nt_3 <- 5\nt_4 <- 10\nt_5 <- t_3 - t_4\n"
//             "y <- t_5\nt_6 <- y VARLOAD \nt_7 <- x VARLOAD \n"
//             "t_8 <- t_6 + t_7\nbob <- t_8\nt_9 <- x VARLOAD \n"
//             "t_10 <- 100\nt_11 <- t_9 < t_10\nt_12 <- y VARLOAD \n"
//             "t_13 <- x VARLOAD \nt_14 <- t_12 > t_13\nt_15 <- t_11 && t_14\n"
//             "t_16 <- bob VARLOAD \nt_17 <- 100\nt_18 <- t_16 <= t_17\n"
//             "t_19 <- bob VARLOAD \nt_20 <- 0\nt_21 <- t_19 >= t_20\n"
//             "t_22 <- t_18 && t_21\nt_23 <- t_15 || t_22\nif t_23 == 0\n"
//             "je falsebranch0\njmp continue0\nMkLabel falsebranch0\n"
//             "jmp continue0\nMkLabel continue0\n <-  PRINTARITH \n"
//             " <-  FUNCTIONDEF \nMkLabel func\n <-  FUNPROLOGUE \n"
//             " <-  FUNEPILOGUE \n");
//       }

// TEST_F(ControlFlowGraphTest, NestedLoop) {
//   Statement::Block loop_body;
//   Statement::Block nested_loop_body;

//   auto assign = make_unique<AssignmentFromArithExp>(
//       make_unique<VariableExpr>("x"),
//       make_unique<IntegerExpr>(0));
//   auto assign1 = make_unique<AssignmentFromArithExp>(
//       make_unique<VariableExpr>("y"),
//       make_unique<IntegerExpr>(0));

//   auto nestedloopbody = make_unique<AssignmentFromArithExp>(
//       make_unique<VariableExpr>("y"),
//       make_unique<AddExpr>(
//           make_unique<VariableExpr>("y"),
//           make_unique<IntegerExpr>(2)));
//   nested_loop_body.push_back(std::move(nestedloopbody));

//   auto loop = make_unique<const Loop>(
//     make_unique<LessThanExpr>(
//         make_unique<VariableExpr>("y"),
//         make_unique<IntegerExpr>(3)),
//     std::move(nested_loop_body));

//   auto loopbody = make_unique<AssignmentFromArithExp>(
//       make_unique<VariableExpr>("x"),
//       make_unique<AddExpr>(
//           make_unique<VariableExpr>("x"),
//           make_unique<IntegerExpr>(1)));
//   loop_body.push_back(std::move(loop));
//   loop_body.push_back(std::move(loopbody));

//   auto ast = make_unique<const Loop>(
//       make_unique<LessThanExpr>(
//           make_unique<VariableExpr>("x"),
//           make_unique<IntegerExpr>(5)),
//       std::move(loop_body));

//   assign->Visit(&lowerer_);
//   assign1->Visit(&lowerer_);
//   ast->Visit(&lowerer_);

//   grapher_.CreateCFG(std::move(lowerer_.GetIR()));
//   grapher_.Optimize();

//   EXPECT_EQ(grapher_.GetOutput(), "t_0 <- 0\nx <- t_0\nMkLabel loop0\n"
//             "t_2 <- x VARLOAD \nt_3 <- 5\nt_4 <- t_2 < t_3\nwhile t_4 == 0\n"
//             "je continue0\nMkLabel loop1\nt_5 <- y VARLOAD \nt_6 <- 3\n"
//             "t_7 <- t_5 < t_6\nwhile t_7 == 0\nje continue1\n"
//             "t_8 <- y VARLOAD \nt_9 <- 2\n"
//             "t_10 <- t_8 + t_9\ny <- t_10\njmp loop1\nMkLabel continue1\n"
//             "jmp loop0\nMkLabel continue0\n");
// }
