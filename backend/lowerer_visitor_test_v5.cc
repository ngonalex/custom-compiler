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


class LowererTestV5 : public ::testing::Test {
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

TEST_F(LowererTestV5, SimpleTupleTest) {
  auto ast = make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("bob"), make_unique<IntegerExpr>(3));
  ast->Visit(&lowerer_);

  EXPECT_EQ(lowerer_.GetOutput(),
            "bob <-  VARCHILDTUPLE \nt_0 <- 3\nbob <- t_0 NEWTUPLE \n");
}


TEST_F(LowererTestV5, AccessTupleTest) {
  auto ast = make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("bob"), make_unique<IntegerExpr>(3));

  // x->1 = 2
  auto def = make_unique<Dereference>(make_unique<VariableExpr>("bob"), make_unique<IntegerExpr>(1));
  auto access = make_unique<AssignmentFromArithExp>(std::move(def), make_unique<IntegerExpr>(2));
  ast->Visit(&lowerer_);
  access->Visit(&lowerer_);

  EXPECT_EQ(lowerer_.GetOutput(), "bob <-  VARCHILDTUPLE \nt_0 <- 3\n"
            "bob <- t_0 NEWTUPLE \nt_1 <- 1\nbob->t_1 <- bob LHSDEREFERENCE Parent\n"
            "t_2 <- 2\nbob->t_1 <- t_2\n");
}

TEST_F(LowererTestV5, TupleRHSDERTest) {
  auto ast = make_unique<AssignmentFromNewTuple>(make_unique<VariableExpr>("bob"), make_unique<IntegerExpr>(3));
  
  // bob->1 = 2
  auto def = make_unique<Dereference>(make_unique<VariableExpr>("bob"), make_unique<IntegerExpr>(1));
  auto access = make_unique<AssignmentFromArithExp>(std::move(def), make_unique<IntegerExpr>(2));
  auto defagain = make_unique<Dereference>(make_unique<VariableExpr>("bob"), make_unique<IntegerExpr>(1));
  auto assign = make_unique<AssignmentFromArithExp>(make_unique<VariableExpr>("y"), std::move(defagain));

  ast->Visit(&lowerer_);
  access->Visit(&lowerer_);
  assign->Visit(&lowerer_);

  EXPECT_EQ(lowerer_.GetOutput(), "bob <-  VARCHILDTUPLE \nt_0 <- 3\nbob <- t_0 NEWTUPLE \nt_1 <- 1\nbob->t_1 <- bob LHSDEREFERENCE Parent\nt_2 <- 2\nbob->t_1 <- t_2\nt_3 <- 1\nbob->t_3 <- bob RHSDEREFERENCE Parent\ny <- bob->t_3\n");
}

TEST_F(LowererTestV5, NestedTupleTest){

  // bob = new Tuple(4)
  // bob -> 1 = new Tuple(2)
  // bob -> 1 -> 2 = 2 + 3
  

  auto assign = make_unique<AssignmentFromNewTuple>(make_unique<VariableExpr>("bob"), make_unique<IntegerExpr>(4));
  auto dereference = make_unique<Dereference>(make_unique<VariableExpr>("bob"), make_unique<IntegerExpr>(1));
  auto nestedassign = make_unique<AssignmentFromNewTuple>(std::move(dereference), make_unique<IntegerExpr>(2));
  auto dereferenceagain = make_unique<Dereference>(make_unique<VariableExpr>("bob"), make_unique<IntegerExpr>(1));
  auto nesteddereference = make_unique<Dereference>(std::move(dereferenceagain), make_unique<IntegerExpr>(2));
  auto assignarith = make_unique<AssignmentFromArithExp>(std::move(nesteddereference), make_unique<AddExpr>(make_unique<IntegerExpr>(2), make_unique<IntegerExpr>(3)));

  assign->Visit(&lowerer_);
  nestedassign->Visit(&lowerer_);
  assignarith->Visit(&lowerer_);

  EXPECT_EQ(lowerer_.GetOutput(), "bob <-  VARCHILDTUPLE \nt_0 <- 4\nbob <- t_0 NEWTUPLE \nt_1 <- 1\nbob->t_1 <- bob LHSDEREFERENCE Parent\nt_2 <- 2\nbob->t_1 <- t_2 NEWTUPLE \nt_3 <- 1\nbob->t_3 <- bob LHSDEREFERENCE Child\nt_4 <- 2\nbob->t_3->t_4 <- bob LHSDEREFERENCE Parent\nt_5 <- 2\nt_6 <- 3\nt_7 <- t_5 + t_6\nbob->t_3->t_4 <- t_7\n");
}

TEST_F(LowererTestV5, AddTupleValueTest) {
  auto ast = make_unique<AssignmentFromNewTuple>(make_unique<VariableExpr>("bob"), make_unique<IntegerExpr>(3));
  
  // bob->1 = 2
  auto def = make_unique<Dereference>(make_unique<VariableExpr>("bob"), make_unique<IntegerExpr>(1));
  auto access = make_unique<AssignmentFromArithExp>(std::move(def), make_unique<IntegerExpr>(2));
  auto defagain = make_unique<Dereference>(make_unique<VariableExpr>("bob"), make_unique<IntegerExpr>(1));

  auto der2 = make_unique<Dereference>(make_unique<VariableExpr>("bob"), make_unique<IntegerExpr>(2));
  auto access2 = make_unique<AssignmentFromArithExp>(std::move(der2), make_unique<IntegerExpr>(3));
  auto defagain2 = make_unique<Dereference>(make_unique<VariableExpr>("bob"), make_unique<IntegerExpr>(2));

  auto assigny = make_unique<AssignmentFromArithExp>(make_unique<VariableExpr>("y"), make_unique<AddExpr>(std::move(defagain), std::move(defagain2)));

  ast->Visit(&lowerer_);
  access->Visit(&lowerer_);
  access2->Visit(&lowerer_);
  assigny->Visit(&lowerer_);

  EXPECT_EQ(lowerer_.GetOutput(), "bob <-  VARCHILDTUPLE \nt_0 <- 3\nbob <- t_0 NEWTUPLE \nt_1 <- 1\nbob->t_1 <- bob LHSDEREFERENCE Parent\nt_2 <- 2\nbob->t_1 <- t_2\nt_3 <- 2\nbob->t_3 <- bob LHSDEREFERENCE Parent\nt_4 <- 3\nbob->t_3 <- t_4\nt_5 <- 1\nbob->t_5 <- bob RHSDEREFERENCE Parent\nt_6 <- 2\nbob->t_6 <- bob RHSDEREFERENCE Parent\nt_7 <- bob->t_5 + bob->t_6\ny <- t_7\n");
}

