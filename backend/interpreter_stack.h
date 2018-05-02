#include <string>

enum StackType { INTEGER, VARIABLE, CONDITIONAL, NONE };

union Data {
    int as_integer;
    std::string as_variable;
    bool as_conditional;
};

struct StackValue {
    StackType type;
    Data data;
    StackValue() : type(NONE), data(NULL);
};