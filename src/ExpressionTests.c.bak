#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "stack.h"
#include "environment.h"
#include "expression.h"
#include "ExpressionTests.h"
#include "testType.h"
#include "execution.h"

void beforeEach(Expression *exp, Instruction *program, int p_size)
{
  exp->env = init_env();
  exp->stack = exp->env->stack;
  exp->program = program;
  exp->p_size = p_size;
}

void assert_int_equals(int actual, int expected, Test *test)
{
  if (actual != expected)
  {
    test->failed = 1;
    char msg[341];
    sprintf(msg, "Expected %d, got %d", expected, actual);
    test->message = msg;
  }
  else
  {
    test->failed = 0;
  }
}

void assert_double_equals(double actual, double expected, Test *test)
{
  if (actual != expected)
  {
    test->failed = 1;
    char msg[341];
    sprintf(msg, "Expected %f, got %f", expected, actual);
    test->message = msg;
  }
  else
  {
    test->failed = 0;
  }
}

void assert_true(int actual, Test * test) {
  if (actual) {
    test->failed = 0;
  } else {
    test->failed = 1;
    char msg[341];
    sprintf(msg, "Expected True, got %d", actual);
    test->message = msg;
  }
}

void assert_false(int actual, Test *test)
{
  if (!actual)
  {
    test->failed = 0;
  } else
  {
    test->failed = 1;
    char msg[341];
    sprintf(msg, "Expected False, got %d", actual);
    test->message = msg;
  }
}

TestResult runExpressionTests(void)
{
  printf("Testing the expression module\n");

  TestResult res;
  
  const int numTests = 41;
  res.total = numTests;
  res.passed = 0;
  res.tests = calloc(numTests, sizeof(Test));

  res.tests[0] = testPushToStack();
  res.tests[1] = testPushVarToStack();
  res.tests[2] = testAnd1();
  res.tests[3] = testAnd2();
  res.tests[4] = testAnd3();
  res.tests[5] = testOr1();
  res.tests[6] = testOr2();
  res.tests[7] = testNot1();
  res.tests[8] = testNot2();
  res.tests[9] = testNot3();
  res.tests[10] = testNot4();
  res.tests[11] = testLT1();
  res.tests[12] = testLT2();
  res.tests[13] = testGT1();
  res.tests[14] = testGT2();
  res.tests[15] = testEqual();
  res.tests[16] = testAdd();
  res.tests[17] = testSub();
  res.tests[18] = testMul();
  res.tests[19] = testDiv();
  res.tests[20] = testMod();
  res.tests[21] = testLog();
  res.tests[22] = testPow();
  res.tests[23] = testSqrt();
  res.tests[24] = expValue();
  res.tests[25] = ceilValue();
  res.tests[26] = ceilValueBig();
  res.tests[27] = floorValue();
  res.tests[28] = floorValueBig();
  res.tests[29] = roundUnderHalf();
  res.tests[30] = roundOverHalf();
  res.tests[31] = absoluteValue1();
  res.tests[32] = absoluteValue2();
  res.tests[33] = lessThanEqual1();
  res.tests[34] = lessThanEqual2();
  res.tests[35] = greaterThanEqual1();
  res.tests[36] = greaterThanEqual2();

  //tests from the original testsuite
  res.tests[37] = og_test_execute_query_with_result();
  res.tests[38] = og_test_execute_query_without_result();
  res.tests[39] = og_test_execute_quries_single_result();
  res.tests[40] = og_test_execute_quries_multiple_results();


  return res;
}

// Test Adding a const to the stack
Test testPushToStack(void){
  // Arrange
  Test res;
  res.name = "Push Const to Stack";

  Instruction p1[2] = {{{CONST}, 0},{.data._int = 3, 2}};
  Expression exp1;
  beforeEach(&exp1, p1, 2);
  
  // Act
  int actual = call(&exp1).type._int;
  int expected = 3;

  // Assert
  assert_int_equals(actual, expected, &res);
  return res;
}

// Test pushing var from env to stack
Test testPushVarToStack(void){
  // Arrange
  Test res;
  res.name = "Push Var to Stack";
  Instruction p[2] = {{{VAR}, 0},{.data._int = 0, 2}};

  Expression exp;
  beforeEach(&exp, p, 2);
  Number val = {{10}, 2};
  set_value(exp.env, 0, val);

  // Act
  Number popped = call(&exp);
  int actual = popped.type._int;
  int expected = 10;
  
  // Assert
  assert_int_equals(actual, expected, &res);
  return res;
}

Test testAnd1(void){
  // Arrange
  Test test;
  test.name = "1 AND 1 returns true";

  Instruction p[5] = {{{CONST}, 0},{.data._int = 1, 2},{{CONST}, 0},{.data._int = 1, 2},{{AND}, 0}};
  Expression exp;
  beforeEach(&exp, p, 5);
  
  // Act
  int actual = call(&exp).type._int;

  // Assert
  assert_true(actual, &test);

  return test;
}

Test testAnd2(void){
  // Arrange
  Test test;
  test.name = "1 AND 0 returns false";

  Instruction p[5] = {{{CONST}, 0},{.data._int = 1, 2},{{CONST}, 0},{.data._int = 0, 2},{{AND}, 0}};
  Expression exp;

  beforeEach(&exp, p, 5);
  
  // Act
  int actual = call(&exp).type._int;

  // Assert
  assert_false(actual, &test);

  return test;
}

Test testAnd3(void){
  // Arrange
  Test test;
  test.name = "0 AND 0 returns false";
  Instruction p[5] = {{{CONST}, 0},{.data._int = 0, 2},{{CONST}, 0},{.data._int = 0, 2},{{AND}, 0}};

  Expression exp;
  beforeEach(&exp, p, 5);

  // Act
  int actual = call(&exp).type._int;

  // Assert
  assert_false(actual, &test);

  return test;
}

Test testOr1(void){
  // Arrange
  Test test;
  test.name = "1 or 0 returns true";
  Instruction p[5] = {{{CONST}, 0},{.data._int = 1, 2},{{CONST}, 0},{.data._int = 0, 2},{{OR}, 0}};

  Expression exp;
  beforeEach(&exp, p, 5);

  // Act
  int actual = call(&exp).type._int;

  // Assert
  assert_true(actual, &test);

  return test;
}

Test testOr2(void){
  // Arrange
  Test test;
  test.name = "0 or 0 returns false";
  Instruction p[5] = {{{CONST}, 0},{.data._int = 0, 2},{{CONST}, 0},{.data._int = 0, 2},{{OR}, 0}};

  Expression exp;
  beforeEach(&exp, p, 5);

  // Act
  int actual = call(&exp).type._int;

  // Assert
  assert_false(actual, &test);

  return test;
}

Test testNot1(void){
  // Arrange
  Test test;
  test.name = "Not 0 returns true";
  Instruction p[3] = {{{CONST}, 0},{.data._int = 0, 2},{{NOT}, 0}};

  Expression exp;
  beforeEach(&exp, p, 3);

  // Act
  int actual = call(&exp).type._int;

  // Assert
  assert_true(actual, &test);

  return test;
}

Test testNot2(void){
  // Arrange
  Test test;
  test.name = "Not 0.0 returns true";
  Instruction p[3] = {{{CONST}, 0},{.data._double = 0.0, 4},{{NOT}, 0}};

  Expression exp;
  beforeEach(&exp, p, 3);

  // Act
  int actual = call(&exp).type._int;

  // Assert
  assert_true(actual, &test);

  return test;
}

Test testNot3(void){
  // Arrange
  Test test;
  test.name = "Not uint 5 returns false";
  Instruction p[3] = {{{CONST}, 0},{.data._uint32 = 5, 1},{{NOT}, 0}};

  Expression exp;
  beforeEach(&exp, p, 3);

  // Act
  int actual = call(&exp).type._int;

  // Assert
  assert_false(actual, &test);

  return test;
}

Test testNot4(void){
  // Arrange
  Test test;
  test.name = "Not int 1 returns false";
  Instruction p[3] = {{{CONST}, 0},{.data._int = 1, 2},{{NOT}, 0}};

  Expression exp;
  beforeEach(&exp, p, 3);

  // Act
  int actual = call(&exp).type._int;

  // Assert
  assert_false(actual, &test);

  return test;
}

Test testLT1(void){
  // Arrange
  Test test;
  test.name = "1.0 < 5 returns true";
  Instruction p[5] = {{{CONST}, 0},{.data._double = 1.0, 4},{{CONST}, 0},{.data._int = 5, 2},{{LT}, 0}};

  Expression exp;
  beforeEach(&exp, p, 5);

  // Act
  int actual = call(&exp).type._int;

  // Assert
  assert_true(actual, &test);

  return test;
}

Test testLT2(void){
  // Arrange
  Test test;
  test.name = "uint 7 < 3.4f returns false";
  Instruction p[5] = {{{CONST}, 0},{.data._uint32 = 7, 1},{{CONST}, 0},{.data._float = 3.4f, 3},{{LT}, 0}};

  Expression exp;
  beforeEach(&exp, p, 5);

  // Act
  int actual = call(&exp).type._int;

  // Assert
  assert_false(actual, &test);

  return test;
}

Test testGT1(void){
  // Arrange
  Test test;
  test.name = "-3 > uint 5 returns false";

  Instruction p[5] = {{{CONST}, 0},{.data._int = -3, 2},{{CONST}, 0},{.data._uint32 = 5, 1},{{GT}, 0}};

  Expression exp;
  beforeEach(&exp, p, 5);

  // Act
  int actual = call(&exp).type._int;

  // Assert
  assert_false(actual, &test);

  return test;
}

Test testGT2(void){
  // Arrange
  Test test;
  test.name = "3.14 > 0 returns true";
  Instruction p[5] = {{{CONST}, 0},{.data._double = 3.14, 4},{{CONST}, 0},{.data._int = 0, 2},{{GT}, 0}};
  Expression exp;
  beforeEach(&exp, p, 5);

  // Act
  int actual = call(&exp).type._int;

  // Assert
  assert_true(actual, &test);

  return test;
}

Test testEqual(void){
  Test res;
  res.name = "Equal 10 == 10";

  Instruction p1[5] = {{{CONST}, 0},{.data._int = 10, 2},{{CONST}, 0},{.data._int = 10, 2},{{EQ},0}};
    
  Expression exp1;
  beforeEach(&exp1, p1, 5);

  // Action
  Number actual = call(&exp1);
  
  // Assert
  assert_true(actual.type._int, &res);
  return res;
}

Test testAdd(void){
  Test res;
  res.name = "Add 10 + 3 = 13";

  Instruction p1[5] = {{{CONST}, 0},{.data._int = 10, 2},{{CONST}, 0},{.data._int = 3, 2},{{ADD},0}};

  Expression exp1;
  beforeEach(&exp1, p1, 5);
  
  // Action
  Number actual = call(&exp1);
  int expected = 13;
  
  // Assert
  assert_int_equals(actual.type._int, expected, &res);
  return res;
}

Test testSub(void){
  Test res;
  res.name = "Sub 10 - 3 = 7";

  Instruction p1[5] = {{{CONST}, 0},{.data._int = 10, 2},{{CONST}, 0},{.data._int = 3, 2},{{SUB},0}};

  Expression exp1;
  beforeEach(&exp1, p1, 5);
  
  // Action
  Number actual = call(&exp1);
  int expected = 7;

  // Assert
  assert_int_equals(actual.type._int, expected, &res);
  return res;

}

Test testMul(void){
  Test res;
  res.name = "Mul 10 * 3 = 30";
  
  Instruction p1[5] = {{{CONST}, 0},{.data._int = 10, 2},{{CONST}, 0},{.data._int = 3, 2},{{MUL},0}};

  Expression exp1;
  beforeEach(&exp1, p1, 5);
  
  // Action
  Number actual = call(&exp1);
  int expected = 30;

  // Assert
  assert_int_equals(actual.type._int, expected, &res);
  return res;
}

Test testDiv(void){
  Test res;
  res.name = "Div 10 / 3 = 3";
  
  Instruction p1[5] = {{{CONST}, 0},{.data._int = 10, 2},{{CONST}, 0},{.data._int = 3, 2},{{DIV},0}};

  Expression exp1;
  beforeEach(&exp1, p1, 5);
  
  // Action
  Number actual = call(&exp1);
  int expected = 3;
  
  // Assert
  assert_int_equals(actual.type._int, expected, &res);
  return res;
}

Test testMod(void){
  Test res;
  res.name = "Mod 10 % 3 = 1";
  
  Instruction p1[5] = {{{CONST}, 0},{.data._int = 10, 2},{{CONST}, 0},{.data._int = 3, 2},{{MOD},0}};

  Expression exp1;
  beforeEach(&exp1, p1, 5);
  
  // Action
  Number actual = call(&exp1);
  int expected = 1;
  
  // Assert
  assert_int_equals(actual.type._int, expected, &res);
  return res;
}

Test testLog(void){
  Test res;
  res.name = "Log 10";
  
  Instruction p1[3] = {{{CONST}, 0},{.data._double = 10, 4},{{LOG},0}};

  Expression exp1;
  beforeEach(&exp1, p1, 3);
  
  // Action
  Number actual = call(&exp1);
  double expected = log(10);
  
  // Assert
  assert_double_equals(actual.type._double, expected, &res);
  return res;
}

Test testPow(void){
  Test res;
  res.name = "2 to the power of 3";

  Instruction p1[5] = {{{CONST}, 0},{.data._int = 2, 2},{{CONST}, 0},{.data._int = 3, 2},{{POW},0}};

  Expression exp1;
  beforeEach(&exp1, p1, 5);
  
  // Action
  Number actual = call(&exp1);
  double expected = pow(2,3);
  
  // Assert
  assert_double_equals(actual.type._double, expected, &res);
  return res;
}


Test testSqrt(void){
  Test res;
  res.name = "Sqrt 4 to 2";

  Instruction p1[3] = {{{CONST}, 0},{.data._int = 4, 2},{{SQRT},0}};

  Expression exp1;
  beforeEach(&exp1, p1, 3);
  
  // Action
  Number actual = call(&exp1);
  double expected = sqrt(4);
  
  // Assert
  assert_double_equals(actual.type._double, expected, &res);
  return res;
}


Test expValue(void){
  Test res;
  res.name = "1 to the power of euler's number";

  Instruction p1[3] = {{{CONST}, 0},{.data._int = 1, 2},{{EXP},0}};

  Expression exp1;
  beforeEach(&exp1, p1, 3);
  
  // Action
  Number actual = call(&exp1);
  double expected = exp(1);
  
  // Assert
  assert_double_equals(actual.type._double, expected, &res);
  return res;
}

Test ceilValue(void){
  Test res;
  res.name = "Ceil 1.4 to 2";

  Instruction p1[3] = {{{CONST}, 0},{.data._double = 1.4, 4},{{CEIL},0}};

  Expression exp1;
  beforeEach(&exp1, p1, 3);
  
  // Action
  Number actual = call(&exp1);
  int expected = 2;
  
  // Assert
  assert_int_equals(actual.type._int, expected, &res);
  return res;
}

Test ceilValueBig(void){
  Test res;
  res.name = "Ceil 1.9 to 2";

  Instruction p1[3] = {{{CONST}, 0},{.data._double = 1.9, 4},{{CEIL},0}};

  Expression exp1;
  beforeEach(&exp1, p1, 3);
  
  // Action
  Number actual = call(&exp1);
  int expected = 2;
  
  // Assert
  assert_int_equals(actual.type._int, expected, &res);
  return res;
}


Test floorValue(void){
  Test res;
  res.name = "Floor 1.4 to 1";

  Instruction p1[3] = {{{CONST}, 0},{.data._double = 1.4, 4},{{FLOOR},0}};

  Expression exp1;
  beforeEach(&exp1, p1, 3);
  
  // Action
  Number actual = call(&exp1);
  int expected = 1;
  
  // Assert
  assert_int_equals(actual.type._int, expected, &res);
  return res;
}

Test floorValueBig(void){
  Test res;
  res.name = "Floor 1.9 to 1";

  Instruction p1[3] = {{{CONST}, 0},{.data._double = 1.9, 4},{{FLOOR},0}};

  Expression exp1;
  beforeEach(&exp1, p1, 3);
  
  // Action
  Number actual = call(&exp1);
  int expected = 1;
  
  // Assert
  assert_int_equals(actual.type._int, expected, &res);
  return res;
}

Test roundUnderHalf(void){
  Test res;
  res.name = "Round 1.4 to 1";

  Instruction p1[3] = {{{CONST}, 0},{.data._double = 1.4, 4},{{ROUND},0}};

  Expression exp1;
  beforeEach(&exp1, p1, 3);
  
  // Action
  Number actual = call(&exp1);
  double expected = 1;
  
  // Assert
  assert_int_equals(actual.type._int, expected, &res);
  return res;
}

Test roundOverHalf(void){
  Test res;
  res.name = "Round 1.6 to 2";

  Instruction p1[3] = {{{CONST}, 0},{.data._double = 1.6, 4},{{ROUND},0}};

  Expression exp1;
  beforeEach(&exp1, p1, 3);
  
  // Action
  Number actual = call(&exp1);
  double expected = 2;
  
  // Assert
  assert_int_equals(actual.type._int, expected, &res);
  return res;
}


Test absoluteValue1(void){
  Test res;
  res.name = "Absolute value of 5";
  
  Instruction p1[3] = {{{CONST}, 0},{.data._int = 5, 2},{{ABS},0}};
  
  Expression exp1;
  beforeEach(&exp1, p1, 3);
  
  // Action
  Number actual = call(&exp1);
  double expected = 5;
  
  // Assert
  if(actual.type._double != expected) {
    res.failed = 1;
    char msg[341];
    sprintf(msg, "Expected %lf, got %lf",expected, actual.type._double);
    res.message = msg;
  } else {
    res.failed = 0;
  }

  return res;
}

Test absoluteValue2(void){
  Test res;
  res.name = "Absolute value of -5";

  Instruction p1[3] = {{{CONST}, 0},{.data._int = -5, 2},{{ABS},0}};
  
  Expression exp1;
  beforeEach(&exp1, p1, 3);
  
  // Action
  Number actual = call(&exp1);
  double expected = 5;
  
  // Assert
  if(actual.type._double != expected) {
    res.failed = 1;
    char msg[341];
    sprintf(msg, "Expected %lf, got %lf",expected, actual.type._double);
    res.message = msg;
  } else {
    res.failed = 0;
  }
  
  return res;
}

Test lessThanEqual1(void){
  Test res;
  res.name = "Less than or equal (less than)";
  
  Instruction p1[5] = {{{CONST}, 0},{.data._int = 2, 2},{{CONST},0},{.data._int = 3, 2},{{LTEQ},0}};

  Expression exp1;
  beforeEach(&exp1, p1, 5);
  
  // Act
  Number popped = call(&exp1);
  int expected = 1;

  // Assert
  assert_int_equals(popped.type._int, expected, &res);
  return res;
}

Test lessThanEqual2(void){
  Test res;
  res.name = "Less than or equal (equal)";
  Instruction p1[5] = {{{CONST}, 0},{.data._int = 3, 2},{{CONST},0},{.data._int = 3, 2},{{LTEQ},0}};
  Expression exp1;
  beforeEach(&exp1, p1, 5);
  
  // Action
  Number popped = call(&exp1);
  int expected = 1;

  // Assert
  assert_int_equals(popped.type._int, expected, &res);
  return res;
}


Test greaterThanEqual1(void){
  Test res;
  res.name = "Greater than or equal (greater than)";
  Instruction p1[5] = {{{CONST}, 0},{.data._int = 4, 2},{{CONST},0},{.data._int = 3, 2},{{GTEQ},0}};
  Expression exp1;
  beforeEach(&exp1, p1, 5);
  
  // Action
  Number popped = call(&exp1);
  int expected = 1;

  // Assert
  assert_int_equals(popped.type._int, expected, &res);
  return res;
}

Test greaterThanEqual2(void){
  Test res;
  res.name = "Greater than or equal (equal)";
  Instruction p1[5] = {{{CONST}, 0},{.data._int = 3, 2},{{CONST},0},{.data._int = 3, 2},{{GTEQ},0}};
  Expression exp1;
  beforeEach(&exp1, p1, 5);
  
  // Action
  Number popped = call(&exp1);
  int expected = 1;

  // Assert
  assert_int_equals(popped.type._int, expected, &res);
  return res;
}

Test og_test_execute_query_with_result(void){
  Test res;
  res.name = "Original execute query with result";
  res.failed = 0;

  Instruction p[5] = {{{CONST}, 0}, {.data._int = 2, 2}, {{VAR}, 0}, {.data._int = 0, 2}, {{MUL}, 0}};

  Number envVal;
  envVal.type._int = 4;
  envVal.unionCase = 2;

  Expression exp1;
  beforeEach(&exp1, p, 5);
  set_value(exp1.env, 0, envVal);
  
  Map map;
  map.attribute = 1;
  map.expression = &exp1;

  Operation op;
  op.operation.map = &map;
  op.unionCase = 0;

  Query query;
  query.operations = &op;
  query.amount = 1;

  // Action
  QueryResponse queryRes;
  executeQuery(query, &queryRes, exp1.env);

  // Assert
  if(exp1.env->stack->stack[0].type._int != 8){
    res.failed = 1;
    char msg[341];
    sprintf(msg, "Expected %d, got %d", 8, exp1.env->stack->stack[0].type._int);
    res.message = msg;
    return res;
  }

  if(exp1.env->stack->stack[1].type._int != 4){
    res.failed = 1;
    char msg[341];
    sprintf(msg, "Expected %d, got %d", 4, exp1.env->stack->stack[1].type._int);
    res.message = msg;
    return res;
  }

  return res;
}

Test og_test_execute_query_without_result(void){
  Test res;
  res.name = "Original execute query without result";
  res.failed = 0;

  Instruction mapPro[5] = {{{CONST}, 0}, {.data._int = 2, 2}, {{VAR}, 0}, {.data._int = 0, 2}, {{MUL}, 0}};
  Instruction filterPro[5] = {{{VAR}, 0}, {.data._int = 1, 2}, {{CONST}, 0}, {.data._int = 2, 2}, {{LT}, 0}};

  Number envVal;
  envVal.type._int = 4;
  envVal.unionCase = 2;

  Env *env = init_env();
  set_value(env, 0, envVal);

  Expression mapExp;
  beforeEach(&mapExp, mapPro, 5);
  mapExp.env = env;
  
  Expression filterExp;
  beforeEach(&filterExp, filterPro, 5);
  filterExp.env = env;

  Map map;
  map.attribute = 1;
  map.expression = &mapExp;

  Filter filter;
  filter.predicate = &filterExp;

  Operation mapOp;
  mapOp.operation.map = &map;
  mapOp.unionCase = 0;

  Operation filterOp;
  filterOp.operation.filter = &filter;
  filterOp.unionCase = 1;

  Operation ops[2] = {mapOp, filterOp};

  Query query;
  query.operations = ops;
  query.amount = 2;

  // Action
  QueryResponse output;
  executeQuery(query, &output, env);

  // Assert
  if(env->stack->stack[0].type._int != 0){
    res.failed = 1;
    char msg[341];
    sprintf(msg, "Expected %d, got %d", 0, env->stack->stack[0].type._int);
    res.message = msg;
    return res;
  }

  return res;
}

Test og_test_execute_quries_single_result(void){
  Test res;
  res.name = "Original execute queries single result";
  res.failed = 0;

  Instruction p[5] = {{{CONST}, 0}, {.data._int = 2, 2}, {{VAR}, 0}, {.data._int = 0, 2}, {{MUL}, 0}};

  Number envVal;
  envVal.type._int = 4;
  envVal.unionCase = 2;

  Expression exp1;
  beforeEach(&exp1, p, 5);
  set_value(exp1.env, 0, envVal);
  
  Map map;
  map.attribute = 1;
  map.expression = &exp1;

  Operation op;
  op.operation.map = &map;
  op.unionCase = 0;

  Query query;
  query.operations = &op;
  query.amount = 1;

  Message msg;
  msg.amount = 1;
  msg.queries = &query;

  // Action
  OutputMessage output;
  executeQueries(msg, &output, exp1.env);

  // Assert
  if(exp1.env->stack->stack[0].type._int != 8){
    res.failed = 1;
    char msg[341];
    sprintf(msg, "Expected %d, got %d", 8, exp1.env->stack->stack[0].type._int);
    res.message = msg;
    return res;
  }

  if(exp1.env->stack->stack[1].type._int != 4){
    res.failed = 1;
    char msg[341];
    sprintf(msg, "Expected %d, got %d", 4, exp1.env->stack->stack[1].type._int);
    res.message = msg;
    return res;
  }

  return res;
}

Test og_test_execute_quries_multiple_results(void){
  Test res;
  res.name = "Original execute queries with multiple results";
  res.failed = 0;
  
  Instruction mapPro[5] = {{{CONST}, 0}, {.data._int = 2, 2}, {{VAR}, 0}, {.data._int = 0, 2}, {{MUL}, 0}};
  Instruction filterPro[5] = {{{VAR}, 0}, {.data._int = 1, 2}, {{CONST}, 0}, {.data._int = 2, 2}, {{LT}, 0}};

  Number envVal;
  envVal.type._int = 4;
  envVal.unionCase = 2;

  Env *env = init_env();
  set_value(env, 0, envVal);

  Expression mapExp;
  beforeEach(&mapExp, mapPro, 5);
  mapExp.env = env;
  
  Expression filterExp;
  beforeEach(&filterExp, filterPro, 5);
  filterExp.env = env;

  Map map;
  map.attribute = 1;
  map.expression = &mapExp;

  Filter filter;
  filter.predicate = &filterExp;

  Operation mapOp;
  mapOp.operation.map = &map;
  mapOp.unionCase = 0;

  Operation filterOp;
  filterOp.operation.filter = &filter;
  filterOp.unionCase = 1;

  Operation ops[2] = {mapOp, filterOp};

  Query query1;
  query1.operations = ops;
  query1.amount = 2;

  Instruction mapPro2[5] = {{{CONST}, 0}, {.data._int = 2, 2}, {{VAR}, 0}, {.data._int = 0, 2}, {{MUL}, 0}};

  Expression mapExp2;
  beforeEach(&mapExp2, mapPro2, 5);
  mapExp2.env = env;

  Map map2;
  map2.attribute = 1;
  map2.expression = &mapExp2;

  Operation mapOp2;
  mapOp2.operation.map = &map2;
  mapOp2.unionCase = 0;

  Query query2;
  query2.operations = &mapOp2;
  query2.amount = 1;

  Query queries[2] = {query1, query2};

  Message msg;
  msg.amount = 2;
  msg.queries = queries;

  // Action
  OutputMessage output;
  executeQueries(msg, &output, env);

  // Assert
  if(env->stack->stack[0].type._int != 8){
    res.failed = 1;
    char msg[341];
    sprintf(msg, "Expected %d, got %d", 8, env->stack->stack[0].type._int);
    res.message = msg;
    return res;
  }

  if(env->stack->stack[1].type._int != 4){
    res.failed = 1;
    char msg[341];
    sprintf(msg, "Expected %d, got %d", 4, env->stack->stack[1].type._int);
    res.message = msg;
    return res;
  }

  if(env->stack->stack[2].type._int != 0){
    res.failed = 1;
    char msg[341];
    sprintf(msg, "Expected %d, got %d", 0, env->stack->stack[2].type._int);
    res.message = msg;
    return res;
  }
  
  return res;

}