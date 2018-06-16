
Totally not a Swift Compiler
=======================================

## Building

Compile subprojects by giving their paths and task names defined in the respective BUILD files to `bazel build`.

The main project, in the public directory, for example, is built by `bazel build //public:main`. If the directory and task in BUILD file have the same name, you can omit the task name.

## Testing

Test in the same way, by using `bazel test`. Make sure to include the library/binary that you are testing as a dependency in the test task.

## Running

To run a certain subproject, you can use `bazel run` with the path and name of the task.
=======================================

Frontend Documentation

We've proud to have completed and tested up to v5. We've also added additional error handling (character number and error messages that tell you where you've failed and how to fix it sometimes) and got cacheing to work for basic_combinators and v1_combinators.

Missing: Getting cache to work for v2 to v5. Cache works for basic combinators and v1 because they don't return ASTs. One solution would be to duplicate the AST nodes as they're being created, but we felt this would be too hacky of a solution and inefficient.

Here is a link to our language specification
https://github.ucsb.edu/CS160-S18/team-influx/issues/21


Frontend: (TACK ON team-influx/frontend/combinators BEFORE EACH folder /)

High level Overview:
Everything in a helper folder does not return an ast node and we've modularized our files such that every file only return one ast.

basic_combinators/null.cc is our base class for all our parsers/combinators. This handles fail error handling with error messages and is where our packrat parsing is called.

basic_combinators/parseStatus.h is the class that is returned from all our parsers/combinators with things like status (success/fail), parsed characters, remaining characters, optional asts, etc. 

Everything else in basic combinator classes serves as building blocks for our later versions (and or sequence parser, or parser, atom parser, null parser, one or more parser, zero or more parser). 

In /v1_tests, we implemented arithmetic expressions with ae.cc as our highest order function. ae.cc calls mul_div_expr, term_expr, and add_sub_expr in a recursive manner that allows for all sorts of complicated arithmetic expressions (i.e. look at the last 2-3 tests in v1_test.cc)

In /v2_tests, we mainly implemented variable assignments. program_parser.cc is our highest order function which allows for multiple assignments and then an arithmetic expression from v1.

In /v3_tests, we mainly implemented loops, boolean operations, blocks, and if statements. In here our higher order functions include conditional_parser.cc, loop_parser.cc, block_parser.cc, and relation_parser.cc. 

In /v4_tests, we implemented function calls

In /v5_tests, we implemented 




**Combinator Library**



**Basic Combinators**

and_parser (sequence parser) = and_combinator.cc (sequence parser)
atom_parser (parses individual characters) = atom_parser.cc
null (super class that handles packrat parsing and fail cases) = null.cc
one or more parser = one_or_more_combinator.cc
zero or more parser = zero_or_more_combinator.cc
or combinator = or_combinator.cc
parse status (results to all our combinators/parser) = parsestatus.h 



**Cacheing**

Refer to "missing" section.

**Non-trivial (recursive) handling of arithmetic expression and relations**


With more time, we'd like to clean up our code such that we can handle longer sequences instead of our hacky solution of having multiple "and combinators", build our parsestatus out such that every combinator/parser would get its own parse status subclass whenever there is a need for it instead of putting everything into one ParseStatus class, and we would also like to do a more efficient version of Packrat parsing that isn't memory intensive (our workaround with unique pointers). Additionally, we'd would've loved to include some non trivial error handling, and added templating in our ParseStatus. 


Technical problems: Getting acquainted to Bazel, learning functional programming + combinators and figuring out how to split up the work such that we maximize efficiency. Also figuring out a solution for cacheing with Unique pointers


Responsibilities:
Victor helped with basic combinators, v1, finished v2, v3, and v4.
Prabal rewrote v1 and v3 to get them working using a recursive manner, and implemented v4 and v5.

**Backend** 
See the readme in the backend folder
