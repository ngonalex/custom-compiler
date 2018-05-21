#include <string>
#include <regex>

std::string preparse_Program(std::string program){
    program = std::regex_replace(program, std::regex("\\( +"), "(");
    program = std::regex_replace(program, std::regex(" +\\)"), ")");
    program = std::regex_replace(program, std::regex("; +"), ";");
    program = std::regex_replace(program, std::regex("( *)\\+( *)"), "+");
    program = std::regex_replace(program, std::regex("( *)-( *)"), "-");
    program = std::regex_replace(program, std::regex("( *)/( *)"), "/");
    program = std::regex_replace(program, std::regex("( *)\\*( *)"), "*");
    return program;
}