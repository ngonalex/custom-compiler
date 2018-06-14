
// def foo()
//  bob = tuple(3)
//  bob -> 3 = tuple(20)
//  return bob
// call foo()
TEST_F(CodeGenTest, CanCreateTupleWithOneTupleAssignmentFunction) {
  Statement::Block foo_statements;
  Statement::Block statements;

  foo_statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("bob"),
      make_unique<IntegerExpr>(3))));
  foo_statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(3)),
      make_unique<IntegerExpr>(20))));
  auto foo_retval = make_unique<VariableExpr>("bob");
  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();
  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();
  statements.push_back(std::move(make_unique<const FunctionCall>(
      make_unique<const VariableExpr>("foo_retval"), "foo",
      std::move(arguments))));

  auto ae = make_unique<const VariableExpr>("foo_retval");
  auto ast = make_unique<const Program>(std::move(function_defs),
                                        std::move(statements), std::move(ae));

  ast->Visit(&lowerer_);
  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_ONLY_RESULT);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The program returned a tuple with indices:\n"
            "1: Unassigned\n"
            "2: Unassigned\n"
            "3: Tuple with size: 20\n");
}

// Function def skipped as it is not used
// bob = tuple(3)
// bob->2 = -500
// bob->3 = tuple(20)
TEST_F(CodeGenTest, CanCreateTupleWithMixedTypes) {
  Statement::Block foo_statements;

  auto foo_retval = make_unique<const AddExpr>(
      make_unique<const IntegerExpr>(5),
      make_unique<const IntegerExpr>(10));

  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();

  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();

  Statement::Block statements;
  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("bob"),
      make_unique<IntegerExpr>(3))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(2)),
      make_unique<IntegerExpr>(-500))));

  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(3)),
      make_unique<IntegerExpr>(20))));

  auto ae = make_unique<const VariableExpr>("bob");
  auto ast = make_unique<const Program>(std::move(function_defs),
                                        std::move(statements), std::move(ae));

  ast->Visit(&lowerer_);
  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_ONLY_RESULT);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The program returned a tuple with indices:\n"
            "1: Unassigned\n"
            "2: Integer with value: -500\n"
            "3: Tuple with size: 20\n");
}

// Function def skipped as it is not used
// bob = tuple(0)
TEST_F(CodeGenTest, TupleCreationErrorsOutWhenArgumentSuppliedisZero) {
  Statement::Block foo_statements;

  auto foo_retval = make_unique<const AddExpr>(
      make_unique<const IntegerExpr>(5),
      make_unique<const IntegerExpr>(10));

  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();

  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();

  Statement::Block statements;
  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("bob"),
      make_unique<IntegerExpr>(0))));

  auto ae = make_unique<const VariableExpr>("bob");
  auto ast = make_unique<const Program>(std::move(function_defs),
                                        std::move(statements), std::move(ae));

  ast->Visit(&lowerer_);
  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_ONLY_RESULT);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Invalid size, tuple size must be greater than zero\n");
}

// Function def skipped as it is not used
// bob = tuple(-10)
TEST_F(CodeGenTest, TupleCreationErrorsOutWhenArgumentSuppliedisNeg) {
  Statement::Block foo_statements;

  auto foo_retval = make_unique<const AddExpr>(
      make_unique<const IntegerExpr>(5),
      make_unique<const IntegerExpr>(10));

  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();

  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();

  Statement::Block statements;
  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("bob"),
      make_unique<IntegerExpr>(-10))));

  auto ae = make_unique<const VariableExpr>("bob");
  auto ast = make_unique<const Program>(std::move(function_defs),
                                        std::move(statements), std::move(ae));

  ast->Visit(&lowerer_);
  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_ONLY_RESULT);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Invalid size, tuple size must be greater than zero\n");
}

// Function def skipped as it is not used
// bob = tuple(3)
// bob->4 = tuple(20)
TEST_F(CodeGenTest, DereferenceAccessErrorsOutWhenArgumentIsAboveMaxSize) {
  Statement::Block foo_statements;

  auto foo_retval = make_unique<const AddExpr>(
      make_unique<const IntegerExpr>(5),
      make_unique<const IntegerExpr>(10));

  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();

  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();

  Statement::Block statements;
  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("bob"),
      make_unique<IntegerExpr>(3))));

  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(4)),
      make_unique<IntegerExpr>(20))));

  auto ae = make_unique<const VariableExpr>("bob");
  auto ast = make_unique<const Program>(std::move(function_defs),
                                        std::move(statements), std::move(ae));

  ast->Visit(&lowerer_);
  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_ONLY_RESULT);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Out of Bounds Error\n");
}

// Function def skipped as it is not used
// bob = tuple(3)
// bob->0 = tuple(20)
TEST_F(CodeGenTest, DereferenceAccessErrorsOutWhenArgumentIsZero) {
  Statement::Block foo_statements;

  auto foo_retval = make_unique<const AddExpr>(
      make_unique<const IntegerExpr>(5),
      make_unique<const IntegerExpr>(10));

  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();

  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();

  Statement::Block statements;
  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("bob"),
      make_unique<IntegerExpr>(3))));

  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(0)),
      make_unique<IntegerExpr>(20))));

  auto ae = make_unique<const VariableExpr>("bob");
  auto ast = make_unique<const Program>(std::move(function_defs),
                                        std::move(statements), std::move(ae));

  ast->Visit(&lowerer_);
  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_ONLY_RESULT);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Out of Bounds Error\n");
}

// Function def skipped as it is not used
// bob = tuple(3)
// bob->-1 = tuple(20)
TEST_F(CodeGenTest, DereferenceAccessErrorsOutWhenArgumentIsNegative) {
  Statement::Block foo_statements;

  auto foo_retval = make_unique<const AddExpr>(
      make_unique<const IntegerExpr>(5),
      make_unique<const IntegerExpr>(10));

  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();

  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();

  Statement::Block statements;
  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("bob"),
      make_unique<IntegerExpr>(3))));

  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(-1)),
      make_unique<IntegerExpr>(20))));

  auto ae = make_unique<const VariableExpr>("bob");
  auto ast = make_unique<const Program>(std::move(function_defs),
                                        std::move(statements), std::move(ae));

  ast->Visit(&lowerer_);
  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_ONLY_RESULT);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Out of Bounds Error\n");
}

// Function def skipped as it is not used
// bob = tuple(3)
// rob = tuple(2)
// bob = rob
TEST_F(CodeGenTest, TupleCreationErrorsOutCorrectlyWhenTheRHSIsATuple) {
  Statement::Block foo_statements;

  auto foo_retval = make_unique<const AddExpr>(
      make_unique<const IntegerExpr>(5),
      make_unique<const IntegerExpr>(10));

  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();

  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();

  Statement::Block statements;
  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("bob"),
      make_unique<IntegerExpr>(3))));
  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("rob"),
      make_unique<IntegerExpr>(2))));

  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("bob"),
      make_unique<VariableExpr>("rob"))));

  auto ae = make_unique<const VariableExpr>("bob");
  auto ast = make_unique<const Program>(std::move(function_defs),
                                        std::move(statements), std::move(ae));

  ast->Visit(&lowerer_);
  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_ONLY_RESULT);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Invalid type: must be an int\n");
}

// Function def skipped as it is not used
// bob = tuple(3)
// rob = tuple(2)
// bob = rob
TEST_F(CodeGenTest, TupleCanBeAssignedCorrectlyWhenTheRHSIsATuple) {
  Statement::Block foo_statements;

  auto foo_retval = make_unique<const AddExpr>(
      make_unique<const IntegerExpr>(5),
      make_unique<const IntegerExpr>(10));

  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();

  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();

  Statement::Block statements;
  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("bob"),
      make_unique<IntegerExpr>(3))));
  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("rob"),
      make_unique<IntegerExpr>(2))));
  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("rob"),
          make_unique<IntegerExpr>(2)),
      make_unique<IntegerExpr>(10))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("bob"),
      make_unique<VariableExpr>("rob"))));

  auto ae = make_unique<const VariableExpr>("bob");
  auto ast = make_unique<const Program>(std::move(function_defs),
                                        std::move(statements), std::move(ae));

  ast->Visit(&lowerer_);
  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_ONLY_RESULT);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The program returned a tuple with indices:\n"
            "1: Unassigned\n"
            "2: Integer with value: 10\n");
}

// Function def skipped as it is not used
// bob = tuple(3)
// rob = tuple(2)
// bob->1 = rob
TEST_F(CodeGenTest, NestedTupleCreationErrorsOutCorrectlyWhenTheRHSIsATuple) {
  Statement::Block foo_statements;

  auto foo_retval = make_unique<const AddExpr>(
      make_unique<const IntegerExpr>(5),
      make_unique<const IntegerExpr>(10));

  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();

  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();

  Statement::Block statements;
  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("bob"),
      make_unique<IntegerExpr>(3))));
  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("rob"),
      make_unique<IntegerExpr>(2))));

  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(1)),
      make_unique<VariableExpr>("rob"))));

  auto ae = make_unique<const VariableExpr>("bob");
  auto ast = make_unique<const Program>(std::move(function_defs),
                                        std::move(statements), std::move(ae));

  ast->Visit(&lowerer_);
  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_ONLY_RESULT);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Invalid type: must be an int\n");
}

// Function def skipped as it is not used
// bob = tuple(3)
// rob = tuple(2)
// bob->1 = rob
TEST_F(CodeGenTest, NestedTupleAssignedCorrectlyWhenTheRHSIsATuple) {
  Statement::Block foo_statements;

  auto foo_retval = make_unique<const AddExpr>(
      make_unique<const IntegerExpr>(5),
      make_unique<const IntegerExpr>(10));

  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();

  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();

  Statement::Block statements;
  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("bob"),
      make_unique<IntegerExpr>(3))));
  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("rob"),
      make_unique<IntegerExpr>(2))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("rob"),
          make_unique<IntegerExpr>(2)),
      make_unique<IntegerExpr>(10))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(1)),
      make_unique<VariableExpr>("rob"))));

  auto ae = make_unique<const VariableExpr>("bob");
  auto ast = make_unique<const Program>(std::move(function_defs),
                                        std::move(statements), std::move(ae));

  ast->Visit(&lowerer_);
  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_ONLY_RESULT);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The program returned a tuple with indices:\n"
            "1: Tuple with size: 2\n"
            "2: Unassigned\n"
            "3: Unassigned\n");
}

// Function def skipped as it is not used
// bob = tuple(3)
// rob = tuple(2)
// bob->1 = rob
// rob = 5
TEST_F(CodeGenTest, DanglingPointersDoNotOccurWhenReassigningATuple) {
  Statement::Block foo_statements;

  auto foo_retval = make_unique<const AddExpr>(
      make_unique<const IntegerExpr>(5),
      make_unique<const IntegerExpr>(10));

  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();

  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();

  Statement::Block statements;
  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("bob"),
      make_unique<IntegerExpr>(3))));
  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("rob"),
      make_unique<IntegerExpr>(2))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("rob"),
          make_unique<IntegerExpr>(2)),
      make_unique<IntegerExpr>(10))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(1)),
      make_unique<VariableExpr>("rob"))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("rob"),
      make_unique<IntegerExpr>(10))));

  auto ae = make_unique<const VariableExpr>("bob");
  auto ast = make_unique<const Program>(std::move(function_defs),
                                        std::move(statements), std::move(ae));

  ast->Visit(&lowerer_);
  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_ONLY_RESULT);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The program returned a tuple with indices:\n"
            "1: Tuple with size: 2\n"
            "2: Unassigned\n"
            "3: Unassigned\n");
}

// Function def skipped as it is not used
// bob = tuple(3)
// bob->1 = tuple(4)
// bob->1->1 = tuple(1)
// bob->1->2 = -100
// return bob->1
TEST_F(CodeGenTest, DoubleNestedDereferencesAreAssignedCorrectly) {
  Statement::Block foo_statements;

  auto foo_retval = make_unique<const AddExpr>(
      make_unique<const IntegerExpr>(5),
      make_unique<const IntegerExpr>(10));

  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();

  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();

  Statement::Block statements;
  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("bob"),
      make_unique<IntegerExpr>(3))));

  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(1)),
      make_unique<IntegerExpr>(4))));

  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<Dereference>(
          make_unique<Dereference>(
              make_unique<VariableExpr>("bob"),
              make_unique<IntegerExpr>(1)),
          make_unique<IntegerExpr>(1)),
      make_unique<IntegerExpr>(1))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<Dereference>(
              make_unique<VariableExpr>("bob"),
              make_unique<IntegerExpr>(1)),
          make_unique<IntegerExpr>(2)),
      make_unique<IntegerExpr>(-100))));

  auto ae = make_unique<const Dereference>(
        make_unique<VariableExpr>("bob"),
        make_unique<IntegerExpr>(1));
  auto ast = make_unique<const Program>(std::move(function_defs),
                                        std::move(statements), std::move(ae));

  ast->Visit(&lowerer_);
  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_ONLY_RESULT);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The program returned a tuple with indices:\n"
            "1: Tuple with size: 1\n"
            "2: Integer with value: -100\n"
            "3: Unassigned\n"
            "4: Unassigned\n");
}

// Function def skipped as it is not used
// bob = tuple(3)
// bob->1 = tuple(4)
// bob->1->1 = tuple(2)
// bob->1->2 = -100
// rob = bob->1->1
// return rob
TEST_F(CodeGenTest, RHSDoubleNestedDereferencesCanBeAccessedCorrectly) {
  Statement::Block foo_statements;

  auto foo_retval = make_unique<const AddExpr>(
      make_unique<const IntegerExpr>(5),
      make_unique<const IntegerExpr>(10));

  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();

  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();

  Statement::Block statements;
  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("bob"),
      make_unique<IntegerExpr>(3))));

  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(1)),
      make_unique<IntegerExpr>(4))));

  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<Dereference>(
          make_unique<Dereference>(
              make_unique<VariableExpr>("bob"),
              make_unique<IntegerExpr>(1)),
          make_unique<IntegerExpr>(1)),
      make_unique<IntegerExpr>(2))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<Dereference>(
              make_unique<VariableExpr>("bob"),
              make_unique<IntegerExpr>(1)),
          make_unique<IntegerExpr>(2)),
      make_unique<IntegerExpr>(-100))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("rob"),
      make_unique<Dereference>(
          make_unique<Dereference>(
              make_unique<VariableExpr>("bob"),
              make_unique<IntegerExpr>(1)),
          make_unique<IntegerExpr>(1)))));

  auto ae = make_unique<const VariableExpr>("rob");
  auto ast = make_unique<const Program>(std::move(function_defs),
                                        std::move(statements), std::move(ae));

  ast->Visit(&lowerer_);
  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_ONLY_RESULT);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The program returned a tuple with indices:\n"
            "1: Unassigned\n"
            "2: Unassigned\n");
}

// Function def skipped as it is not used
// bob = tuple(3)
// bob->1 = tuple(4)
// bob->2 = 4
// bob->1->1 = tuple(2)
// bob->1->2 = -100
// rob = bob->2->1
// return rob
TEST_F(CodeGenTest, NestedDereferencesErrorOutIfObjectAccessedIsNotTuple) {
  Statement::Block foo_statements;

  auto foo_retval = make_unique<const AddExpr>(
      make_unique<const IntegerExpr>(5),
      make_unique<const IntegerExpr>(10));

  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();

  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();

  Statement::Block statements;
  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("bob"),
      make_unique<IntegerExpr>(3))));

  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(1)),
      make_unique<IntegerExpr>(4))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(2)),
      make_unique<IntegerExpr>(4))));

  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<Dereference>(
          make_unique<Dereference>(
              make_unique<VariableExpr>("bob"),
              make_unique<IntegerExpr>(1)),
          make_unique<IntegerExpr>(1)),
      make_unique<IntegerExpr>(2))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<Dereference>(
              make_unique<VariableExpr>("bob"),
              make_unique<IntegerExpr>(1)),
          make_unique<IntegerExpr>(2)),
      make_unique<IntegerExpr>(-100))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("rob"),
      make_unique<Dereference>(
          make_unique<Dereference>(
              make_unique<VariableExpr>("bob"),
              make_unique<IntegerExpr>(2)),
          make_unique<IntegerExpr>(1)))));

  auto ae = make_unique<const VariableExpr>("rob");
  auto ast = make_unique<const Program>(std::move(function_defs),
                                        std::move(statements), std::move(ae));

  ast->Visit(&lowerer_);
  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_ONLY_RESULT);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Invalid type: must be a tuple\n");
}

// Function def skipped as it is not used
// bob = tuple(3)
// bob->1 = tuple(4)
// rob = bob->2->1
// return rob
TEST_F(CodeGenTest, CannotAccessAnIndexThatDoesNotExist) {
  Statement::Block foo_statements;

  auto foo_retval = make_unique<const AddExpr>(
      make_unique<const IntegerExpr>(5),
      make_unique<const IntegerExpr>(10));

  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();

  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();

  Statement::Block statements;
  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("bob"),
      make_unique<IntegerExpr>(3))));

  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(1)),
      make_unique<IntegerExpr>(4))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("rob"),
      make_unique<Dereference>(
          make_unique<Dereference>(
              make_unique<VariableExpr>("bob"),
              make_unique<IntegerExpr>(2)),
          make_unique<IntegerExpr>(1)))));

  auto ae = make_unique<const VariableExpr>("rob");
  auto ast = make_unique<const Program>(std::move(function_defs),
                                        std::move(statements), std::move(ae));

  ast->Visit(&lowerer_);
  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_ONLY_RESULT);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Value does not exist in the tuple\n");
}

// Function def skipped as it is not used
// bob = tuple(10)
// bob->1 = 1
// bob->2 = 4
// bob->3 = 5
// bob->4 = 10
// rob = (bob->4 / bob->3) + (bob->1 * bob->2)
// rob = rob - bob->4
// return rob
TEST_F(CodeGenTest, ArithmeticExprWorksOnTuplesIndicesIfTheyAreInts) {
  Statement::Block foo_statements;

  auto foo_retval = make_unique<const AddExpr>(
      make_unique<const IntegerExpr>(5),
      make_unique<const IntegerExpr>(10));

  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();

  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();

  Statement::Block statements;
  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("bob"),
      make_unique<IntegerExpr>(4))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(1)),
      make_unique<IntegerExpr>(1))));
  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(2)),
      make_unique<IntegerExpr>(4))));
  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(3)),
      make_unique<IntegerExpr>(5))));
  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(4)),
      make_unique<IntegerExpr>(10))));
  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("rob"),
      make_unique<AddExpr>(
          make_unique<DivideExpr>(
              make_unique<Dereference>(
                  make_unique<VariableExpr>("bob"),
                  make_unique<IntegerExpr>(4)),
              make_unique<Dereference>(
                  make_unique<VariableExpr>("bob"),
                  make_unique<IntegerExpr>(3))),
          make_unique<MultiplyExpr>(
              make_unique<Dereference>(
                  make_unique<VariableExpr>("bob"),
                  make_unique<IntegerExpr>(2)),
              make_unique<Dereference>(
                  make_unique<VariableExpr>("bob"),
                  make_unique<IntegerExpr>(1)))))));
  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("rob"),
      make_unique<SubtractExpr>(
          make_unique<VariableExpr>("rob"),
          make_unique<Dereference>(
              make_unique<VariableExpr>("bob"),
              make_unique<IntegerExpr>(4))))));

  auto ae = make_unique<const VariableExpr>("rob");
  auto ast = make_unique<const Program>(std::move(function_defs),
                                        std::move(statements), std::move(ae));

  ast->Visit(&lowerer_);
  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_ONLY_RESULT);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The program returned: -4\n");
}

// Function def skipped as it is not used
// bob = tuple(10)
// bob->1 = 1
// bob->2 = 4
// bob->3 = 5
// bob->4 = 10
// if(bob->1 < bob->2 || bob->3 > bob->4)
//  rob = 1
// else
//  rob = 0
// return rob
TEST_F(CodeGenTest, GreaterThanLessThanWorksOnTuplesIndicesIfTheyAreInts) {
  Statement::Block foo_statements;

  auto foo_retval = make_unique<const AddExpr>(
      make_unique<const IntegerExpr>(5),
      make_unique<const IntegerExpr>(10));

  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();

  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();

  Statement::Block statements;
  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("bob"),
      make_unique<IntegerExpr>(4))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(1)),
      make_unique<IntegerExpr>(1))));
  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(2)),
      make_unique<IntegerExpr>(4))));
  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(3)),
      make_unique<IntegerExpr>(5))));
  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(4)),
      make_unique<IntegerExpr>(10))));

  Statement::Block trueblock;
  Statement::Block falseblock;

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("rob"),
      make_unique<IntegerExpr>(1))));
  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("rob"),
      make_unique<IntegerExpr>(0))));

  statements.push_back(make_unique<Conditional>(
      make_unique<LogicalOrExpr>(
          make_unique<LessThanExpr>(
              make_unique<Dereference>(
                  make_unique<VariableExpr>("bob"),
                  make_unique<IntegerExpr>(1)),
              make_unique<Dereference>(
                  make_unique<VariableExpr>("bob"),
                  make_unique<IntegerExpr>(2))),
          make_unique<GreaterThanExpr>(
              make_unique<Dereference>(
                  make_unique<VariableExpr>("bob"),
                  make_unique<IntegerExpr>(3)),
              make_unique<Dereference>(
                  make_unique<VariableExpr>("bob"),
                  make_unique<IntegerExpr>(4)))),
      std::move(trueblock), std::move(falseblock)));

  auto ae = make_unique<const VariableExpr>("rob");
  auto ast = make_unique<const Program>(std::move(function_defs),
                                        std::move(statements), std::move(ae));

  ast->Visit(&lowerer_);
  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_ONLY_RESULT);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The program returned: 1\n");
}

// Function def skipped as it is not used
// bob = tuple(10)
// bob->1 = 1
// bob->2 = 4
// bob->3 = 5
// bob->4 = 10
// if(bob->1 <= bob->2 && bob->3 >= bob->4)
//  rob = 1
// else
//  rob = 0
// return rob
TEST_F(CodeGenTest,
       GreaterThanEqualToLessThanEqualToWorksOnTuplesIndicesIfTheyAreInts) {
  Statement::Block foo_statements;

  auto foo_retval = make_unique<const AddExpr>(
      make_unique<const IntegerExpr>(5),
      make_unique<const IntegerExpr>(10));

  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();

  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();

  Statement::Block statements;
  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("bob"),
      make_unique<IntegerExpr>(4))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(1)),
      make_unique<IntegerExpr>(1))));
  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(2)),
      make_unique<IntegerExpr>(4))));
  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(3)),
      make_unique<IntegerExpr>(5))));
  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(4)),
      make_unique<IntegerExpr>(10))));

  Statement::Block trueblock;
  Statement::Block falseblock;

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("rob"),
      make_unique<IntegerExpr>(1))));
  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("rob"),
      make_unique<IntegerExpr>(0))));

  statements.push_back(make_unique<Conditional>(
      make_unique<LogicalAndExpr>(
          make_unique<LessThanEqualToExpr>(
              make_unique<Dereference>(
                  make_unique<VariableExpr>("bob"),
                  make_unique<IntegerExpr>(1)),
              make_unique<Dereference>(
                  make_unique<VariableExpr>("bob"),
                  make_unique<IntegerExpr>(2))),
          make_unique<GreaterThanEqualToExpr>(
              make_unique<Dereference>(
                  make_unique<VariableExpr>("bob"),
                  make_unique<IntegerExpr>(3)),
              make_unique<Dereference>(
                  make_unique<VariableExpr>("bob"),
                  make_unique<IntegerExpr>(4)))),
      std::move(trueblock), std::move(falseblock)));

  auto ae = make_unique<const VariableExpr>("rob");
  auto ast = make_unique<const Program>(std::move(function_defs),
                                        std::move(statements), std::move(ae));

  ast->Visit(&lowerer_);
  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_ONLY_RESULT);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The program returned: 0\n");
}

// Function def skipped as it is not used
// bob = tuple(10)
// bob->1 = 1
// bob->2 = 4
// bob->3 = tuple(5)
// bob->4 = 10
// if(bob->1 <= bob->2 && bob->3 >= bob->4)
//  rob = 1
// else
//  rob = 0
// return rob
TEST_F(CodeGenTest, CannotUseATupleAsAnArgumentForRelationals) {
  Statement::Block foo_statements;

  auto foo_retval = make_unique<const AddExpr>(
      make_unique<const IntegerExpr>(5),
      make_unique<const IntegerExpr>(10));

  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();

  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();

  Statement::Block statements;
  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("bob"),
      make_unique<IntegerExpr>(4))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(1)),
      make_unique<IntegerExpr>(1))));
  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(2)),
      make_unique<IntegerExpr>(4))));
  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(3)),
      make_unique<IntegerExpr>(5))));
  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(4)),
      make_unique<IntegerExpr>(10))));

  Statement::Block trueblock;
  Statement::Block falseblock;

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("rob"),
      make_unique<IntegerExpr>(1))));
  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("rob"),
      make_unique<IntegerExpr>(0))));

  statements.push_back(make_unique<Conditional>(
      make_unique<LogicalAndExpr>(
          make_unique<LessThanEqualToExpr>(
              make_unique<Dereference>(
                  make_unique<VariableExpr>("bob"),
                  make_unique<IntegerExpr>(1)),
              make_unique<Dereference>(
                  make_unique<VariableExpr>("bob"),
                  make_unique<IntegerExpr>(2))),
          make_unique<GreaterThanEqualToExpr>(
              make_unique<Dereference>(
                  make_unique<VariableExpr>("bob"),
                  make_unique<IntegerExpr>(3)),
              make_unique<Dereference>(
                  make_unique<VariableExpr>("bob"),
                  make_unique<IntegerExpr>(4)))),
      std::move(trueblock), std::move(falseblock)));

  auto ae = make_unique<const VariableExpr>("rob");
  auto ast = make_unique<const Program>(std::move(function_defs),
                                        std::move(statements), std::move(ae));

  ast->Visit(&lowerer_);
  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_ONLY_RESULT);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Invalid type: must be an int\n");
}

// Function def skipped as it is not used
// bob = tuple(10)
// bob->1 = 1
// bob->2 = tuple(2)
// bob->3 = 5
// bob->4 = 10
// rob = (bob->4 / bob->3) + (bob->1 * bob->2)
// rob = rob - bob->4
// return rob
TEST_F(CodeGenTest, ArithExprFailsIfArgumentIsTuple) {
  Statement::Block foo_statements;

  auto foo_retval = make_unique<const AddExpr>(
      make_unique<const IntegerExpr>(5),
      make_unique<const IntegerExpr>(10));

  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();

  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();

  Statement::Block statements;
  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<VariableExpr>("bob"),
      make_unique<IntegerExpr>(4))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(1)),
      make_unique<IntegerExpr>(1))));
  statements.push_back(std::move(make_unique<AssignmentFromNewTuple>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(2)),
      make_unique<IntegerExpr>(4))));
  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(3)),
      make_unique<IntegerExpr>(5))));
  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<Dereference>(
          make_unique<VariableExpr>("bob"),
          make_unique<IntegerExpr>(4)),
      make_unique<IntegerExpr>(10))));
  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("rob"),
      make_unique<AddExpr>(
          make_unique<DivideExpr>(
              make_unique<Dereference>(
                  make_unique<VariableExpr>("bob"),
                  make_unique<IntegerExpr>(4)),
              make_unique<Dereference>(
                  make_unique<VariableExpr>("bob"),
                  make_unique<IntegerExpr>(3))),
          make_unique<MultiplyExpr>(
              make_unique<Dereference>(
                  make_unique<VariableExpr>("bob"),
                  make_unique<IntegerExpr>(2)),
              make_unique<Dereference>(
                  make_unique<VariableExpr>("bob"),
                  make_unique<IntegerExpr>(1)))))));
  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("rob"),
      make_unique<SubtractExpr>(
          make_unique<VariableExpr>("rob"),
          make_unique<Dereference>(
              make_unique<VariableExpr>("bob"),
              make_unique<IntegerExpr>(4))))));

  auto ae = make_unique<const VariableExpr>("rob");
  auto ast = make_unique<const Program>(std::move(function_defs),
                                        std::move(statements), std::move(ae));

  ast->Visit(&lowerer_);
  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_ONLY_RESULT);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Invalid type: must be an int\n");
}