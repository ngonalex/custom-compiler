#include "backend/code_gen.h"

namespace cs160 {
namespace backend {

// For V2 We want to print out in 2 places
// With an assignment instruction e.g. string <- register
// Or At the end when it's an arithmetic instruction

// High level wise the way this SHOULD work in the future when Generate
// Assignment is implemented is that
// GeneratePrinter, GeneratePrintHeader are unique
// There will be subfunctions later for each assignment though
// e.g x= 2+5, y = 2-10 should create 4 subfunctions
// xascii: .ascii "Variable x is equal to"
// yascii: .ascii "Variable y is equal to"
// printx: (assembly code to print xascii here)
// printy: (assembly code to print yascii here)
// printy/x should contain a jmp/call instruction to "printstart"
// defined in GeneratePrintHeader
// so it just does it's job of printing INTs normally

// How it all links together (Unique printer described above) ->
// call printstart: -> call intloop -> (Potentiall a jump to printnegative:)
// call printloop: -> call printnewline ->ret printloop: ... ret uniqueprinter
// -> ret original place that called it (_start)
// Basically this print routine should NOT affect the regular
// assembly in anyway and is ONLY really meant for making
// unit testing our assembly easier. Also Very crude implementation,
// doesn't follow calling conventions at all , Refactor later
// Note to self each function requires that rax has the value to be printed
// It cannot be at the top of the stack until after the call to printstart

void CodeGen::GeneratePrinter() {
  // ASSUME RESULT IS IN RAX

  GeneratePrintHeader();

  // intloop divides the int in rax by 10
  // and converts the remainder into a char
  // for printing
  // E.g 12345 -> rax: 1234, rdx: 5 -> convert + push 5 to stack
  outfile_ << "intloop:" << std::endl;
  outfile_ << "\tmov $10, %rcx" << std::endl;
  outfile_ << "\txor %rdx, %rdx" << std::endl;

  outfile_ << "\tdiv %rcx" << std::endl;
  outfile_ << "\tadd $'0', %rdx" << std::endl;
  outfile_ << "\tinc %rsi" << std::endl;
  outfile_ << "\tpush %rdx" << std::endl;
  outfile_ << "\tcmp $0, %rax" << std::endl;
  outfile_ << "\tjnz intloop" << std::endl;

  outfile_ << "\tmov %rsi, %rbx" << std::endl;
  outfile_ << "\tcmp $1, %rdi" << std::endl;
  outfile_ << "\tje printnegative" << std::endl;
  outfile_ << "\tjmp printloop" << std::endl;

  // Printloop
  outfile_ << "printloop:" << std::endl;
  outfile_ << "\tmov $1, %rax" << std::endl;
  outfile_ << "\tmov $1, %rdi" << std::endl;
  outfile_ << "\tmov %rsp, %rsi" << std::endl;
  outfile_ << "\tmov $1, %rdx" << std::endl;
  outfile_ << "\tsyscall" << std::endl;

  outfile_ << "\tpop %rdx" << std::endl;
  outfile_ << "\tdec %rbx" << std::endl;
  outfile_ << "\tcmp $0, %rbx" << std::endl;
  outfile_ << "\tjnz printloop" << std::endl;

  outfile_ << "\tmov $1, %rax" << std::endl;
  outfile_ << "\tmov $1, %rdi" << std::endl;
  outfile_ << "\tmov $printnewline, %rsi" << std::endl;
  outfile_ << "\tmov $1, %rdx" << std::endl;
  outfile_ << "\tsyscall" << std::endl;

  outfile_ << "\tret" << std::endl;

  // printnegative:
  outfile_ << "printnegative:" << std::endl;
  outfile_ << "\tmov $1, %rax" << std::endl;
  outfile_ << "\tmov $1, %rdi" << std::endl;
  outfile_ << "\tmov $negative, %rsi" << std::endl;
  outfile_ << "\tmov $1, %rdx" << std::endl;
  outfile_ << "\tsyscall" << std::endl;
  outfile_ << "\tjmp printloop" << std::endl;

  // negative:
  outfile_ << "negative:" << std::endl;
  outfile_ << "\t.ascii \"-\"" << std::endl;

  // printresult
  outfile_ << "printresult:" << std::endl;
  outfile_ << "\t.ascii \"The result is equal to: \"" << std::endl;

  // printnewline
  outfile_ << "printnewline:" << std::endl;
  outfile_ << "\t.ascii \"\\n\"" << std::endl;

  GenerateResult();

  // Generate Unique Printers here
  std::set<std::string>::iterator it;
  for (it = assignmentset_.begin(); it != assignmentset_.end(); ++it) {
    GenerateAssignment(*it);
  }
}

void CodeGen::GeneratePrintHeader() {
  outfile_ << "printstart:" << std::endl;

  // Handle negative number here (If the number
  // were printing out is negative then note it
  // otherwise print normally)

  // Why do we need these  2 lines, Look into later?
  outfile_ << "\txor %rsi, %rsi" << std::endl;
  outfile_ << "\txor %rdi, %rdi" << std::endl;

  // outfile_ << "\tpop %rax" << std::endl;

  outfile_ << "\tcmp $0, %rax" << std::endl;
  outfile_ << "\tjge intloop" << std::endl;
  outfile_ << "\tneg %rax" << std::endl;
  outfile_ << "\tmov $1, %rdi" << std::endl;
  outfile_ << "\tjmp intloop" << std::endl;
}

void CodeGen::GenerateAssignment(std::string variablename) {
  outfile_ << "print" + variablename + ":" << std::endl;
  outfile_ << "\tpush %rax" << std::endl;
  outfile_ << "\tmov $1, %rax" << std::endl;
  outfile_ << "\tmov $1, %rdi" << std::endl;
  outfile_ << "\tmov $" + variablename + "ascii, %rsi" << std::endl;
  outfile_ << "\tmov $" + std::to_string(23 + variablename.length()) + ", %rdx"
           << std::endl;
  outfile_ << "\tsyscall" << std::endl;

  outfile_ << "\txor %rsi, %rsi" << std::endl;
  outfile_ << "\tpop %rax" << std::endl;

  outfile_ << "\tjmp printstart" << std::endl;

  outfile_ << "\tret" << std::endl;

  outfile_ << variablename + "ascii:" << std::endl;
  outfile_ << "\t.ascii \"Variable " + variablename + " is equal to: \""
           << std::endl;
}

void CodeGen::GenerateResult() {
  outfile_ << "printarith:" << std::endl;

  outfile_ << "\tpush %rax" << std::endl;

  outfile_ << "\tmov $1, %rax" << std::endl;
  outfile_ << "\tmov $1, %rdi" << std::endl;
  outfile_ << "\tmov $printresult, %rsi" << std::endl;
  outfile_ << "\tmov $24, %rdx" << std::endl;
  outfile_ << "\tsyscall" << std::endl;

  outfile_ << "\tpop %rax" << std::endl;

  outfile_ << "\tjmp printstart" << std::endl;

  outfile_ << "\tret" << std::endl;
}

void CodeGen::GenerateData(std::set<std::string> variableset) {
  // Creates a .data section for variables
  outfile_ << ".data " << std::endl;
  for (auto iter = variableset.begin(); iter != variableset.end(); ++iter) {
    outfile_ << "\t" << *iter << ":\n\t\t.quad 1" << std::endl;
  }
}

void CodeGen::GenerateEpilogue() {
  outfile_ << "\tmov $60, %rax\n\txor %rdi, %rdi\n\tsyscall" << std::endl;
}

void CodeGen::GenerateBoiler() {
  outfile_ << "\t.global _start" << std::endl;
  outfile_ << "\t.text" << std::endl;
  outfile_ << "_start:" << std::endl;
}

void CodeGen::ClearRegister(std::string reg) {
  outfile_ << "\txor %" + reg + ", %" + reg << std::endl;
}

void CodeGen::GenerateLoadInstructions(std::unique_ptr<ThreeAddressCode> tac) {
  if (tac->arg1.optype() == INT) {
    outfile_ << "\t# Loading in an integer" << std::endl;
    outfile_ << "\tmov $" + std::to_string(tac->arg1.value()) + ", %rcx"
             << std::endl;
    outfile_ << "\tpush %rcx\n" << std::endl;
  } else {
    outfile_ << "\t# Loading a value into variable " + tac->target.reg().name()
             << std::endl;
    ClearRegister("rbx");
    outfile_ << "\tpop %rbx" << std::endl;
    outfile_ << "\tmov %rbx, " << tac->target.reg().name() << "" << std::endl;
    outfile_ << "\tpush %rbx" << std::endl;

    // Add it to the set, then call the print function
    assignmentset_.insert(tac->target.reg().name());
    outfile_ << "\tmov %rbx, %rax\n" << std::endl;
    // Call on correct print function
    outfile_ << "\t# Going to print " << tac->target.reg().name() << "\n"
             << std::endl;
    outfile_ << "\tcall print" + tac->target.reg().name() << std::endl;
    outfile_ << "\n\t# Returning from printing " << tac->target.reg().name()
             << "\n"
             << std::endl;
  }
}

void CodeGen::GenerateArithmeticExpr(std::unique_ptr<ThreeAddressCode> tac,
                                     Type type) {
  switch (type) {
    case ADD:
      outfile_ << "\t# Addition";
      GenerateBinaryExprHelper(std::move(tac));
      ClearRegister("rcx");
      outfile_ << "\tadd %rax, %rcx\n\tadd %rbx, %rcx" << std::endl;
      outfile_ << "\tpush %rcx\n" << std::endl;
      break;
    case SUB:
      outfile_ << "\t# Subtraction";
      GenerateBinaryExprHelper(std::move(tac));
      outfile_ << "\tsub %rbx, %rax" << std::endl;
      outfile_ << "\tpush %rax\n" << std::endl;
      break;
    case MULT:
      outfile_ << "\t# Multiplication";
      GenerateBinaryExprHelper(std::move(tac));
      outfile_ << "\timul %rax, %rbx" << std::endl;
      outfile_ << "\tpush %rbx\n" << std::endl;
      break;
    case DIV:
      ClearRegister("rdx");
      outfile_ << "\t# Division";
      GenerateBinaryExprHelper(std::move(tac));
      outfile_ << "\tcqto" << std::endl;  // indicating its a signed division
      outfile_ << "\tidiv %rbx" << std::endl;
      outfile_ << "\tpush %rax\n" << std::endl;
      break;
    default:
      std::cerr << "Inside GenerateArithmeticExpr, something went"
                << "went very wrong\n";
      exit(1);
  }
}

void CodeGen::GenerateRelationalExpr(std::unique_ptr<ThreeAddressCode> tac,
                                     Type type) {
  // Note to self you can abstract this out even more
  switch (type) {
    case LESSTHAN:
      outfile_ << "\t# LessThan Comparision";
      GenerateBinaryExprHelper(std::move(tac));
      outfile_ << "\tcmp %rbx, %rax" << std::endl;
      outfile_ << "\tsetl %dl" << std::endl;
      outfile_ << "\tmovzx %dl, %rcx" << std::endl;
      outfile_ << "\tpush %rcx\n" << std::endl;
      break;
    case LESSTHANEQ:
      outfile_ << "\t# LessThanEq Comparision";
      GenerateBinaryExprHelper(std::move(tac));
      outfile_ << "\tcmp %rbx, %rax" << std::endl;
      outfile_ << "\tsetle %dl" << std::endl;
      outfile_ << "\tmovzx %dl, %rcx" << std::endl;
      outfile_ << "\tpush %rcx\n" << std::endl;
      break;
    case GREATERTHAN:
      outfile_ << "\t# GreaterThan Comparision";
      GenerateBinaryExprHelper(std::move(tac));
      outfile_ << "\tcmp %rbx, %rax" << std::endl;
      outfile_ << "\tsetg %dl" << std::endl;
      outfile_ << "\tmovzx %dl, %rcx" << std::endl;
      outfile_ << "\tpush %rcx\n" << std::endl;
      break;
    case GREATERTHANEQ:
      outfile_ << "\t# GreaterThanEq Comparision";
      GenerateBinaryExprHelper(std::move(tac));
      outfile_ << "\tcmp %rbx, %rax" << std::endl;
      outfile_ << "\tsetge %dl" << std::endl;
      outfile_ << "\tmovzx %dl, %rcx" << std::endl;
      outfile_ << "\tpush %rcx\n" << std::endl;
      break;
    case EQUAL:
      outfile_ << "\t# Equals Comparision";
      GenerateBinaryExprHelper(std::move(tac));
      outfile_ << "\tcmp %rbx, %rax" << std::endl;
      outfile_ << "\tsete %dl" << std::endl;
      outfile_ << "\tmovzx %dl, %rcx" << std::endl;
      outfile_ << "\tpush %rcx\n" << std::endl;
      break;
    default:
      std::cerr << "Inside GenerateRelationalExpr, something went"
                << "went very wrong\n";
      exit(1);
  }
}

void CodeGen::GenerateLogicalExpr(std::unique_ptr<ThreeAddressCode> tac,
                                  Type type) {
  switch (type) {
    case LOGAND:
      outfile_ << "\t# LogicalAnd\n";
      outfile_ << "\tpop %rbx" << std::endl;
      outfile_ << "\tpop %rax" << std::endl;
      outfile_ << "\tand %rbx, %rax" << std::endl;
      outfile_ << "\tpush %rax\n" << std::endl;
      break;
    case LOGOR:
      outfile_ << "\t# LogicalOr\n";
      outfile_ << "\tpop %rbx" << std::endl;
      outfile_ << "\tpop %rax" << std::endl;
      outfile_ << "\tor %rbx, %rax" << std::endl;
      outfile_ << "\tpush %rax\n" << std::endl;
      break;
    case LOGNOT:
      outfile_ << "\t# LogicalNot\n";
      outfile_ << "\tpop %rbx" << std::endl;
      outfile_ << "\txor $1, %rbx" << std::endl;
      // outfile_ << "\tmovzx %bl, %rbx" << std::endl;
      outfile_ << "\tpush %rbx\n" << std::endl;
      break;
    default:
      std::cerr << "Inside GenerateLogicalExpr, something went"
                << "went very wrong\n";
      exit(1);
  }
}

void CodeGen::GenerateBinaryExprHelper(std::unique_ptr<ThreeAddressCode> tac) {
  // Shouldn't have to check it it's been assigned yet
  // Lowerer should have done that
  RegisterType arg1type = tac->arg1.reg().type();
  RegisterType arg2type = tac->arg2.reg().type();

  if (arg1type == VARIABLEREG && arg2type == VARIABLEREG) {
    // Do a double load from the variable names
    outfile_ << " btwn two variables\n";
    outfile_ << "\tmov " << tac->arg1.reg().name() << ", %rax" << std::endl;
    outfile_ << "\tmov " << tac->arg2.reg().name() << ", %rbx" << std::endl;
  } else if (arg1type == VARIABLEREG && arg2type == VIRTUALREG) {
    // Load from the first variable
    outfile_ << " btwn var,int\n";
    outfile_ << "\tmov " << tac->arg1.reg().name() << ", %rax" << std::endl;
    outfile_ << "\tpop %rbx" << std::endl;
  } else if (arg1type == VIRTUALREG && arg2type == VARIABLEREG) {
    // Load from the second variable
    outfile_ << " btwn int, var\n";
    outfile_ << "\tmov " << tac->arg2.reg().name() << ", %rbx" << std::endl;
    outfile_ << "\tpop %rax" << std::endl;
  } else {
    // double pop from stack
    outfile_ << " btwn int, int\n";
    outfile_ << "\tpop %rbx" << std::endl;  // rbx = right
    outfile_ << "\tpop %rax" << std::endl;  // rax = left
  }
}

void CodeGen::Generate(
    std::vector<std::unique_ptr<struct ThreeAddressCode>> blocks) {
  // boiler code here
  GenerateBoiler();

  // IR to assembly inst
  for (unsigned int i = 0; i < blocks.size(); ++i) {
    auto code = std::move(blocks[i]);
    Type opcode = code->op.opcode();

    switch (opcode) {
      case LOAD:
        GenerateLoadInstructions(std::move(code));
        break;
      case ADD:
        GenerateArithmeticExpr(std::move(code), ADD);
        break;
      case SUB:
        GenerateArithmeticExpr(std::move(code), SUB);
        break;
      case MULT:
        GenerateArithmeticExpr(std::move(code), MULT);
        break;
      case DIV:
        GenerateArithmeticExpr(std::move(code), DIV);
        break;
      case LESSTHAN:
        GenerateRelationalExpr(std::move(code), LESSTHAN);
        break;
      case LESSTHANEQ:
        GenerateRelationalExpr(std::move(code), LESSTHANEQ);
        break;
      case GREATERTHAN:
        GenerateRelationalExpr(std::move(code), GREATERTHAN);
        break;
      case GREATERTHANEQ:
        GenerateRelationalExpr(std::move(code), GREATERTHANEQ);
        break;
      case EQUAL:
        GenerateRelationalExpr(std::move(code), EQUAL);
        break;
      case LOGAND:
        GenerateLogicalExpr(std::move(code), LOGAND);
        break;
      case LOGOR:
        GenerateLogicalExpr(std::move(code), LOGOR);
        break;
      case LOGNOT:
        GenerateLogicalExpr(std::move(code), LOGNOT);
        break;
      case LOOP:
        outfile_ << "\t# LOOP\n";
        outfile_ << "\tpop %rax" << std::endl;
        outfile_ << "\tcmp $" << std::to_string(code->arg1.value())
                 << ", %rax\n"
                 << std::endl;
        break;
      case CONDITIONAL:
        outfile_ << "\t# CONDITIONAL\n";
        outfile_ << "\tpop %rax" << std::endl;
        outfile_ << "\tcmp $" << std::to_string(code->arg1.value())
                 << ", %rax\n"
                 << std::endl;
        break;
      // Note to self abstract jumps out later
      case JUMP:
        outfile_ << "\t# JUMP\n";
        outfile_ << "\tjmp " << code->target.label().name() << std::endl;
        break;
      case JEQUAL:
        outfile_ << "\t# Jump on Equal\n";
        outfile_ << "\tje " << code->target.label().name() << std::endl;
        break;
      case JNOTEQUAL:
        outfile_ << "\t# Jump on Not Equal\n";
        outfile_ << "\tjne " << code->target.label().name() << std::endl;
        break;
      case JGREATER:
        outfile_ << "\t# Jump on greater than\n";
        outfile_ << "\tjg " << code->target.label().name() << std::endl;
        break;
      case JGREATEREQ:
        outfile_ << "\t# Jump on greater or equal\n";
        outfile_ << "\tjge " << code->target.label().name() << std::endl;
        break;
      case JLESS:
        outfile_ << "\t# Jump on less than\n";
        outfile_ << "\tjl " << code->target.label().name() << std::endl;
      case JLESSEQ:
        outfile_ << "\t# Jump on less or equal\n";
        outfile_ << "\tjle " << code->target.label().name() << std::endl;
        break;
      case LABEL:
        outfile_ << code->target.label().name() << ":" << std::endl;
        break;
      default:
        std::cerr << "Inside Generate and something really bad happened\n";
        exit(1);
    }
  }
  // This will probably change later, call on the print function for the
  // Arith Expr
  outfile_ << "\tpop %rax" << std::endl;
  outfile_ << "\tcall printarith" << std::endl;

  GenerateEpilogue();
  GeneratePrinter();
  outfile_.close();
}

}  // namespace backend
}  // namespace cs160
