
#include "backend/lowerer_visitor.h"

namespace cs160 {
namespace backend {

void LowererVisitor::GetOutputArithmeticHelper(std::string &output, int index,
  std::vector<std::string> printhelper) {

  output = output + blocks_[index]->target.reg().name()
    + " <- " + blocks_[index]->arg1.reg().name();

  output = output + " " + printhelper[blocks_[index]->op.opcode()]
    + " " + blocks_[index]->arg2.reg().name();
}

std::string LowererVisitor::GetOutput() {
  // Iterate through the vector and print out each basic block
  std::string output = "";

  // Probably make this a private variable or something?
  std::vector<std::string> printhelper = {"load", "+", "-", "*", "/", "<", "<=",
    ">", ">=", "==", "&&", "||", "¬", "while", "if",
    "jmp", "je", "jne", "jg", "jge", "jl", "jle", "MkLabel"};

  for (unsigned int i = 0; i < blocks_.size(); ++i) {
    // If it's a just a int (Register without a name then access it's value)
    // Otherwise access its name
    Type opcodetype = blocks_[i]->op.opcode();
    switch (opcodetype) {
      case LOAD:
        if (blocks_[i]->arg1.optype() == INT) {
          output = output + blocks_[i]->target.reg().name()
            + " <- " + std::to_string(blocks_[i]->arg1.value());
        } else {
            output = output + blocks_[i]->target.reg().name()
              + " <- " + blocks_[i]->arg1.reg().name();
        }
        break;
      // case ADD:
      //   GetOutputArithmeticHelper(output, i, printhelper);
      //   break;
      // case SUB:
      //   GetOutputArithmeticHelper(output, i, printhelper);
      //   break;
      // case MULT:
      //   GetOutputArithmeticHelper(output, i, printhelper);
      //   break;
      // case DIV:
      //   GetOutputArithmeticHelper(output, i, printhelper);
      //   break;
      // case LESSTHAN:
      //   GetOutputArithmeticHelper(output, i, printhelper);
      //   break;
      // case LESSTHANEQ:
      case LOGNOT:
        output = output + blocks_[i]->target.reg().name()
          + " <- " + printhelper[LOGNOT] + blocks_[i]->arg1.reg().name();
        break;
      case LOOP:
        output = output + printhelper[LOOP] + " " +
          blocks_[i]->arg1.reg().name() + " == 0";
        break;
      case CONDITIONAL:
        output = output + printhelper[CONDITIONAL] + " " +
          blocks_[i]->arg1.reg().name() + " == 0";
        break;
      case JUMP:
        output = output + printhelper[JUMP] + " " +
          blocks_[i]->target.label().name();
        break;
      case JEQUAL:
        output = output + printhelper[JEQUAL] + " " +
          blocks_[i]->target.reg().name();
        break;
      case JNOTEQUAL:
        output = output + printhelper[JNOTEQUAL] + " " +
          blocks_[i]->target.reg().name();
        break;
      case JGREATER:
        output = output + printhelper[JGREATER] + " " +
          blocks_[i]->target.reg().name();
        break;
      case JGREATEREQ:
        output = output + printhelper[JGREATEREQ] + " " +
          blocks_[i]->target.reg().name();
        break;
      case JLESS:
        output = output + printhelper[JEQUAL] + " " +
          blocks_[i]->target.reg().name();
        break;
      case JLESSEQ:
        output = output + printhelper[JEQUAL] + " " +
          blocks_[i]->target.reg().name();
        break;
      case LABEL:
        output = output + printhelper[LABEL] + " " +
          blocks_[i]->target.label().name();
        break;
      default:
        GetOutputArithmeticHelper(output, i, printhelper);
        break;
    }
    output = output + "\n";
  }

  return output;
}

void LowererVisitor::VisitLessThanExpr(const LessThanExpr& exp) {
  // Visit left hand side (Last thing should be the target where it stores it)
  exp.lhs().Visit(const_cast<LowererVisitor*>(this));
  Register arg1 = GetArgument(lastchildtype_);
  exp.rhs().Visit(const_cast<LowererVisitor*>(this));
  Register arg2 = GetArgument(lastchildtype_);

  BinaryOperatorHelper(LESSTHAN, arg1, arg2);
}
void LowererVisitor::VisitLessThanEqualToExpr(const LessThanEqualToExpr& exp) {
  // Visit left hand side (Last thing should be the target where it stores it)
  exp.lhs().Visit(const_cast<LowererVisitor*>(this));
  Register arg1 = GetArgument(lastchildtype_);
  exp.rhs().Visit(const_cast<LowererVisitor*>(this));
  Register arg2 = GetArgument(lastchildtype_);

  BinaryOperatorHelper(LESSTHANEQ, arg1, arg2);
}
void LowererVisitor::VisitGreaterThanExpr(const GreaterThanExpr& exp) {
  // Visit left hand side (Last thing should be the target where it stores it)
  exp.lhs().Visit(const_cast<LowererVisitor*>(this));
  Register arg1 = GetArgument(lastchildtype_);
  exp.rhs().Visit(const_cast<LowererVisitor*>(this));
  Register arg2 = GetArgument(lastchildtype_);

  BinaryOperatorHelper(GREATERTHAN, arg1, arg2);
}
void LowererVisitor::VisitGreaterThanEqualToExpr(
  const GreaterThanEqualToExpr& exp) {
    // Visit left hand side (Last thing should be the target where it stores it)
  exp.lhs().Visit(const_cast<LowererVisitor*>(this));
  Register arg1 = GetArgument(lastchildtype_);
  exp.rhs().Visit(const_cast<LowererVisitor*>(this));
  Register arg2 = GetArgument(lastchildtype_);

  BinaryOperatorHelper(GREATERTHANEQ, arg1, arg2);
}
void LowererVisitor::VisitEqualToExpr(const EqualToExpr& exp) {
  // Visit left hand side (Last thing should be the target where it stores it)
  exp.lhs().Visit(const_cast<LowererVisitor*>(this));
  Register arg1 = GetArgument(lastchildtype_);
  exp.rhs().Visit(const_cast<LowererVisitor*>(this));
  Register arg2 = GetArgument(lastchildtype_);

  BinaryOperatorHelper(EQUAL, arg1, arg2);
}
void LowererVisitor::VisitLogicalAndExpr(const LogicalAndExpr& exp) {
  // Visit left hand side (Last thing should be the target where it stores it)
  exp.lhs().Visit(const_cast<LowererVisitor*>(this));
  Register arg1 = GetArgument(lastchildtype_);
  exp.rhs().Visit(const_cast<LowererVisitor*>(this));
  Register arg2 = GetArgument(lastchildtype_);

  BinaryOperatorHelper(LOGAND, arg1, arg2);
}
void LowererVisitor::VisitLogicalOrExpr(const LogicalOrExpr& exp) {
  // Visit left hand side (Last thing should be the target where it stores it)
  exp.lhs().Visit(const_cast<LowererVisitor*>(this));
  Register arg1 = GetArgument(lastchildtype_);
  exp.rhs().Visit(const_cast<LowererVisitor*>(this));
  Register arg2 = GetArgument(lastchildtype_);

  BinaryOperatorHelper(LOGOR, arg1, arg2);
}
void LowererVisitor::VisitLogicalNotExpr(const LogicalNotExpr& exp) {
  // Visit left hand side (Last thing should be the target where it stores it)
  exp.operand().Visit(const_cast<LowererVisitor*>(this));
  Register arg1 = GetArgument(lastchildtype_);

  BinaryOperatorHelper(LOGNOT, arg1, Register());
}
void LowererVisitor::VisitConditional(const Conditional& conditional) {
  // Make the IR of the guard first
  // We don't "evaluate" the conditional until the boolean
  // value of the guard has been reached
  conditional.guard().Visit(const_cast<LowererVisitor*>(this));

  // So at this point the last statement should be something of the form
  // t_a <- t_b re t_c
  // How should we structure the if TAC
  // Ideas: 1) Structure it like the assembly instructions
  // If/Else branches in assembly are done with cmp, j[re] chains
  // Algorithm would be
  // 1) Look at the previous instructions opcode
  // 2) represent the "if" as a condition jump statement
  // Ex. if(x>5) translates to
  // t0 <- 5, t1 <- x > 5, block1 <- t1 jge/jmp (Basically if t1 is true
  // jump to block 1

  // Code to create if TAC here
  // Two notes: I looked at the gcc compiler and it'll reverse the conditional
  // e.x. x == 5 is represented as a jne

  // Create a cmp to let codegen know it's a branch coming
  auto branchcmpblock = make_unique<struct ThreeAddressCode>();
  branchcmpblock->arg2 = Operand(blocks_[blocks_.size()-1]->target.reg());
  // Flip the comparision so it jumps if it's negative
  branchcmpblock->arg1 = Operand(0);

  branchcmpblock->op = Opcode(CONDITIONAL);
  branchcmpblock->target = Target(Register("t_" +
    std::to_string(counter_.variablecount), VIRTUALREG));
  ++counter_.variablecount;

  blocks_.push_back(std::move(branchcmpblock));

  // Jump conditional to the false branch here
  auto jumpblock = make_unique<struct ThreeAddressCode>();
  // Is it okay to make a "label" a register or should we make
  // a target class that can either be a label or a register?
  std::string falselabel = JumpLabelHelper();
  jumpblock->target = Target(Label(falselabel));
  jumpblock->op = Opcode(JEQUAL);

  blocks_.push_back(std::move(jumpblock));

  // Do this normally
  for (auto& statement : conditional.true_branch()) {
    statement->Visit(this);
  }

  // Create a jump to the continue
  std::string continuelabel = ContinueLabelHelper();
  auto jumpcontinueblock = make_unique<struct ThreeAddressCode>();
  jumpcontinueblock->target = Target(Label(continuelabel));
  jumpcontinueblock->op = Opcode(JUMP);
  blocks_.push_back(std::move(jumpcontinueblock));


  // Create a false label
  auto falselabelblock  = make_unique<struct ThreeAddressCode>();
  falselabelblock->target = Target(Label(falselabel));
  falselabelblock->op = Opcode(LABEL);
  blocks_.push_back(std::move(falselabelblock));


  // This should be inside the false label also
  // is it worth it to check if this branch is empty
  // (if it's empty you don't even need to create a label)
  for (auto& statement : conditional.false_branch()) {
    statement->Visit(this);
  }

  // TAC to jump to continue here
  auto jumpblock2 = make_unique<struct ThreeAddressCode>();
  jumpblock2->target = Target(Label(continuelabel));
  jumpblock2->op = Opcode(JUMP);
  blocks_.push_back(std::move(jumpblock2));


  // Create continue label
  auto jumpcontinueblock2 = make_unique<struct ThreeAddressCode>();
  jumpcontinueblock->target = Target(Label(continuelabel));
  jumpcontinueblock->op = Opcode(JUMP);
  blocks_.push_back(std::move(jumpcontinueblock2));

}
void LowererVisitor::VisitLoop(const Loop& loop) {

  // Similar to branching (Again flip conditionals + eval variables)
  // Probably have a TAC to create a new label here

  // create a looplabel
  std::string looplabel = LoopLabelHelper();
  auto looplabelblock = make_unique<struct ThreeAddressCode>();
  looplabelblock->op = Opcode(LABEL);
  looplabelblock->target = Target(Label(looplabel);
  blocks_.push_back(std::move(looplabelblock));

  // Eval guard
  loop.guard().Visit(const_cast<LowererVisitor*>(this));


  // Create a loop to let codegen know it's a loop coming
  auto loopblock = make_unique<struct ThreeAddressCode>();
  loopblock->arg2 = Operand(blocks_[blocks_.size()-2]->target.reg());
  // Flip the comparision so it jumps if it's negative
  loopblock->arg1 = Operand(0);

  loopblock->op = Opcode(LOOP);
  loopblock->target = Target(Register("t_" +
    std::to_string(counter_.variablecount), VIRTUALREG));
  ++counter_.variablecount;
  blocks_.push_back(std::move(loopblock));


  // Jump conditional to the continue here
  auto jumpblock = make_unique<struct ThreeAddressCode>();
  std::string continuelabel = ContinueLabelHelper();
  jumpblock->target = Target(Label(continuelabel));
  jumpblock->op = Opcode(JEQUAL);
  blocks_.push_back(std::move(jumpblock));


  // Do this normally
  for (auto& statement : loop.body()) {
    statement->Visit(this);
  }

  // Jump to the loop again
  auto jumploop = make_unique<struct ThreeAddressCode>();
  jumploop->target = Target(Label(looplabel));
  jumploop->op = Opcode(JUMP);
  blocks_.push_back(std::move(jumploop));


  // Create a continue label
  auto jumpcontinueblock = make_unique<struct ThreeAddressCode>();
  jumpcontinueblock->target = Target(Label(continuelabel));
  jumpcontinueblock->op = Opcode(LABEL);
  blocks_.push_back(std::move(jumpcontinueblock));
}

void LowererVisitor::VisitAssignment(const Assignment& assignment) {
  auto newblock = make_unique<struct ThreeAddressCode>();

  // Visit the left which will add its variable name to the stack
  assignment.lhs().Visit(const_cast<LowererVisitor*>(this));
  std::string varname = variablestack_.top();
  variableset_.insert(varname);
  variablestack_.pop();

  // Push variablename into the set (Flagging it as being assigned)
  variableset_.insert(varname);

  // assign the right hand side to be equal to the left hand side
  // The latest vector addition is the final register to be set to the
  // string name
  assignment.rhs().Visit(const_cast<LowererVisitor*>(this));

  newblock->target = Target(Register(varname, VARIABLEREG));
  newblock->op = Opcode(LOAD);
  newblock->arg1 = Operand(blocks_[blocks_.size()-1]->target.reg());

  blocks_.push_back(std::move(newblock));
}

void LowererVisitor::VisitProgram(const Program& program) {
  // Do all the Assignments, then eval the AE?

  for (auto& statement : program.statements()) {
      statement->Visit(this);
  }

  program.arithmetic_exp().Visit(this);
}

void LowererVisitor::VisitVariableExpr(const VariableExpr& exp) {
  // Just get the string stored in VariableExpr and push it to
  // the stack
  variablestack_.push(exp.name());
  lastchildtype_ = VARCHILD;
}

// General comment, Lots of reused code, Think about abstracting out to helper
// functions as the only difference between add/sub/mult/div is their "sign"
// (Also some additional error handling for div req)

void LowererVisitor::BinaryOperatorHelper(Type type,
  Register arg1, Register arg2) {
  // Load value into target (t <- prev->target + prev->prev->target)
  // Last two elements of the vector should be the integers to load in
  int size = blocks_.size();

  auto newblock = make_unique<struct ThreeAddressCode>();

  // Check for variable non assignment here?
  newblock->arg1 = Operand(arg1);

  if (arg2.name() != "") {
    newblock->arg2 = Operand(arg2);
  }

  // if (type == DIV) {check for div zero?}

  newblock->op = Opcode(type);
  // look at this later, just going to do this now to test some things
  newblock->target = Target(Target(Register("t_" +
    std::to_string(counter_.variablecount), VIRTUALREG)));

  // Push into vector
  blocks_.push_back(std::move(newblock));

  ++counter_.variablecount;
  lastchildtype_ = INTCHILD;
}

void LowererVisitor::VisitIntegerExpr(const IntegerExpr& exp) {
  // Load value into a target (t <- value)
  auto newblock = make_unique<struct ThreeAddressCode>();

  newblock->arg1 = Operand(exp.value());
  newblock->op = Opcode(LOAD);

  // look at this later,just going to do this now to test some things
  newblock->target = Target(Register("t_" +
    std::to_string(counter_.variablecount), VIRTUALREG));

  // Push into vector
  blocks_.push_back(std::move(newblock));
  lastchildtype_ = INTCHILD;

  ++counter_.variablecount;
}

void LowererVisitor::VisitAddExpr(const AddExpr& exp) {
  exp.lhs().Visit(const_cast<LowererVisitor*>(this));
  Register arg1 = GetArgument(lastchildtype_);
  exp.rhs().Visit(const_cast<LowererVisitor*>(this));
  Register arg2 = GetArgument(lastchildtype_);
  BinaryOperatorHelper(ADD, arg1, arg2);
}

void LowererVisitor::VisitSubtractExpr(const SubtractExpr& exp) {
  // Visit left then right(eventually reaches the base case of Int)
  exp.lhs().Visit(const_cast<LowererVisitor*>(this));
  Register arg1 = GetArgument(lastchildtype_);
  exp.rhs().Visit(const_cast<LowererVisitor*>(this));
  Register arg2 = GetArgument(lastchildtype_);

  BinaryOperatorHelper(SUB, arg1, arg2);
}

void LowererVisitor::VisitMultiplyExpr(const MultiplyExpr& exp) {
  // Visit lhs,rhs
  exp.lhs().Visit(const_cast<LowererVisitor*>(this));
  Register arg1 = GetArgument(lastchildtype_);
  exp.rhs().Visit(const_cast<LowererVisitor*>(this));
  Register arg2 = GetArgument(lastchildtype_);

  BinaryOperatorHelper(MULT, arg1, arg2);
}

void LowererVisitor::VisitDivideExpr(const DivideExpr& exp) {
  // Visit left/right
  exp.lhs().Visit(const_cast<LowererVisitor*>(this));
  Register arg1 = GetArgument(lastchildtype_);
  exp.rhs().Visit(const_cast<LowererVisitor*>(this));
  Register arg2 = GetArgument(lastchildtype_);

  BinaryOperatorHelper(DIV, arg1, arg2);
}

std::string LowererVisitor::JumpLabelHelper() {
  std::string newlabel = "falsebranch" + counter_.branchcount;
  ++counter_.branchcount;
  return newlabel;
}

std::string LowererVisitor::LoopLabelHelper() {
  std::string newloop = "loop" + counter_.loopcount;
  ++counter_.loopcount;
  return newloop;
}

std::string LowererVisitor::ContinueLabelHelper() {
  std::string newcontinue = "continue" + counter_.continuecount;
  ++counter_.continuecount;
  return newcontinue;
}

Register LowererVisitor::GetArgument(ChildType type) {
  Register arg;
  switch (type) {
    case INTCHILD:
      arg = blocks_[blocks_.size()-1]->target.reg();
      break;
    case VARCHILD:
      // Check if variable has been assigned here?
      arg = Register(variablestack_.top());
      variablestack_.pop();

      if ( variableset_.count(arg.name()) == 0 ) {
        std::cerr << "Variable "+ arg.name() +" not assigned";
        exit(1);
      }
      break;
    default:
      // There should be type throw error?
      break;
  }
  return arg;
}


}  // namespace backend
}  // namespace cs160
