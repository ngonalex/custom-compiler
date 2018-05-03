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
  outfile_ << "\t.ascii \"\\n\""  << std::endl;

  GenerateResult();

  //Generate Unique Printers here
  std::set<std::string>::iterator it;
  for (it = variableset_.begin(); it != variableset_.end(); ++it) {
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
  outfile_ << "\tmov $" + variablename +"ascii, %rsi" << std::endl;
  outfile_ << "\tmov $" + std::to_string(23+variablename.length()) +  +", %rdx" << std::endl;
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
  //Creates a .data section for variables
  outfile_ << ".data " << std::endl;
  for ( auto iter = variableset.begin(); iter!=variableset.end(); ++iter) {
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

void CodeGen::Generate(std::vector
  <std::unique_ptr<struct ThreeAddressCode>> blocks) {
  // boiler code here
  GenerateBoiler();

  // IR to assembly inst
  for (unsigned int i = 0; i < blocks.size(); ++i) {
    auto code = std::move(blocks[i]);
    Opcode opcode = code->op;

    // Two different loads now, one for reg <- int, 
    // another variable <- arithmetic
    if (opcode.opcode() == LOAD) {
      // outfile_ << "\t#Storing " + code->arg1 + " into rcx" << std::endl;
      if (code->arg1.optype() == INT) {
        outfile_ << "\tmov $" + std::to_string(code->arg1.value())
          + ", %rcx" << std::endl;
        outfile_ << "\tpush %rcx" << std::endl;
      } else {

          ClearRegister("rbx");
          outfile_ << "\tpop %rbx" << std::endl;
          outfile_ << "\tmov %rbx, " << code->target.name() << "" << std::endl;
          outfile_ << "\tpush %rbx" << std::endl;
    
          // Add it to the set, then call the print function
          variableset_.insert(code->target.name());
          outfile_ << "\tmov %rbx, %rax" << std::endl;
          // Call on correct print function
          outfile_ << "\tcall print" + code->target.name() << std::endl;
      }

    } else if (opcode.opcode() == ADD) {
        // Load arg1,arg2 then add them into target
        // outfile_ << "\t#Adding << std::endl;
        outfile_ << "\tpop %rbx" << std::endl;  // rbx = right
        outfile_ << "\tpop %rax" << std::endl;  // rax = left
        ClearRegister("rcx");
        outfile_ << "\tadd %rax, %rcx\n\tadd %rbx, %rcx" << std::endl;
        outfile_ << "\tpush %rcx" << std::endl;

    } else if (opcode.opcode() == SUB) {
        // Load arg1,arg2 then sub them into target
        outfile_ << "\tpop %rax" << std::endl;  // rbx = right
        outfile_ << "\tpop %rcx" << std::endl;  // rax = left
        outfile_ << "\tsub %rax, %rcx" << std::endl;
        outfile_ << "\tpush %rcx" << std::endl;
    } else if (opcode.opcode() == MULT) {
        outfile_ << "\tpop %rbx" << std::endl;  // rbx = right
        outfile_ << "\tpop %rcx" << std::endl;  // rcx = left
        outfile_ << "\timul %rbx, %rcx" << std::endl;
        outfile_ << "\tpush %rcx" << std::endl;
    } else if (opcode.opcode() == DIV) {
        // Load dividend (arg1) into %rax
        ClearRegister("rdx");
        outfile_ << "\tpop %rbx" << std::endl;
        outfile_ << "\tpop %rax" << std::endl;
        outfile_ << "\tcqto" << std::endl;  // indicating its a signed division
        outfile_ << "\tidiv %rbx" << std::endl;
        outfile_ << "\tpush %rax" << std::endl;
    } else if (opcode.opcode() == LESSTHAN) {
        outfile_ << "\tpop %rbx" << std::endl;
        outfile_ << "\tpop %rax" << std::endl; 
        outfile_ << "\tcmp %rbx, %rax" << std:: endl;
        outfile_ << "\tsetl %dl" << std::endl;
        outfile_ << "\tmovzx %dl, %rcx" << std::endl;
        outfile_ << "\tpush %rcx" << std:: endl;
    } else if (opcode.opcode() == LESSTHANEQ) {
        outfile_ << "\tpop %rbx" << std::endl;
        outfile_ << "\tpop %rax" << std::endl; 
        outfile_ << "\tcmp %rbx, %rax" << std:: endl;
        outfile_ << "\tsetle %dl" << std::endl;
        outfile_ << "\tmovzx %dl, %rcx" << std::endl;
        outfile_ << "\tpush %rcx" << std:: endl;
    } else if (opcode.opcode() == GREATERTHAN) {
        outfile_ << "\tpop %rbx" << std::endl;
        outfile_ << "\tpop %rax" << std::endl; 
        outfile_ << "\tcmp %rbx, %rax" << std:: endl;
        outfile_ << "\tsetg %dl" << std::endl;
        outfile_ << "\tmovzx %dl, %rcx" << std::endl;
        outfile_ << "\tpush %rcx" << std:: endl;
    } else if (opcode.opcode() == GREATERTHANEQ) {
        outfile_ << "\tpop %rbx" << std::endl;
        outfile_ << "\tpop %rax" << std::endl; 
        outfile_ << "\tcmp %rbx, %rax" << std:: endl;
        outfile_ << "\tsetge %dl" << std::endl;
        outfile_ << "\tmovzx %dl, %rcx" << std::endl;
        outfile_ << "\tpush %rcx" << std:: endl;
    } else if (opcode.opcode() == EQUAL) { 
        outfile_ << "\tpop %rbx" << std::endl;
        outfile_ << "\tpop %rax" << std::endl; 
        outfile_ << "\tcmp %rbx, %rax" << std:: endl;
        outfile_ << "\tsete %bl" << std:: endl;
        outfile_ << "\tmovzx %dl, %rcx" << std::endl;
        outfile_ << "\tpush %rcx" << std:: endl;
    } else if (opcode.opcode() == LOGAND) { 
        outfile_ << "\tpop %rbx" << std::endl;
        outfile_ << "\tpop %rax" << std::endl; 
        outfile_ << "\tand %rbx, %rax" << std:: endl;
        outfile_ << "\tpush %rax" << std:: endl;
    } else if (opcode.opcode() == LOGOR) {  
        outfile_ << "\tpop %rbx" << std::endl;
        outfile_ << "\tpop %rax" << std::endl; 
        outfile_ << "\tor %rbx, %rax" << std:: endl;
        outfile_ << "\tpush %rax" << std:: endl;
    } else if (opcode.opcode() == LOGNOT) { 
        outfile_ << "\tpop %rbx" << std::endl;
        outfile_ << "\tnot %rbx" << std:: endl;
        outfile_ << "\tpush %rbx" << std:: endl;
    } else if (opcode.opcode() == LOOP) {  
        
    } else if (opcode.opcode() == CONDITIONAL) { 
    } else if (opcode.opcode() == JUMP) {  
    }
  }
  //This will probably change later, call on the print function for the
  // Arith Expr
  outfile_ << "\tpop %rax" << std::endl;
  outfile_ << "\tcall printarith" << std::endl;

  GenerateEpilogue();
  GeneratePrinter();
  outfile_.close();
}

}  // namespace backend
}  // namespace cs160
