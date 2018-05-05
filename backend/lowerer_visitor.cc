
#include "backend/lowerer_visitor.h"

namespace cs160 {
namespace backend {

std::string LowererVisitor::GetOutputArithmeticHelper(std::string output,
  int index, std::vector<std::string> printhelper) {
  output = output + blocks_[index]->target.reg().name()
    + " <- " + blocks_[index]->arg1.reg().name();

  output = output + " " + printhelper[blocks_[index]->op.opcode()]
    + " " + blocks_[index]->arg2.reg().name();

  return output;
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
          blocks_[i]->arg2.reg().name() + " == 0";
        break;
      case CONDITIONAL:
        output = output + printhelper[CONDITIONAL] + " " +
          blocks_[i]->arg2.reg().name() + " == 0";
        break;
      // Abstract jumps out
      case JUMP:
        output = output + printhelper[JUMP] + " " +
          blocks_[i]->target.label().name();
        break;
      case JEQUAL:
        output = output + printhelper[JEQUAL] + " " +
          blocks_[i]->target.label().name();
        break;
      case JNOTEQUAL:
        output = output + printhelper[JNOTEQUAL] + " " +
          blocks_[i]->target.label().name();
        break;
      case JGREATER:
        output = output + printhelper[JGREATER] + " " +
          blocks_[i]->target.label().name();
        break;
      case JGREATEREQ:
        output = output + printhelper[JGREATEREQ] + " " +
          blocks_[i]->target.label().name();
        break;
      case JLESS:
        output = output + printhelper[JLESS] + " " +
          blocks_[i]->target.label().name();
        break;
      case JLESSEQ:
        output = output + printhelper[JLESSEQ] + " " +
          blocks_[i]->target.label().name();
        break;
      case LABEL:
        output = output + printhelper[LABEL] + " " +
          blocks_[i]->target.label().name();
        break;
      default:
        output = GetOutputArithmeticHelper(output, i, printhelper);
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

  // Create a cmp to let codegen know it's a branch coming
  CreateComparisionBlock(CONDITIONAL);

  // Jump conditional to the false branch here
  std::string falselabel = JumpLabelHelper();
  CreateJumpBlock(falselabel, JEQUAL);

  // Do this normally
  for (auto& statement : conditional.true_branch()) {
    statement->Visit(this);
  }

  // Create a jump to the continue
  std::string continuelabel = ContinueLabelHelper();
  CreateJumpBlock(continuelabel, JUMP);

  // Create a false label
  CreateLabelBlock(falselabel);

  // This should be inside the false label
  for (auto& statement : conditional.false_branch()) {
    statement->Visit(this);
  }

  // TAC to jump to continue here
  CreateJumpBlock(continuelabel, JUMP);

  // Create a continue label (Entrypoint back to main prog)
  CreateLabelBlock(continuelabel);
}
void LowererVisitor::VisitLoop(const Loop& loop) {
  // Similar to branching (Again flip conditionals + eval variables)
  // Probably have a TAC to create a new label here

  // create a looplabel
  std::string looplabel = LoopLabelHelper();
  CreateLabelBlock(looplabel);

  // Eval guard
  loop.guard().Visit(const_cast<LowererVisitor*>(this));

  // Create a loop to let codegen know it's a loop coming
  CreateComparisionBlock(LOOP);

  // Jump conditional to the continue here
  std::string continuelabel = ContinueLabelHelper();
  CreateJumpBlock(continuelabel, JEQUAL);

  // Do this normally
  for (auto& statement : loop.body()) {
    statement->Visit(this);
  }

  // Jump to the loop again
  CreateJumpBlock(looplabel, JUMP);

  // Create a continue label
  CreateLabelBlock(continuelabel);
}

void LowererVisitor::VisitAssignment(const Assignment& assignment) {
  auto newblock = make_unique<struct ThreeAddressCode>();

  // Visit the left which will add its variable name to the stack
  assignment.lhs().Visit(const_cast<LowererVisitor*>(this));
  std::string varname = variablestack_.top();
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

  auto newblock = make_unique<struct ThreeAddressCode>();

  // Check for variable non assignment here?
  newblock->arg1 = Operand(arg1);

  if (arg2.name() != "") {
    newblock->arg2 = Operand(arg2);
  }

  // if (type == DIV) {check for div zero?}

  newblock->op = Opcode(type);
  // look at this later, just going to do this now to test some things
  newblock->target = Target(Register("t_" +
    std::to_string(counter_.variablecount), VIRTUALREG));

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

void LowererVisitor::CreateComparisionBlock(Type type) {
  ASSERT(type == CONDITIONAL || type == LOOP,
    "Must be a loop or a conditional type");

  auto newblock = make_unique<struct ThreeAddressCode>();
  newblock->arg2 = Operand(blocks_[blocks_.size()-1]->target.reg());
  // Flip the comparision so it jumps if it's negative
  newblock->arg1 = Operand(0);

  newblock->op = Opcode(type);

  // Might not need this
  // newblock->target = Target(Register("t_" +
  //   std::to_string(counter_.variablecount), VIRTUALREG));
  // ++counter_.variablecount;
  blocks_.push_back(std::move(newblock));
}

void LowererVisitor::CreateLabelBlock(std::string labelname) {
  auto labelblock  = make_unique<struct ThreeAddressCode>();
  labelblock->target = Target(Label(labelname));
  labelblock->op = Opcode(LABEL);
  blocks_.push_back(std::move(labelblock));
}

void LowererVisitor::CreateJumpBlock(std::string jumpname, Type type) {
  // Probably a better way to do this
  ASSERT(type == JUMP || type == JEQUAL || type == JGREATER
    || type == JGREATEREQ || type == JLESS || type == JLESSEQ
    || type == JNOTEQUAL, "Must be a jump type");

  auto jumpblock = make_unique<struct ThreeAddressCode>();
  jumpblock->target = Target(Label(jumpname));
  jumpblock->op = Opcode(type);

  blocks_.push_back(std::move(jumpblock));
}

std::string LowererVisitor::JumpLabelHelper() {
  std::string newlabel = "falsebranch" + std::to_string(counter_.branchcount);
  ++counter_.branchcount;
  return newlabel;
}

std::string LowererVisitor::LoopLabelHelper() {
  std::string newloop = "loop" + std::to_string(counter_.loopcount);
  ++counter_.loopcount;
  return newloop;
}

std::string LowererVisitor::ContinueLabelHelper() {
  std::string newcontinue = "continue" + std::to_string(counter_.continuecount);
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
      arg = Register(variablestack_.top(), VARIABLEREG);
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
