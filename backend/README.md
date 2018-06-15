# Backend Documentation #

**Development:**

1) Mainly developed on a Ubuntu 17.04 machine. Our codegen tests will not work on mac/windows machines due to things like absolute addressing. If bazel is installed on a csil machine, our program should work.

**Testing:**
- to run all tests: bazel test backend/backend_test
- to run codegen tests: bazel test backend/codegen_test
- to run lowerer_tests: bazel test backend/lowerer_test
- to run control_graph tests: bazel test backend/control_flow_graph_test
- to run optimizer tests: bazel test backend/optimizer_test

Additional notes about tests. Optimizer uses EXPECT_LE to compare the file size of the non optimized file from codegen and the optimized file from codegen. Because it's a LE (LessThanEqualTo) that means that it will always "pass" as if the optimizer did not do anything, the two files will be identical. Run bazel build backend/optimizer_test && bazel-bin/backend/optimizer_test instead to see output that shows the file size of both the optimized file and the non optimized file.

Our project is split into 4 parts.
1) Intermediate Representation
2) Lowerer
3) Code Generation
4) Control Flow Graph + Optimizer

# Intermediate Representation #

Files: ir.h
Basically contains all of our useful enums and classes that are used to make up the ThreeAddressCode data structure.
The way we broke down our ThreeAddressCode structure is  
- Target class, can either be a Label (for jumps/functions/loops/conditionals) or a Register (For ArithExprs)
- Label class, a string that contains what label to jump to
- Register class, holds a string a virtual register, a variable name, or a dereference as well as the type
- Operand class, either a integer or a Register
- Opcode class, holds an opcode defined by our OpCode Enum which signals to CodeGen what instruction to perform

There's nothing too difficult here, but it may be somewhat confusing on why we decided to have Target/Operand hold two different classes rather than be a parent and have classes inherit from it. We based this structure off of the way Ben created his tokens in the example-code repository because we didn't want to deal with object slicing. If we had more time we definitely would revise our class structure as it can get very confusing to deal with though.

# Lowerer #

Files: lowerer_visitor.cc, lowerer_visitor.h
Files it depends on: ir.h, helper_struct.h
Tests: lowerer_visitor_test* 

The way our lowerer works is it takes in an AST and creates a vector of ThreeAddressCodes by using the visitor design provided by abstract_syntax_tree.

We'll break our lowerer_visitor into what it does for each version.

**V1**

V1 does two things
1) Responsible for signaling instructions for Codegen to load an integer/dereference/variable into the stack
2) Signal to codegen to pop two things off the stack and do the corresponding operation on them as provided by the opcode, then pushing the result onto the stack

**V2**

V2 is an extension of V1 except now it needs to handle LeftHandSide variable/Dereferences present in ArithmeticExprs

All V2 really is is differentiating between a LHS and a RHS variable. If it's a LHS variable signal to codegen that it needs to pop the address of the variable and load something into it.
If it's a RHS variable signal to codegen to load from memory and use that value in resulting calculations.

**V3**

Loops
- Tell codegen how to evaluate the guard, create a label for the loop name, create instructions for the loop body, then jumpback to either the loop or to main.

Conditionals
- Tell codegen how to evaluate the guard, create a label for the true, false, continueblock (main), then create instructions for the true/false block

Relational/Logical Exprs
- Similar to how our lowerer handles Add/Sub/Mul/Div, all it does is signal to codegen to pop two things of the stack compare them based on the opcode and push the result on the stack.

**V4**

Function calls
- Iterate through the arguments vector which tells CodeGen to load in arguments into the stack, then tell Codegen to create a "call instruction", then which variable to load in the result of the function

Function definitions
- Iterate through the function definition vector and tell codegen to create a label for the function as well as which instructions to put in each function

**V5**

Dereference
- Similar to how variables are handled, dereferences are handled by differentiating on if they are a RHS or a LHS and signaling to codegen to do a different action accordingly. You can look at our OpCode Enum in ir.h to see a RHS and a LHS dereference opcode instruction.
- It does one key thing differently though which is it will signal to codegen where the dereference is in the chain.
e.g.
x->1 vs x->1->1
There are two types of different dereferences which are a "parent" dereference and a "child" dereference. We need to figure this stuff out here, because codegen handles the two cases differently. If it's a child dereference then codegen needs to know to push the address of where the child lies so the next dereference can access further into the tuple. If it's a parent dereference however this changes based on if it's a RHS or a LHS defererence. If it's a RHS dereference then access the value inside the dereference and push it on the stack otherwise push the address so it can write into the tuple.

Some extra features our lowerer does:

1) Catches unassigned variable errors
  e.g y = x where x has never been assigned yet, our lowerer will catch this error and exit the program by using a global set that is created as variables are assigned.

2) Catches unassigned variables in branches/conditionals
If you have a program where there's a new variable created in one branch and is not created in the other branch, our lowerer will note it, but will not throw an error. It will only throw an error when it detects that variable being used outside of the branch.
It does this by retaining a globalset and a totalset. Totalset is the set of ALL variables seen in the program in the current scope while globalset is variables that exist in EVERY branch. The only place that creates differences in the two are in conditionals or in loops. We use functions called GetSetDifference and GetSetIntersection to do this.

e.g.
if (...) {
  x = 5 <- x is assigned here, but not in the false branch
  y = x
} else {
  y = 5
}

If the variable "x" is never used again then our program will continue. However in the case that "x" is used without being assigned to first, our program will throw an error.

3) Incorrect number of function arguments
  If the number of arguments supplied into a function call != to the number of arguments a function call expects, we throw an error. This is done by counting the number of arguments for a function call and storing it in a map of function_name -> # of arguments. Then when we see a function call anywhere in the program we consult the map if the number of arguments provided in the function call is equal to the number of arguments that the map tells us.

4) Redefining Functions
  Using the map we created as described in number 3) when we're creating the function definitions for Codegen, we check if the function already exists in the map as our language has no notion of overloading functions. The AAST notes that function names should be unique, but it's not enforced in anyway, so this is our way of doing it.

5) Undeclared Functions
  If the function name that is being referred to in a function call does not exist in our map we throw an error.

# Code Generation #

Files: code_gen.cc, code_gen.h
Files it depends on: ir.h, helper_struct.h, lowerer_visitor.cc, lowerer_visitor.h
Tests: codegen_test.cc

The way our codegen works is it's dynamically typed so it figures out the types at runtime. The way objects are stored in our language is each object is that each object is 16 bytes where byte 0 is the type flag (0 for int, 1 for tuple/pointer), byte 1 is the existence flag (0 if there is no object, 1 if there is one), byte 2-5 is the size flag (0 for ints, 1 -> 2^4-1 for tuples depending on the size asked for by AssignmentFromNewTuple), and bytes 8-15 being either the valued of the integer (64 bit signed integer) or the address of the tuple in the heap (64 bit address).

Some other things to note:
Our language will type check meaning that things like x->1 + 1 (if x->1 is a tuple) are not valid. In any Arithmetic/Relational/Logical Expr, if any of the arguments are not integers we throw a type error and exit. Our language will protect against tuple of out of bounds errors. Anytime a dereference is accessed we check if the argument provided is within range of 1 -> size (stored as metadata in the first 8 bytes of the object). Also we catch things like accessing a tuple that doesn't exist, so if x->1->1 has not been assigned or x->1 is actually an integer, we throw an error by checking first if it exists (existence flag) then if it's a tuple (if type flag is 1 for tuple).
We also guard against using tuple creation with bogus arguments (cannot create a tuple of size 0 or less). Because we never garbare collect our program doesn't have dangling pointers, so things like x = tuple (2), y = x, x = 5 is totally valid as y will just point to the origin tuple that x contained after x is reassigned to the variable 5.

Also this is important to note that there are reserved keywords in our language. There's an issue up on github that details this, but it's not handled in backend. So if a user were to use any of the reserved keywords like heap, bumpptr, returnobj, their program would crash or have undefined behavior.

**V1**
- loading in integers -> creates flags to indicate it exists and it's an integer (sets type flag to 0, and existence flag to 1) and then pushes it on the stack, then pushes the actual value to the stack
- Add/Mult/Div/Sub -> pops 4 things off the stack (Each object is split into two 64 bit spaces on the stack, one for the flags, one for the object itself)
                   -> checks the existence + type flag of the two objects on the stack (Both must exist and both must be integers as we don't suppose tuple addition otherwise throw an error), then perform the corresponding operator on it and push flags + object back on the stack

**V2**
- Variables -> stored in the static segment, where each variable is given a 16 bytes of space in it (global). If it belongs to a function, then it's stored in the stack (more on this in the v4 section)
- LHS variables -> get the address of the variable using a map. If it's a global then it just returns it's name (e.g. variable "x" is $x). If it's in a function then it returns the offset in the stack where it is, then pop the                      top thing off the stack (should be the result of the variable ArithmeticExprs, and write into memory/stack)
- RHS variables -> get the address of the variable using a map, then push the flags + value of the variable onto the stack

**V3**
- Relational Exprs -> We use the instruction cmp to compare two values, then use an instruction setcc which will set a byte register to either 0 or 1 depending on what "cc" was. e.g if we want LessThan we would do setl. Then we                        sign-extend the byte register to a 8 byte register (64 bits) and push that and an integer flag register to the stack. Basically this allows us to emulate booleans using "0s" and "1s" rather than having to                         create a "boolean type" in our language.

- Logical Exprs -> Use corresponding provided instruction in x86 (and, or). For LogicalNot, we use xor to flip the byte provided to us because we know that the thing at the top of the stack is either a 0 or a 1.

- Loops -> Pop the top two things off the stack (Flags + Boolean value from the guard), then we compare it to 0, if it's equal to 0 then we jump to "main" otherwise we continue (does the loop body)
        -> At the end it automatically jumps back to the looplabel, and does the guard again

- Conditional -> Pop top two things off the stack and evaluate it. If it's == to 0 then we jump to the false branch, otherwise we continues (falls in the true branch), Eventually both branches converge and have a jump to main.

**V4**

-Function calls -> Loads all the arguments on the stack, then calls the corresponding function (it has a label)
                -> when it returns, we have a variable in the static segment called "returnobj", unpack that object and load the flags + object into the corresponding variable defined by the function call return variable. Then 
                -> clear the returnobj so it can be used in the next function call then restore the stack (misaligned because we loaded in arguments into the stack)
              
- Function def  -> Save the old bump ptr (so we can return correctly) then move the stack pointer to the bump ptr. 
                -> Then we create space on the stack based on the number of variables needed in the function. This is predetermined by the lowerer by keeping track of all the unique variables used in the function (totalset).
                -> Then we load the variables pushed onto the stack by the function call and move it into the local stack (variables in rbp+X to rbp-Y)
                -> Run the statements normally, until we return which is where we load the ArithmeticExpr to be returned into "returnobj", reset the stack and bump ptr then call return

- Variable/Dereference, The way this works is codegen has a map of where each variable is mapped to. So it knows where to access each variable in the stack. Anytime it sees a variable instead of accessing it from the static now it uses the stack. Once it's on the stack there's no difference in how Arith/Logical/Relational Exprs interact with them. The only thing that changes here in loading from and into variables.

**V5**
- AssignmentFromNewTuple -> We have a heap allocator subroutine called "newtuple" defined in GenerateCreateNewTuple(). The way it works is we have a static segment variable called "heap" and a 8 byte variable called "bumpptr" 
                         -> which is reserved memory acting as our heap and our head of freelist. One thing we did not implement is garbage collection, so objects are never reclaimed. In addition there is no error handling when
                         -> bumpptr > size of heap. Our program will just enter into the realm of undefined behavior. Everytime newtuple is called it takes in an argument of how much space to allocate. It takes that argument 
                         -> returns the current bumpptr then increments the bumpptr to the space requested
                         -> When newtuple returns the register rax will contain the address of the heap and that's placed in the the variable that is requested.

- Dereferences -> Split into 4 things
1) Parent Dereference + LHS Dereference
2) Child Dereference + LHS Dereference
3) Parent Dereference + RHS Dereference
4) Child Dereference + RHS Dereference

Child/Parent + LHS Dereference and Child RHS Dereference are handled the same. The address of the base variable ("x" if it's x->1) is pushed on the stack and we keep accessing deeper, so if x->5->1 is requested we would access the variable x then add 16*(5-1) to that address to access x->5, push that to the stack then access x->5->1 by offsetting that address.

Parent Dereference + RHS Dereference takes an extra step of accessing the actual value of the tuple rather than just the address and then pushing that on the stack.

# Control Flow Graph && Optimizer #
Files: control_flow_graph.cc, control_flow_graph.h
Files it depends on: ir.h, helper_struct.h, lowerer_visitor.cc, lowerer_visitor.h
Tests: control_flow_graph.cc

The Control Flow Graph takes an IR and attempts to optimizes it using Dead Code Elimination
1) Takes the vector of IR and creates a vector of Control Flow Graph Nodes
  1a) Each Control Flow Graph Node has a subset (in the form of a vector) of the vector of IR
2) Takes the Control Flow Graph Nodes and recursively build a graph
3) Apply the Optimize() function

Each step is fairly complicated in its actual implementation however:
In order to build the nodes, the Control Flow Graph iterates through the vector of unique_ptr ThreeAddressCodes.
As the Graph iterates through each vector, it builds a new unique_ptr ThreeAddressCodes vector, creating a "local block" for each node.
On each iteration, the operation type of the ThreeAddressCode is checked, and on certain operations, it then breaks off and creates a node. These operations include: CONDITIONAL, LOOP, JUMP.
Also it checks if it has reached the end of the IR to make a node.
It makes blocks at these points because these are points where the instructions can jump to another location. Creating these blocks allow for a local optimization.

Since the nodes created are unique_ptr ControlFlowGraphNodes, it is impossible to directly point nodes at each other since  2 nodes can point at a single one. Thus I used a vector of "Edges", a self definied struct that contained a from -> to int pair using the creation order of each node as the numerical identifier, and an edge type (which is important in recursely traversing the graph in reverse).
The recursion begins with the first node in the vector (which should be the node that contains the first few lines of IR), and then checks if it is a CONDITIONAL or a LOOP.
Depending on what it sees, it'll make 3-4 recursive calls. These recursive calls allow the ControlFlowGraph to handle nested conditionals or nested loops regardless. Because of the nature of the graph, you have to connect 6 nodes if it is a CONDITIONAL or 5 if it is a LOOP, while there is a possibility that 2 of those nodes are the same as 2 of the other nodes. This is due to the fact that you have to connect the top and bottom part of another conditional/loop if it ends up being nested

Labeling the Edge Type was important in creating the reverse traversal needed for Optimize().
When Optimize is called, it calls a RecursiveFind by using a helper. The RecursiveFind is designed to pass the current live_set to the next neccessarily block. This allows it to handle a global optimization.
For conditionals, it is important that both the true and false branch make their local optimizations, and then combine their local set, and then pass it onto the guard who can then make a local optimization. Because you have to merge the vector, the recursive call to the functions can't possibly figure out what to combine. Thus they prematurely return and give their vectors back to the original function that called them to merge their vectors. They also return the guard that they were supposed to give their merged vectors too, allowing the last recursive call to happen.
Loops are much easier, as they only have to pass it off to the next node. They do require a visited vector so they don't infinitely pass their live_sets. Edge Types must be used determine what kind of operation must be done, because it is impossible to check node type since node type can only be used to determine direction going forward, not backward.
Overall, this reverse recurse was incredibly difficult because it was impossible to figure out what operation has to be done unless more information was given (which is why I had to create a edge struct and label each edge)

Finally now since it can figure out how to pass the local set between the nodes, it can apply the liveliness analysis in the local blocks.
The function reverse iterates through the vector of ThreeAddressCode. It checks if the target (LHS) is in the liveset. If it isn't and also isn't being use by the args (RHS), then that means that the variable is dead and can be eliminated. It sets the unique_ptr to NULL (which code_gen knows to ignore). 
Then it checks the args (RHS) to see if they're in the liveset. If they are then they're ignored, but if they aren't, then they're placed into the live_set, meaning they won't get optimized out the next time they're assigned.
This operation is also done on virtual registers, allowing for the naive implementation that ThreeAddressCode use to be completely cleaned up.

Finally it returns back to the user the IR by rebuilding a vector of ThreeAddressCode by iterating over the Nodes and reclaiming ownership of their IRs

There are some additional functionality, including some debug functions and a GetOutput() function. 

The Recursion for this part was only possible by using unique_ptr's .get(), returning a direct pointer back. Since the recursion didn't need the ownership, it made sense to use them. Also it would've been incredibly difficult to recursively pass unique_ptrs, because when the functions are popped out of the stack, then the original function call will no longer have ownership. 

