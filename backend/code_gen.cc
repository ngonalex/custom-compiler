
// #include "backend/code_gen.h"

// namespace cs160 {
// namespace backend {

// void CodeGen::GenerateEpilogue() {
//   outfile_ << "\tmov $60, %rax\n\txor %rdi, %rdi\n\tsyscall" << std::endl;
//   outfile_.close();
// }

// void CodeGen::GenerateBoiler() {
//   outfile_ << "\t.global _start" << std::endl;
//   outfile_ << "\t.text" << std::endl;
//     outfile_ << "_start:" << std::endl;
// }

// void CodeGen::ClearRegister(std::string reg) {
//   outfile_ << "\txor %" + reg + ", %" + reg << std::endl;
// }

// void CodeGen::Generate(std::vector<struct ThreeAddressCode*> blocks) {
//   // boiler code here

//   // IR to assembly inst
//   for (unsigned int i = 0; i < blocks.size(); ++i) {
//     ThreeAddressCode* code = blocks[i];
//     std::string OpCode = code->op;
//     if (OpCode == "load") {
//       // outfile_ << "\t#Storing " + code->arg1 + " into rcx" << std::endl;
//       outfile_ << "\tmov $" + code-> arg1 + ", %rcx" << std::endl;
//       outfile_ << "\tpush %rcx" << std::endl;

//     } else if (OpCode == "+") {
//         // Load arg1,arg2 then add them into target
//         // outfile_ << "\t#Adding << std::endl;
//         outfile_ << "\tpop %rbx" << std::endl;  // rbx = right
//         outfile_ << "\tpop %rax" << std::endl;  // rax = left
//         ClearRegister("rcx");
//         outfile_ << "\tadd %rax, %rcx\nadd %rbx, %rcx" << std::endl;
//         outfile_ << "\tpush %rcx" << std::endl;

//     } else if (OpCode == "-") {
//         // Load arg1,arg2 then add them into target
//         outfile_ << "\tpop %rax" << std::endl;  // rbx = right
//         outfile_ << "\tpop %rcx" << std::endl;  // rax = left
//         outfile_ << "\tsub %rax, %rcx" << std::endl;
//         outfile_ << "\tpush %rcx" << std::endl;
//     } else if (OpCode == "*") {
//         outfile_ << "\tpop %rbx" << std::endl;  // rbx = right
//         outfile_ << "\tpop %rcx" << std::endl;  // rcx = left
//         outfile_ << "\timul %rbx, %rcx" << std::endl;
//         outfile_ << "\tpush %rcx" << std::endl;
//     } else if (OpCode == "/") {
//         // Load dividend (arg1) into %rax
//         ClearRegister("rdx");
//         outfile_ << "\tpop %rbx" << std::endl;
//         outfile_ << "\tpop %rax" << std::endl;
//         outfile_ << "\tcqto" << std::endl;  // indicating its a signed division
//         outfile_ << "\tidiv %rbx" << std::endl;
//         outfile_ << "\tpush %rax" << std::endl;
//     }
//   }
// }

// }  // namespace backend
// }  // namespace cs160
