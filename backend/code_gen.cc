#include "backend/code_gen.h"

namespace cs160 {
namespace backend {

void CodeGen::GeneratePrinter() {
  GenerateTupleFlagCheck();
  GenerateIntegerFlagCheck();
  GenerateExistenceCheck();
  GenerateTupleSizeCheck();

  // printtupletypeerror
  outfile_ << "printtupletypeerror:" << std::endl;
  outfile_ << "\tmov $printstring, %rdi" << std::endl;
  outfile_ << "\tmov $tupletypeasciz, %rsi" << std::endl;
  outfile_ << "\tmov $0, %rax" << std::endl;
  outfile_ << "\tcall printf" << std::endl;
  outfile_ << "\tmov $60, %rax" << std::endl;
  outfile_ << "\txor $2, %rdi" << std::endl;
  outfile_ << "\tsyscall" << std::endl;

  // printintegertypeerror
  outfile_ << "printintegertypeerror:" << std::endl;
  outfile_ << "\tmov $printstring, %rdi" << std::endl;
  outfile_ << "\tmov $inttypeasciz, %rsi" << std::endl;
  outfile_ << "\tmov $0, %rax" << std::endl;
  outfile_ << "\tcall printf" << std::endl;
  outfile_ << "\tmov $60, %rax" << std::endl;
  outfile_ << "\txor $2, %rdi" << std::endl;
  outfile_ << "\tsyscall" << std::endl;

  // printexistenceerror
  outfile_ << "printexistenceerror:" << std::endl;
  outfile_ << "\tmov $printstring, %rdi" << std::endl;
  outfile_ << "\tmov $existenceerrorasciz, %rsi" << std::endl;
  outfile_ << "\tmov $0, %rax" << std::endl;
  outfile_ << "\tcall printf" << std::endl;
  outfile_ << "\tmov $60, %rax" << std::endl;
  outfile_ << "\txor $2, %rdi" << std::endl;
  outfile_ << "\tsyscall" << std::endl;

  // printsizeerror (change this later to print out the size of the tuple
  // and the index that tried to be access)
  outfile_ << "printsizeerror:" << std::endl;
  outfile_ << "\tmov $printstring, %rdi" << std::endl;
  outfile_ << "\tmov $outofboundserrorasciz, %rsi" << std::endl;
  outfile_ << "\tmov $0, %rax" << std::endl;
  outfile_ << "\tcall printf" << std::endl;
  outfile_ << "\tmov $60, %rax" << std::endl;
  outfile_ << "\txor $2, %rdi" << std::endl;
  outfile_ << "\tsyscall" << std::endl;

  // negative:
  outfile_ << "negative:" << std::endl;
  outfile_ << "\t.asciz \"-\"" << std::endl;

  // printresult
  outfile_ << "printresult:" << std::endl;
  outfile_ << "\t.asciz \"The result is equal to: \"" << std::endl;

  // printfunctionresult
  outfile_ << "printfunctionresult:" << std::endl;
  outfile_ << "\t.asciz \"The function returned: \"" << std::endl;

  // printnewline
  outfile_ << "printnewline:" << std::endl;
  outfile_ << "\t.asciz \"\\n\""  << std::endl;

  // printstring
  outfile_ << "printstring:" << std::endl;
  outfile_ << "\t.asciz \"%s\"" << std::endl;

  // printint
  outfile_ << "printint:" << std::endl;
  outfile_ << "\t.asciz \"%d\\n\""  << std::endl;

  // printstringint
  outfile_ << "printstringint:" << std::endl;
  outfile_ << "\t.asciz \"%s%d\\n\"" << std::endl;

  // existenceerrorasciz
  outfile_ << "existenceerrorasciz:" << std::endl;
  outfile_ << "\t.asciz \"Value does not exist in the tuple\\n\""
    << std::endl;

  // tupletypeascii
  outfile_ << "tupletypeasciz:" << std::endl;
  outfile_ << "\t.asciz \"Invalid type must be a tuple\\n\"" << std::endl;

  // inttypeascii
  outfile_ << "inttypeasciz:" << std::endl;
  outfile_ << "\t.asciz \"Invalid type must be a int\\n\"" << std::endl;

  // outofboundserrorascii
  outfile_ << "outofboundserrorasciz:" << std::endl;
  outfile_ << "\t.asciz \"Out of Bounds Error\\n\"" << std::endl;

  // Generate Unique Printers here
  std::set<std::string>::iterator it;
  for (it = assignmentset_.begin(); it != assignmentset_.end(); ++it) {
    GeneratePrintAssignment(*it);
  }
}

void CodeGen::GeneratePrintCall(std::string label) {
  outfile_ << "\t# Calling printf" << std::endl;
  outfile_ << "\tpush %rax" << std::endl;
  outfile_ << "\tmov $" << label << ", %rdi" << std::endl;
  outfile_ << "\tmov $0, %rax" << std::endl;
  outfile_ << "\tcall printf" << std::endl;

  outfile_ << "\tpop %rax" << std::endl;
  outfile_ << "\tmov $printint, %rdi" << std::endl;
  outfile_ << "\tmov %rax, %rsi" << std::endl;
  outfile_ << "\tmov $0, %rax" << std::endl;
  outfile_ << "\tcall printf\n" << std::endl;
}

void CodeGen::GeneratePrintAssignment(std::string variablename) {
  outfile_ << variablename + "ascii:" << std::endl;
  outfile_ << "\t.asciz \"Variable " + variablename + " is equal to: \""
    << std::endl;
}

// This needs to change
void CodeGen::GenerateData(std::set<std::string> variableset) {
  // Creates a .data section for variables
  outfile_ << ".data " << std::endl;
  for (auto iter = variableset.begin(); iter != variableset.end(); ++iter) {
    outfile_ << "\t" << *iter << ":\n\t\t.zero 16" << std::endl;
  }
}

void CodeGen::GenerateEpilogue() {
  outfile_ << "\tmov $60, %rax\n";
  outfile_ << "\tmov $2, %rdi\n";
  outfile_ << "\tsyscall\n" << std::endl;
}

void CodeGen::GenerateBoiler() {
  outfile_ << "\t.global main" << std::endl;
  outfile_ << "\t.text" << std::endl;
    outfile_ << "main:" << std::endl;
}

void CodeGen::ClearRegister(std::string reg) {
  outfile_ << "\txor %" + reg + ", %" + reg << std::endl;
}

void CodeGen::GenerateLoadInstructions(std::unique_ptr<ThreeAddressCode> tac) {
  Type loadtype = tac->op.opcode();
  int argumentnum;
  std::string varname;
  std::vector<std::string> parsedstring;

  switch (loadtype) {
    case INTLOAD:
      outfile_ << "\t# Loading in an integer" << std::endl;
      outfile_ << "\tmov $" + std::to_string(tac->arg1.value())
        + ", %rcx" << std::endl;
      outfile_ << "\tpush %rcx\n" << std::endl;
      break;
    case VARLOAD:
      outfile_ << "\t# Loading from a variable" << std::endl;
      outfile_ << "\tmov " << VariableNameHelper(tac->arg1.reg().name())
        << ", %rbx" << std::endl;

      // Then load it into register
      if (currscope_ == FUNCTION) {
        // Error handling to check if it's actually an integer here
        outfile_ << "\tmovzx %bl, %rdi" << std::endl;
        outfile_ << "\tpush %rbx" << std::endl;
        outfile_ << "\tcall integerflagcheck" << std::endl;
        outfile_ << "\tpop %rbx" << std::endl;
        outfile_ << "\tmov " << std::to_string(
        symbollocations_.find(tac->arg1.reg().name())->second+8)
          << "(%rbp), %rbx" << std::endl;
        outfile_ << "\tpush %rbx\n" << std::endl;
      } else {
        // Error handling (Refactor first 4 lines into a function)
        outfile_ << "\tmovb (%rbx), %al" << std::endl;
        outfile_ << "\tmovzx %al, %rdi" << std::endl;
        outfile_ << "\tpush %rbx" << std::endl;
        outfile_ << "\tcall integerflagcheck" << std::endl;
        outfile_ << "\tpop %rbx" << std::endl;
        outfile_ << "\tmov 8(%rbx), %rcx" << std::endl;
        outfile_ << "\tpush %rcx\n" << std::endl;
      }
      break;
    case VARASSIGNLOAD:

      // Two cases one of it it's a deref, other if it's a var
      // Only add to map if it's a var
      // Case down below is for vars
      parsedstring = DereferenceParserHelper(tac->target.label().name());

      if (parsedstring.size() == 1) {
        if (symbollocations_.count(tac->target.label().name()) == 0) {
          // Add it to the map if its new
          symbollocations_.insert(std::pair<std::string, int>(
          tac->target.label().name(), -16+-16*(symbollocations_.size()+1)));
        }

        outfile_ << "\t# Loading a value into variable "
          + tac->target.reg().name() << std::endl;
        ClearRegister("rcx");
        outfile_ << "\tpop %rcx" << std::endl;

        if (currscope_ == FUNCTION) {
          // Update flags
          outfile_ << "\tmovb $0,"
            << VariableNameHelper(tac->target.label().name()) << std::endl;
          int index = symbollocations_.find(tac->target.label().name())->second;
          outfile_ << "\tmovb $1, " << std::to_string(index+1) << "(%rbp)\n";
          outfile_ << "\tmovl $0, " << std::to_string(index+2) << "(%rbp)\n";
          outfile_ << "\tmovq %rcx, " << std::to_string(index+8) << "(%rbp)\n"
            << std::endl;
        } else {
          outfile_ << "\tmov "
            << VariableNameHelper(tac->target.label().name())
            << ", %rbx" << std::endl;
          outfile_ << "\tmovb $0, (%rbx)" << std::endl;
          outfile_ << "\tmovb $1, 1(%rbx)" << std::endl;
          outfile_ << "\tmovl $0, 2(%rbx)" << std::endl;
          outfile_ << "\tmov %rcx, 8(%rbx)\n" << std::endl;
        }

        // Add it to the set, then call the print function
        if (currscope_ == GLOBAL) {
          assignmentset_.insert(tac->target.label().name());
          outfile_ << "\tmov %rcx, %rax\n" << std::endl;
          // Call on correct print function
          outfile_ << "\t# Going to print " << tac->target.label().name()
            << std::endl;
          GeneratePrintCall(tac->target.label().name()+"ascii");
          outfile_ << "\t# Returning from printing "
            << tac->target.label().name() << std::endl;
          outfile_ << std::endl;
        }
      } else {  // Needs to handle functions
        // Handle Derefs
        outfile_ << "\t# Loading an int into " << tac->target.label().name()
          << std::endl;

        // Rewrite flags
        outfile_ << "\tpop %rcx" << std::endl;
        outfile_ << "\tpop %rbx" << std::endl;
        outfile_ << "\tmovb $0, (%rbx)" << std::endl;
        outfile_ << "\tmovb $1, 1(%rbx)" << std::endl;
        outfile_ << "\tmovl $0, 2(%rbx)" << std::endl;
        outfile_ << "\tmovq %rcx, 8(%rbx)\n" << std::endl;
      }
      break;
    case FUNARGLOAD:
      // Here we're moving arguments loaded into the stack before the
      // function call and moving it to the local stack
      argumentnum = tac->arg1.value()+1;
      varname = tac->target.reg().name();
      outfile_ << "\t# Moving argument " << std::to_string(argumentnum)
        << " into the stack" << std::endl;
      outfile_ << "\tmov " << std::to_string(8+argumentnum*8)
        << "(%rbp), %rax" << std::endl;
      outfile_ << "\tmov %rax, -" << std::to_string(-8 + argumentnum*32)
        << "(%rbp)" << std::endl;
      // Remake the "Int" object
      outfile_ << "\tmovb $0, -" << std::to_string(argumentnum*32)
        << "(%rbp)" << std::endl;
      outfile_ << "\tmovb $1, -" << std::to_string(-1 + argumentnum*32)
        << "(%rbp)" << std::endl;
      outfile_ << "\tmovl $0, -" << std::to_string(-2 + argumentnum*32)
        << "(%rbp)\n" << std::endl;
      // Include it in the symbol table
      if (symbollocations_.count(varname) == 0) {
        symbollocations_.insert(std::pair<std::string, int>(varname,
        -16+-16*argumentnum));
      } else {
        std::cerr << "FUNARGLOAD VARIABLE ASSIGNMENT PROBLEM\n";
        exit(1);
      }
      break;
    case FUNRETLOAD:
      // All we do here is move the value from the function to the correct
      // variable
      if (symbollocations_.count(tac->target.reg().name()) == 0) {
        // Add it to the map then create a spot for it (if its a function)
        symbollocations_.insert(std::pair<std::string, int>(
        tac->target.reg().name(), -16+-16*(symbollocations_.size()+1)));
      }
      outfile_ << "\t# Returning from function and loading value" << std::endl;

      // Update the whole structure with the correct type
      // (always returns an int) Then move it into the value field
      // Remake the "Int" object if it's a function
      if (currscope_ == FUNCTION) {
        // Update flags
        outfile_ << "\tmovb $0,"
          << VariableNameHelper(tac->target.reg().name()) << std::endl;
        int index = symbollocations_.find(tac->target.reg().name())->second;
        outfile_ << "\tmovb $1, " << std::to_string(index+1) << "(%rbp)\n";
        outfile_ << "\tmovl $0, " << std::to_string(index+2) << "(%rbp)\n";


        // now load the value of rax in
        outfile_ << "\tmovq %rax, " << std::to_string(index+8) << "(%rbp)\n"
          << std::endl;
      } else {
        // Update flags
        outfile_ << "\tmov "
          << VariableNameHelper(tac->target.reg().name())
          << ", %rbx" << std::endl;
        outfile_ << "\tmovb $0, (%rbx)" << std::endl;
        outfile_ << "\tmovb $1, 1(%rbx)" << std::endl;
        outfile_ << "\tmovl $0, 2(%rbx)" << std::endl;
        outfile_ << "\tmov %rax, 8(%rbx)\n" << std::endl;
      }
      break;
    default:
      break;
  }
}

void CodeGen::GenerateArithmeticExpr(
  std::unique_ptr<ThreeAddressCode> tac, Type type) {
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
      outfile_ << "\tcmp %rbx, %rax" << std:: endl;
      outfile_ << "\tsetl %dl" << std::endl;
      outfile_ << "\tmovzx %dl, %rcx" << std::endl;
      outfile_ << "\tpush %rcx\n" << std:: endl;
      break;
    case LESSTHANEQ:
      outfile_ << "\t# LessThanEq Comparision";
      GenerateBinaryExprHelper(std::move(tac));
      outfile_ << "\tcmp %rbx, %rax" << std:: endl;
      outfile_ << "\tsetle %dl" << std::endl;
      outfile_ << "\tmovzx %dl, %rcx" << std::endl;
      outfile_ << "\tpush %rcx\n" << std:: endl;
      break;
    case GREATERTHAN:
      outfile_ << "\t# GreaterThan Comparision";
      GenerateBinaryExprHelper(std::move(tac));
      outfile_ << "\tcmp %rbx, %rax" << std:: endl;
      outfile_ << "\tsetg %dl" << std::endl;
      outfile_ << "\tmovzx %dl, %rcx" << std::endl;
      outfile_ << "\tpush %rcx\n" << std:: endl;
      break;
    case GREATERTHANEQ:
      outfile_ << "\t# GreaterThanEq Comparision";
      GenerateBinaryExprHelper(std::move(tac));
      outfile_ << "\tcmp %rbx, %rax" << std:: endl;
      outfile_ << "\tsetge %dl" << std::endl;
      outfile_ << "\tmovzx %dl, %rcx" << std::endl;
      outfile_ << "\tpush %rcx\n" << std:: endl;
      break;
    case EQUAL:
      outfile_ << "\t# Equals Comparision";
      GenerateBinaryExprHelper(std::move(tac));
      outfile_ << "\tcmp %rbx, %rax" << std:: endl;
      outfile_ << "\tsete %dl" << std::endl;
      outfile_ << "\tmovzx %dl, %rcx" << std::endl;
      outfile_ << "\tpush %rcx\n" << std:: endl;
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
      outfile_ << "\tand %rbx, %rax" << std:: endl;
      outfile_ << "\tpush %rax\n" << std:: endl;
      break;
    case LOGOR:
      outfile_ << "\t# LogicalOr\n";
      outfile_ << "\tpop %rbx" << std::endl;
      outfile_ << "\tpop %rax" << std::endl;
      outfile_ << "\tor %rbx, %rax" << std:: endl;
      outfile_ << "\tpush %rax\n" << std:: endl;
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

void CodeGen::GenerateBinaryExprHelper(
  std::unique_ptr<ThreeAddressCode> tac) {
  // Shouldn't have to check it it's been assigned yet
  // Lowerer should have done that
  // RegisterType arg1type = tac->arg1.reg().type();
  // RegisterType arg2type = tac->arg2.reg().type();

  // double pop from stack
  outfile_ << " btwn int, int\n";
  outfile_ << "\tpop %rbx" << std::endl;  // rbx = right
  outfile_ << "\tpop %rax" << std::endl;  // rax = left
  // }
}

std::string CodeGen::VariableNameHelper(std::string variablename) {
  if (currscope_ == GLOBAL) {
    return "$"+variablename;
  } else {
    // We're inside a function so consult the map

    if (symbollocations_.count(variablename) == 1) {
      std::string mappedname = std::to_string(
        symbollocations_.find(variablename)->second) + "(%rbp)";
      return mappedname;
    } else {
      // something bad happened
      std::cerr << "Variable name helper could not find the mapping for "
        << variablename <<" \n";
      exit(1);
    }
  }
}

std::vector<std::string> CodeGen::DereferenceParserHelper(
  std::string variablename) {
  std::istringstream issvarname(variablename);
  std::string token;
  std::string result = "";
  std::vector<std::string> resultvector;

  while (std::getline(issvarname, token, '-')) {
        result = result + token;
  }

  issvarname = std::istringstream(result);
  result = "";

  while (std::getline(issvarname, token, '>')) {
    resultvector.push_back(token);
  }
  return resultvector;
}

void CodeGen::GenerateTupleFlagCheck() {
  // rdi contains the address of what to check
  outfile_ << "tupleflagcheck:" << std::endl;
  outfile_ << "\tcmp $1, %rdi" << std::endl;
  outfile_ << "\tsete %dl" << std::endl;
  outfile_ << "\tmovzx %dl, %rcx" << std::endl;
  outfile_ << "\tcmp $0, %rcx" << std::endl;
  outfile_ << "\tje printtupletypeerror" << std::endl;
  outfile_ << "\tret" << std::endl;
}
void CodeGen::GenerateIntegerFlagCheck() {
  // rdi contains the address of what to check
  outfile_ << "integerflagcheck:" << std::endl;
  outfile_ << "\tcmp $0, %rdi" << std::endl;
  outfile_ << "\tsete %dl" << std::endl;
  outfile_ << "\tmovzx %dl, %rcx" << std::endl;
  outfile_ << "\tcmp $0, %rcx" << std::endl;
  outfile_ << "\tje printintegertypeerror" << std::endl;
  outfile_ << "\tret" << std::endl;
}
void CodeGen::GenerateExistenceCheck() {
  // rdi contains the address of what to check
  outfile_ << "existencecheck:" << std::endl;
  outfile_ << "\tcmp $1, %rdi" << std::endl;
  outfile_ << "\tsete %dl" << std::endl;
  outfile_ << "\tmovzx %dl, %rcx" << std::endl;
  outfile_ << "\tcmp $0, %rcx" << std::endl;
  outfile_ << "\tje printexistenceerror" << std::endl;
  outfile_ << "\tret" << std::endl;
}
void CodeGen::GenerateTupleSizeCheck() {
  // rdi contains the address of what to check
  // rsi contains the integer of the size of the rhs
  // basically this is just a tuple out of bounds check
  outfile_ << "tuplesizecheck:" << std::endl;
  // Move arguments into registers so we can compare them
  outfile_ << "\tmovl %edi, %eax" << std::endl;
  outfile_ << "\tmovl %esi, %ebx" << std::endl;

  // Check for greater than size of tuple
  outfile_ << "\tcmp %eax, %ebx" << std::endl;
  outfile_ << "\tsetle %dl" << std::endl;
  outfile_ << "\tmovzx %dl, %rcx" << std::endl;
  outfile_ << "\tcmp $0, %rcx" << std::endl;
  outfile_ << "\tje printsizeerror" << std::endl;

  // Check for less than 1
  outfile_ << "\tcmp $1, %ebx" << std::endl;
  outfile_ << "\tsetge %dl" << std::endl;
  outfile_ << "\tmovzx %dl, %rcx" << std::endl;
  outfile_ << "\tcmp $0, %rcx" << std::endl;
  outfile_ << "\tje printsizeerror" << std::endl;
  outfile_ << "\tret" << std::endl;
}

void CodeGen::Generate(std::vector
  <std::unique_ptr<struct ThreeAddressCode>> blocks) {
  // boiler code here
  GenerateBoiler();

  // Belongs to LHS/RHS Deref, factor it out later
  std::vector<std::string> parsedstring;

  // IR to assembly inst
  for (unsigned int i = 0; i < blocks.size(); ++i) {
    auto code = std::move(blocks[i]);
    Type opcode = code->op.opcode();

    switch (opcode) {
      case INTLOAD:
        GenerateLoadInstructions(std::move(code));
        break;
      case VARLOAD:
        GenerateLoadInstructions(std::move(code));
        break;
      case VARASSIGNLOAD:
        GenerateLoadInstructions(std::move(code));
        break;
      case FUNARGLOAD:
        GenerateLoadInstructions(std::move(code));
        break;
      case FUNRETLOAD:
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
          << ", %rax\n"<< std::endl;
        break;
      case CONDITIONAL:
        outfile_ << "\t# CONDITIONAL\n";
        outfile_ << "\tpop %rax" << std::endl;
        outfile_ << "\tcmp $" << std::to_string(code->arg1.value())
          << ", %rax\n" << std::endl;
        break;
      // Note to self abstract jumps out later
      case JUMP:
        outfile_ << "\t# JUMP\n";
        outfile_ << "\tjmp " << code->target.label().name()
          << "\n" << std::endl;
        break;
      case JEQUAL:
        outfile_ << "\t# Jump on Equal\n";
        outfile_ << "\tje " << code->target.label().name() << "\n" << std::endl;
        break;
      case JNOTEQUAL:
        outfile_ << "\t# Jump on Not Equal\n";
        outfile_ << "\tjne " << code->target.label().name()
          << "\n" << std::endl;
        break;
      case JGREATER:
        outfile_ << "\t# Jump on greater than\n";
        outfile_ << "\tjg " << code->target.label().name()
          << "\n" << std::endl;
        break;
      case JGREATEREQ:
        outfile_ << "\t# Jump on greater or equal\n";
        outfile_ << "\tjge " << code->target.label().name()
          << "\n" << std::endl;
        break;
      case JLESS:
        outfile_ << "\t# Jump on less than\n";
        outfile_ << "\tjl " << code->target.label().name()
          << "\n" << std::endl;
      case JLESSEQ:
        outfile_ << "\t# Jump on less or equal\n";
        outfile_ << "\tjle " << code->target.label().name()
          << "\n" << std::endl;
        break;
      case LABEL:
        outfile_ << code->target.label().name() << ":"   << std::endl;
        break;
      case FUNCALL:
        outfile_ << "\t# Calling Function" << std::endl;
        outfile_ << "\tcall " << code->target.label().name() << "\n"
          << std::endl;
        break;
      case FUNRETEP:
        outfile_ << "\t# FunctionRetEpilogue (Restore Stack)" << std::endl;
        outfile_ << "\tadd $" << code->arg1.value() * 16 <<
          ", %rsp" << std::endl;
        GeneratePrintCall("printfunctionresult");
        break;
      case FUNDEF:
        // Change scope
        currscope_ = FUNCTION;
        // Clear map
        symbollocations_.clear();
        break;
      case FUNPROLOGUE:
        outfile_ << "\t# Function Prologue " << std::endl;
        outfile_ << "\tpush %rbp" << std::endl;
        outfile_ << "\tmov %rsp, %rbp" << std::endl;
        // May be unneeded
        outfile_ << "\tpush %rbx" << std::endl;
        outfile_ << "\tsub $" << 16+code->arg1.value()*16 << ", %rsp\n"
          << std::endl;
        break;
      case FUNEPILOGUE:

        outfile_ << "\t# Function Epilogue " << std::endl;
        // Do a correct load lol
        outfile_ << "\tpop %rax" << std::endl;

        outfile_ << "\tpop %rbx" << std::endl;
        outfile_ << "\tmov %rbp, %rsp" << std::endl;
        outfile_ << "\tpop %rbp" << std::endl;
        outfile_ << "\tret\n" << std::endl;
        break;
      case PRINTARITH:
        outfile_ << "\tpop %rax" << std::endl;
        GeneratePrintCall("printresult");

        // BAD FIX, we "know" that the last thing will be the arithmetic expr
        // So im just going to go ahead and make the exit code here
        // later on THIS SHOULD CHANGE
        GenerateEpilogue();
        break;
      case LHSDEREFERENCE:  // Needs to handle functions (only for varchild)
        parsedstring = DereferenceParserHelper(code->target.label().name());
        outfile_ << "\t# LHSDereference of variable "
            << code->target.label().name() << std::endl;
        if (parsedstring.size() == 2) {
          // REFACTOR OUT TO HELPER FUNCTION LATER
          // ITS VERY LIKE VARIABLENAMEHELPER OR ANOTHER HELPER
          // FUNCTION CAN DO WHAT FUNCTIONS REQUIRE DITTO FOR RHS
          if (currscope_ == GLOBAL) {
            outfile_ << "\tmov " << VariableNameHelper(code->arg1.reg().name())
            << ", %rbx" << std::endl;
            // Error Handling here to check the size
            // and if it's actually a tuple
            outfile_ << "\t#Error Handling Here LHS VARCHILD" << std::endl;
            outfile_ << "\tmovb 1(%rbx), %al" << std::endl;
            outfile_ << "\tmovzx %al, %rdi" << std::endl;
            outfile_ << "\tpush %rbx" << std::endl;
            outfile_ << "\tcall existencecheck" << std::endl;
            outfile_ << "\tpop %rbx" << std::endl;

            outfile_ << "\tmovl 2(%rbx), %eax" << std::endl;
            outfile_ << "\tmovslq %eax, %rdi" << std::endl;
            outfile_ << "\tpop %rsi" << std::endl;
            outfile_ << "\tpush %rsi" << std::endl;
            outfile_ << "\tpush %rbx" << std::endl;
            outfile_ << "\tcall tuplesizecheck" << std::endl;
            outfile_ << "\tpop %rbx" << std::endl;

            outfile_ << "\tmovb (%rbx), %al" << std::endl;
            outfile_ << "\tmovzx %al, %rdi" << std::endl;
            outfile_ << "\tpush %rbx" << std::endl;
            outfile_ << "\tcall tupleflagcheck" << std::endl;
            outfile_ << "\tpop %rbx\n" << std::endl;

            // Get the actual object
            outfile_ << "\tmovq 8(%rbx), %rbx" << std::endl;

            // Get the correct index of the object
            outfile_ << "\tpop %rax" << std::endl;
            outfile_ << "\tsub $1, %rax" << std::endl;
            outfile_ << "\timul $16, %rax" << std::endl;
            outfile_ << "\tadd %rax, %rbx" << std::endl;
            outfile_ << "\tpush %rbx\n" << std::endl;
          } else {
            // Error Handling here to check the size
            // and if it's actually a tuple
            outfile_ << "\t#Error Handling Here LHS VARCHILD" << std::endl;
            outfile_ << "\tmovb " << VariableNameHelper(code->arg1.reg().name())
              << ", %al" << std::endl;
            outfile_ << "\tmovzx %al, %rdi" << std::endl;
            outfile_ << "\tcall tupleflagcheck" << std::endl;

            int index =
            symbollocations_.find(code->arg1.reg().name())->second;

            outfile_ << "\tmovb " << std::to_string(index+1) <<
              "(%rbp), %al" << std::endl;
            outfile_ << "\tmovzx %al, %rdi" << std::endl;
            outfile_ << "\tcall existencecheck" << std::endl;

            outfile_ << "\tmovl " << std::to_string(index+2) <<
              "(%rbp), %eax" << std::endl;
            outfile_ << "\tmovslq %eax, %rdi" << std::endl;
            outfile_ << "\tpop %rsi" << std::endl;
            outfile_ << "\tpush %rsi" << std::endl;
            outfile_ << "\tcall tuplesizecheck" << std::endl;

            // Get the actual object
            outfile_ << "\tmovq " << std::to_string(index+8) <<
              "(%rbp), %rbx" << std::endl;

            // Get the correct index of the object
            outfile_ << "\tpop %rax" << std::endl;
            outfile_ << "\tsub $1, %rax" << std::endl;
            outfile_ << "\timul $16, %rax" << std::endl;
            outfile_ << "\tadd %rax, %rbx" << std::endl;
            outfile_ << "\tpush %rbx\n" << std::endl;
          }
        } else {
          outfile_ << "\tpop %rcx" << std::endl;
          outfile_ << "\tpop %rbx" << std::endl;
          outfile_ << "\tpush %rcx" << std::endl;

          // Error Handling here to check the size + if it's a tuple
          outfile_ << "\t#Error Handling Here LHSDerefChild" << std::endl;
          outfile_ << "\tmovb 1(%rbx), %al" << std::endl;
          outfile_ << "\tmovzx %al, %rdi" << std::endl;
          outfile_ << "\tpush %rbx" << std::endl;
          outfile_ << "\tcall existencecheck" << std::endl;
          outfile_ << "\tpop %rbx" << std::endl;

          outfile_ << "\tmovl 2(%rbx), %eax" << std::endl;
          outfile_ << "\tmovslq %eax, %rdi" << std::endl;
          outfile_ << "\tpop %rsi" << std::endl;
          outfile_ << "\tpush %rsi" << std::endl;
          outfile_ << "\tpush %rbx" << std::endl;
          outfile_ << "\tcall tuplesizecheck" << std::endl;
          outfile_ << "\tpop %rbx" << std::endl;

          outfile_ << "\tmovb (%rbx), %al" << std::endl;
          outfile_ << "\tmovzx %al, %rdi" << std::endl;
          outfile_ << "\tpush %rbx" << std::endl;
          outfile_ << "\tcall tupleflagcheck" << std::endl;
          outfile_ << "\tpop %rbx\n" << std::endl;

          // Get the actual object
          outfile_ << "\tmovq 8(%rbx), %rbx" << std::endl;

          // Get the correct index of the object
          outfile_ << "\tpop %rax" << std::endl;
          outfile_ << "\tsub $1, %rax" << std::endl;
          outfile_ << "\timul $16, %rax" << std::endl;
          outfile_ << "\tadd %rax, %rbx" << std::endl;
          outfile_ << "\tpush %rbx\n" << std::endl;
        }

        break;
      case RHSDEFERERENCE:  // Needs to handle functions
        parsedstring = DereferenceParserHelper(code->target.label().name());
        outfile_ << "\t#Dereference of variable "
            << code->target.label().name() << std::endl;
        if (parsedstring.size() == 2) {
          // Same comment as LHS REFACTOR!!
          if (currscope_ == GLOBAL) {
            outfile_ << "\tmov " << VariableNameHelper(code->arg1.reg().name())
            << ", %rbx" << std::endl;

            // Error Handling here to check the size
            // if you're actually an tuple
            outfile_ << "\t#Error Handling Here RHS Varchild" << std::endl;
            outfile_ << "\tmovb 1(%rbx), %al" << std::endl;
            outfile_ << "\tmovzx %al, %rdi" << std::endl;
            outfile_ << "\tpush %rbx" << std::endl;
            outfile_ << "\tcall existencecheck" << std::endl;
            outfile_ << "\tpop %rbx" << std::endl;

            outfile_ << "\tmovl 2(%rbx), %eax" << std::endl;
            outfile_ << "\tmovslq %eax, %rdi" << std::endl;
            outfile_ << "\tpop %rsi" << std::endl;
            outfile_ << "\tpush %rsi" << std::endl;
            outfile_ << "\tpush %rbx" << std::endl;
            outfile_ << "\tcall tuplesizecheck" << std::endl;
            outfile_ << "\tpop %rbx" << std::endl;

            outfile_ << "\tmovb (%rbx), %al" << std::endl;
            outfile_ << "\tmovzx %al, %rdi" << std::endl;
            outfile_ << "\tpush %rbx" << std::endl;
            outfile_ << "\tcall tupleflagcheck" << std::endl;
            outfile_ << "\tpop %rbx\n" << std::endl;

            // Get the actual object (x->whatever)
            outfile_ << "\tmovq 8(%rbx), %rbx" << std::endl;

            // Get the correct offset
            outfile_ << "\tpop %rax" << std::endl;
            outfile_ << "\tsub $1, %rax" << std::endl;
            outfile_ << "\timul $16, %rax" << std::endl;
            outfile_ << "\tadd %rax, %rbx" << std::endl;
            outfile_ << "\tpush %rbx" << std::endl;

            if (code->arg2.reg().name().compare("Child") == 0) {
              // If you're a child don't need to do anything
            } else {
              // Otherwise you're the parent so check that what you're
              // accessing is actually an integer
              // get the value stored and push it on the stack

              outfile_ << "\tpop %rbx" << std::endl;
              // More error handling
              outfile_ << "\t#Check if it's an int RHS DEREF" << std::endl;
              outfile_ << "\tmovb (%rbx), %al" << std::endl;
              outfile_ << "\tmovzx %al, %rdi" << std::endl;
              outfile_ << "\tpush %rbx" << std::endl;
              outfile_ << "\tcall integerflagcheck" << std::endl;
              outfile_ << "\tpop %rbx\n" << std::endl;
              // Get the correct offset
              outfile_ << "\tadd $8, %rbx" << std::endl;
              outfile_ << "\tmov (%rbx), %rcx" << std::endl;
              outfile_ << "\tpush %rcx\n" << std::endl;
            }
          } else {
            // Error Handling here to check the size
            // and if it's actually a tuple
            outfile_ << "\t#Error Handling Here RHS VARCHILD" << std::endl;
            outfile_ << "\tmovb " << VariableNameHelper(code->arg1.reg().name())
              << ", %al" << std::endl;
            outfile_ << "\tmovzx %al, %rdi" << std::endl;
            outfile_ << "\tcall tupleflagcheck" << std::endl;

            int index =
            symbollocations_.find(code->arg1.reg().name())->second;

            outfile_ << "\tmovb " << std::to_string(index+1) <<
              "(%rbp), %al" << std::endl;
            outfile_ << "\tmovzx %al, %rdi" << std::endl;
            outfile_ << "\tcall existencecheck" << std::endl;

            outfile_ << "\tmovl " << std::to_string(index+2) <<
              "(%rbp), %eax" << std::endl;
            outfile_ << "\tmovslq %eax, %rdi" << std::endl;
            outfile_ << "\tpop %rsi" << std::endl;
            outfile_ << "\tpush %rsi" << std::endl;
            outfile_ << "\tcall tuplesizecheck" << std::endl;

            // Get the actual object
            outfile_ << "\tmovq " << std::to_string(index+8) <<
              "(%rbp), %rbx" << std::endl;

            // Get the correct index of the object
            outfile_ << "\tpop %rax" << std::endl;
            outfile_ << "\tsub $1, %rax" << std::endl;
            outfile_ << "\timul $16, %rax" << std::endl;
            outfile_ << "\tadd %rax, %rbx" << std::endl;
            outfile_ << "\tpush %rbx\n" << std::endl;

            if (code->arg2.reg().name().compare("Child") == 0) {
              // If you're a child don't need to do anything
            } else {
              // Otherwise you're the parent so check that what you're
              // accessing is actually an integer
              // get the value stored and push it on the stack

              outfile_ << "\tpop %rbx" << std::endl;
              // More error handling
              outfile_ << "\t#Check if it's an int RHS DEREF" << std::endl;
              outfile_ << "\tmovb (%rbx), %al" << std::endl;
              outfile_ << "\tmovzx %al, %rdi" << std::endl;
              outfile_ << "\tpush %rbx" << std::endl;
              outfile_ << "\tcall integerflagcheck" << std::endl;
              outfile_ << "\tpop %rbx\n" << std::endl;
              // Get the correct offset
              outfile_ << "\tadd $8, %rbx" << std::endl;
              outfile_ << "\tmov (%rbx), %rcx" << std::endl;
              outfile_ << "\tpush %rcx\n" << std::endl;
            }
          }
        } else {
          outfile_ << "\tpop %rcx" << std::endl;
          outfile_ << "\tpop %rbx" << std::endl;
          outfile_ << "\tpush %rcx" << std::endl;
          // Error Handling here to check the size + if you're actually an tuple
          outfile_ << "\t#Error Handling Here DerefChild" << std::endl;
          outfile_ << "\tmovb 1(%rbx), %al" << std::endl;
          outfile_ << "\tmovzx %al, %rdi" << std::endl;
          outfile_ << "\tpush %rbx" << std::endl;
          outfile_ << "\tcall existencecheck" << std::endl;
          outfile_ << "\tpop %rbx" << std::endl;

          outfile_ << "\tmovl 2(%rbx), %eax" << std::endl;
          outfile_ << "\tmovslq %eax, %rdi" << std::endl;
          outfile_ << "\tpop %rsi" << std::endl;
          outfile_ << "\tpush %rsi" << std::endl;
          outfile_ << "\tpush %rbx" << std::endl;
          outfile_ << "\tcall tuplesizecheck" << std::endl;
          outfile_ << "\tpop %rbx" << std::endl;

          outfile_ << "\tmovb (%rbx), %al" << std::endl;
          outfile_ << "\tmovzx %al, %rdi" << std::endl;
          outfile_ << "\tpush %rbx" << std::endl;
          outfile_ << "\tcall tupleflagcheck" << std::endl;
          outfile_ << "\tpop %rbx\n" << std::endl;

          // Get the actual object (x->whatever)
          outfile_ << "\tmovq 8(%rbx), %rbx" << std::endl;

          // Get the correct offset
          outfile_ << "\tpop %rax" << std::endl;
          outfile_ << "\tsub $1, %rax" << std::endl;
          outfile_ << "\timul $16, %rax" << std::endl;
          outfile_ << "\tadd %rax, %rbx" << std::endl;
          outfile_ << "\tpush %rbx" << std::endl;

          if (code->arg2.reg().name().compare("Child") == 0) {
            // If you're a child don't need to do anything
          } else {
            // Otherwise you're the parent so check that what you're
            // accessing is actually an integer
            // get the value stored and push it on the stack

            outfile_ << "\tpop %rbx" << std::endl;
            // More error handling
            outfile_ << "\t#Check if it's an int RHS DEREF" << std::endl;
            outfile_ << "\tmovb (%rbx), %al" << std::endl;
            outfile_ << "\tmovzx %al, %rdi" << std::endl;
            outfile_ << "\tpush %rbx" << std::endl;
            outfile_ << "\tcall integerflagcheck" << std::endl;
            outfile_ << "\tpop %rbx\n" << std::endl;
            // Get the correct offset
            outfile_ << "\tadd $8, %rbx" << std::endl;
            outfile_ << "\tmov (%rbx), %rcx" << std::endl;
            outfile_ << "\tpush %rcx\n" << std::endl;
          }
        }
        break;
      case NEWTUPLE:  // Needs to handle functions (maybe unneeded)
        outfile_ << "\t# Making a tuple for variable: "
          << code->target.label().name() << std::endl;
        outfile_ << "\tpop %rcx" << std::endl;
        outfile_ << "\tpush %rcx" << std::endl;
        outfile_ << "\timul $16, %rcx" << std::endl;
        outfile_ << "\tmov %rcx, %rdi" << std::endl;
        outfile_ << "\tcall malloc" << std::endl;

        outfile_ << "\tpop %rcx" << std::endl;
        outfile_ << "\tpop %rbx" << std::endl;

        // 100% theres a better way to do this
        if (DereferenceParserHelper(code->target.label().name()).size() == 1
          && currscope_ == FUNCTION) {
          // Rewrite flags
          outfile_ << "\tmovb $1,"
            << VariableNameHelper(code->target.label().name()) << std::endl;
          int index =
            symbollocations_.find(code->target.label().name())->second;
          outfile_ << "\tmovb $1, " << std::to_string(index+1) << "(%rbp)\n";
          outfile_ << "\tmovl %ecx, " << std::to_string(index+2) << "(%rbp)\n";
          outfile_ << "\tmovq %rax, " << std::to_string(index+8) << "(%rbp)\n"
            << std::endl;
        } else {
          // Rewrite flags
        outfile_ << "\tmovb $1, (%rbx)" << std::endl;
        outfile_ << "\tmovb $1, 1(%rbx)" << std::endl;
        outfile_ << "\tmovl %ecx, 2(%rbx)" << std::endl;

        // Write pointer given by malloc into value field
        outfile_ << "\tmovq %rax, 8(%rbx)\n" << std::endl;
        }
        break;
      case VARCHILDTUPLE:  // May need to handle functions
        if (symbollocations_.count(code->target.label().name()) == 0) {
          // Add it to the map then create a spot for it (if its a function)
          symbollocations_.insert(std::pair<std::string, int>(
          code->target.label().name(), -16+-16*(symbollocations_.size()+1)));
        }
        outfile_ << "\t# Getting value of " << code->target.label().name()
          << std::endl;
        outfile_ << "\tmov " <<
          VariableNameHelper(code->target.label().name()) << ", %rbx"
          << std::endl;
        outfile_ << "\tpush %rbx\n" << std::endl;
        break;
      default:
        std::cerr << "Inside Generate and something really bad happened\n";
        std::cerr << "Unknown opcode " << std::to_string(opcode) << std::endl;
        exit(1);
    }
  }

  GeneratePrinter();
  outfile_.close();
}

}  // namespace backend
}  // namespace cs160
