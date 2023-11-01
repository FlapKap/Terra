#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "embUnit.h"

#include "stack.h"
#include "environment.h"
#include "expression.h"
#include "expression_tests.h"
#include "execution.h"

static Expression expres;

static void setUp(void)
{
  expres.env = init_env();
  expres.stack = expres.env->stack;
  // exp->program = program;
  // exp->p_size = p_size;
}

static void tearDown(void)
{
  free(expres.env->memory);
  free(expres.env->stack->stack);
  free(expres.env->stack);
  //free(expres.program);
}

// static void Adding a const to the stack
static void testPushToStack(void)
{
  // Arrange
  Instruction p[2] = {{{CONST}, 0}, {.data._int = 3, INSTRUCTION_INT}};
  expres.program = p;
  expres.p_size = 2;
  // Act
  int actual = call(&expres).type._int;
  int expected = 3;
  // Assert
  TEST_ASSERT_EQUAL_INT(expected, actual);
}

// static void pushing var from env to stack
static void testPushVarToStack(void)
{
  // Arrange
  Instruction p[2] = {{{VAR}, 0}, {.data._int = 0, INSTRUCTION_INT}};

  expres.program = p;
  expres.p_size = 2;
  Number val = {{10}, 2};
  set_value(expres.env, 0, val);

  // Act
  Number popped = call(&expres);
  int actual = popped.type._int;
  int expected = 10;

  // Assert
  TEST_ASSERT_EQUAL_INT(expected, actual);
}

static void testAnd1(void)
{
  // Arrange
  Instruction p[5] = {{{CONST}, 0}, {.data._int = 1, INSTRUCTION_INT}, {{CONST}, 0}, {.data._int = 1, INSTRUCTION_INT}, {{AND}, 0}};
  expres.program = p;
  expres.p_size = 5;
  // Act
  int actual = call(&expres).type._int;

  // Assert
  TEST_ASSERT(actual);
}

static void testAnd2(void)
{
  // Arrange

  Instruction p[5] = {{{CONST}, 0}, {.data._int = 1, INSTRUCTION_INT}, {{CONST}, 0}, {.data._int = 0, INSTRUCTION_INT}, {{AND}, 0}};
  expres.program = p;
  expres.p_size = 5;
  // Act
  int actual = call(&expres).type._int;

  // Assert
  TEST_ASSERT(!actual);
}

static void testAnd3(void)
{
  // Arrange

  Instruction p[5] = {{{CONST}, 0}, {.data._int = 0, INSTRUCTION_INT}, {{CONST}, 0}, {.data._int = 0, INSTRUCTION_INT}, {{AND}, 0}};

  expres.program = p;
  expres.p_size = 5;
  // Act
  int actual = call(&expres).type._int;

  // Assert
  TEST_ASSERT(!actual);
}

static void testOr1(void)
{
  // Arrange

  Instruction p[5] = {{{CONST}, 0}, {.data._int = 1, INSTRUCTION_INT}, {{CONST}, 0}, {.data._int = 0, INSTRUCTION_INT}, {{OR}, 0}};

  expres.program = p;
  expres.p_size = 5;
  // Act
  int actual = call(&expres).type._int;

  // Assert
  TEST_ASSERT(actual);
}

static void testOr2(void)
{
  // Arrange

  Instruction p[5] = {{{CONST}, 0}, {.data._int = 0, INSTRUCTION_INT}, {{CONST}, 0}, {.data._int = 0, INSTRUCTION_INT}, {{OR}, 0}};

  expres.program = p;
  expres.p_size = 5;
  // Act
  int actual = call(&expres).type._int;

  // Assert
  TEST_ASSERT(!actual);
}

static void testNot1(void)
{
  // Arrange

  Instruction p[3] = {{{CONST}, 0}, {.data._int = 0, INSTRUCTION_INT}, {{NOT}, 0}};

  expres.program = p;
  expres.p_size = 3;
  // Act
  int actual = call(&expres).type._int;

  // Assert
  TEST_ASSERT(actual);
}

static void testNot2(void)
{
  // Arrange

  Instruction p[3] = {{{CONST}, 0}, {.data._double = 0.0, 4}, {{NOT}, 0}};

  expres.program = p;
  expres.p_size = 3;
  // Act
  int actual = call(&expres).type._int;

  // Assert
  TEST_ASSERT(actual);
}

static void testNot3(void)
{
  // Arrange

  Instruction p[3] = {{{CONST}, 0}, {.data._uint32 = 5, 1}, {{NOT}, 0}};

  expres.program = p;
  expres.p_size = 3;
  // Act
  int actual = call(&expres).type._int;

  // Assert
  TEST_ASSERT(!actual);
}

static void testNot4(void)
{
  // Arrange

  Instruction p[3] = {{{CONST}, 0}, {.data._int = 1, INSTRUCTION_INT}, {{NOT}, 0}};

  expres.program = p;
  expres.p_size = 3;
  // Act
  int actual = call(&expres).type._int;

  // Assert
  TEST_ASSERT(!actual);
}

static void testLT1(void)
{
  // Arrange

  Instruction p[5] = {{{CONST}, 0}, {.data._double = 1.0, INSTRUCTION_DOUBLE}, {{CONST}, 0}, {.data._int = 5, INSTRUCTION_INT}, {{LT}, 0}};

  expres.program = p;
  expres.p_size = 5;
  // Act
  int actual = call(&expres).type._int;

  // Assert
  TEST_ASSERT(actual);
}

static void testLT2(void)
{
  // Arrange
  Instruction p[5] = {{{CONST}, 0}, {.data._uint32 = 7, INSTRUCTION_UINT32}, {{CONST}, 0}, {.data._float = 3.4f, INSTRUCTION_FLOAT}, {{LT}, 0}};

  expres.program = p;
  expres.p_size = 5;
  // Act
  int actual = call(&expres).type._int;

  // Assert
  TEST_ASSERT(!actual);
}

static void testGT1(void)
{
  // Arrange
  Instruction p[5] = {{{CONST}, 0}, {.data._int = -3, INSTRUCTION_INT}, {{CONST}, 0}, {.data._uint32 = 5, INSTRUCTION_UINT32}, {{GT}, 0}};

  expres.program = p;
  expres.p_size = 5;
  // Act
  int actual = call(&expres).type._int;

  // Assert
  TEST_ASSERT(!actual);
}

static void testGT2(void)
{
  // Arrange

  Instruction p[5] = {{{CONST}, 0}, {.data._double = 3.14, INSTRUCTION_DOUBLE}, {{CONST}, 0}, {.data._int = 0, INSTRUCTION_INT}, {{GT}, 0}};
  expres.program = p;
  expres.p_size = 5;
  // Act
  int actual = call(&expres).type._int;
  // Assert
  TEST_ASSERT(actual);
}

static void testEqual(void)
{

  Instruction p[5] = {{{CONST}, 0}, {.data._int = 10, INSTRUCTION_INT}, {{CONST}, 0}, {.data._int = 10, INSTRUCTION_INT}, {{EQ}, 0}};

  expres.program = p;
  expres.p_size = 5;
  // Action
  Number actual = call(&expres);

  // Assert
  TEST_ASSERT(actual.type._int);
}

static void testAdd(void)
{

  Instruction p[5] = {{{CONST}, 0}, {.data._int = 10, INSTRUCTION_INT}, {{CONST}, 0}, {.data._int = 3, INSTRUCTION_INT}, {{ADD}, 0}};

  expres.program = p;
  expres.p_size = 5;
  // Action
  Number actual = call(&expres);
  int expected = 13;

  // Assert
  TEST_ASSERT_EQUAL_INT(expected, actual.type._int);
}

static void testSub(void)
{

  Instruction p[5] = {{{CONST}, 0}, {.data._int = 10, INSTRUCTION_INT}, {{CONST}, 0}, {.data._int = 3, INSTRUCTION_INT}, {{SUB}, 0}};

  expres.program = p;
  expres.p_size = 5;
  // Action
  Number actual = call(&expres);
  int expected = 7;

  // Assert
  TEST_ASSERT_EQUAL_INT(expected, actual.type._int);
}

static void testMul(void)
{

  Instruction p[5] = {{{CONST}, 0}, {.data._int = 10, INSTRUCTION_INT}, {{CONST}, 0}, {.data._int = 3, INSTRUCTION_INT}, {{MUL}, 0}};

  expres.program = p;
  expres.p_size = 5;
  // Action
  Number actual = call(&expres);
  int expected = 30;

  // Assert
  TEST_ASSERT(NUMBER_INT == actual.unionCase);
  TEST_ASSERT_EQUAL_INT(expected, actual.type._int);
}

static void testDiv(void)
{

  Instruction p[5] = {{{CONST}, 0}, {.data._int = 10, INSTRUCTION_INT}, {{CONST}, 0}, {.data._int = 3, INSTRUCTION_INT}, {{DIV}, 0}};

  expres.program = p;
  expres.p_size = 5;
  // Action
  Number actual = call(&expres);
  int expected = 3;

  // Assert
  TEST_ASSERT(NUMBER_INT == actual.unionCase);
  TEST_ASSERT_EQUAL_INT(expected, actual.type._int);
}

static void testMod(void)
{

  Instruction p[5] = {{{CONST}, 0}, {.data._int = 10, INSTRUCTION_INT}, {{CONST}, 0}, {.data._int = 3, INSTRUCTION_INT}, {{MOD}, 0}};

  expres.program = p;
  expres.p_size = 5;
  // Action
  Number actual = call(&expres);
  int expected = 1;

  // Assert
  TEST_ASSERT(NUMBER_INT == actual.unionCase);
  TEST_ASSERT_EQUAL_INT(expected, actual.type._int);
}

void testLog(void)
{

  Instruction p[3] = {{{CONST}, 0}, {.data._double = 10, INSTRUCTION_DOUBLE}, {{LOG}, 0}};
  expres.program = p;
  expres.p_size = 3;
  // Action
  Number actual = call(&expres);
  double expected = log(10);

  // Assert
  TEST_ASSERT(NUMBER_DOUBLE == actual.unionCase);
  TEST_ASSERT(actual.type._double >= expected - 0.001 && actual.type._double <= expected + 0.001);
}

static void testPow(void)
{

  Instruction p[5] = {{{CONST}, 0}, {.data._int = 2, INSTRUCTION_INT}, {{CONST}, 0}, {.data._int = 3, INSTRUCTION_INT}, {{POW}, 0}};

  expres.program = p;
  expres.p_size = 5;
  // Action
  Number actual = call(&expres);
  double expected = pow(2, 3);

  // Assert
  TEST_ASSERT(NUMBER_DOUBLE == actual.unionCase);
  TEST_ASSERT(actual.type._double >= expected - 0.001 && actual.type._double <= expected + 0.001);
}

static void testSqrt(void)
{

  Instruction p[3] = {{{CONST}, 0}, {.data._int = 4, INSTRUCTION_INT}, {{SQRT}, 0}};

  expres.program = p;
  expres.p_size = 3;
  // Action
  Number actual = call(&expres);
  double expected = sqrt(4);

  // Assert
  TEST_ASSERT(NUMBER_DOUBLE == actual.unionCase);
  TEST_ASSERT(actual.type._double >= expected - 0.001 && actual.type._double <= expected + 0.001);
}

static void expValue(void)
{

  Instruction p[3] = {{{CONST}, 0}, {.data._int = 1, INSTRUCTION_INT}, {{EXP}, 0}};

  expres.program = p;
  expres.p_size = 3;
  // Action
  Number actual = call(&expres);
  double expected = exp(1);

  // Assert
  TEST_ASSERT(NUMBER_DOUBLE == actual.unionCase);
  TEST_ASSERT(actual.type._double >= expected - 0.001 && actual.type._double <= expected + 0.001);
}

static void ceilValue(void)
{

  Instruction p[3] = {{{CONST}, 0}, {.data._double = 1.4, INSTRUCTION_DOUBLE}, {{CEIL}, 0}};
  expres.p_size = 3;
  expres.program = p;

  // Action
  Number actual = call(&expres);
  int expected = 2;

  // Assert
  TEST_ASSERT(NUMBER_INT == actual.unionCase);
  TEST_ASSERT_EQUAL_INT(expected, actual.type._int);
}

static void ceilValueBig(void)
{

  Instruction p[3] = {{{CONST}, 0}, {.data._double = 1.9, INSTRUCTION_DOUBLE}, {{CEIL}, 0}};
  expres.p_size = 3;
  expres.program = p;

  // Action
  Number actual = call(&expres);
  int expected = 2;

  // Assert
  TEST_ASSERT(NUMBER_INT == actual.unionCase);
  TEST_ASSERT_EQUAL_INT(expected, actual.type._int);
}

static void floorValue(void)
{

  Instruction p[3] = {{{CONST}, 0}, {.data._double = 1.4, INSTRUCTION_DOUBLE}, {{FLOOR}, 0}};
  expres.p_size = 3;
  expres.program = p;

  // Action
  Number actual = call(&expres);
  int expected = 1;

  // Assert
  TEST_ASSERT(NUMBER_INT == actual.unionCase);
  TEST_ASSERT_EQUAL_INT(expected, actual.type._int);
}

static void floorValueBig(void)
{

  Instruction p[3] = {{{CONST}, 0}, {.data._double = 1.9, INSTRUCTION_DOUBLE}, {{FLOOR}, 0}};
  expres.p_size = 3;
  expres.program = p;

  // Action
  Number actual = call(&expres);
  int expected = 1;

  // Assert
  TEST_ASSERT(NUMBER_INT == actual.unionCase);
  TEST_ASSERT_EQUAL_INT(expected, actual.type._int);
}

static void roundUnderHalf(void)
{

  Instruction p[3] = {{{CONST}, 0}, {.data._double = 1.4, INSTRUCTION_DOUBLE}, {{ROUND}, 0}};
  expres.p_size = 3;
  expres.program = p;

  // Action
  Number actual = call(&expres);
  int expected = 1;
  
  // Assert
  TEST_ASSERT(NUMBER_INT == actual.unionCase);
  TEST_ASSERT_EQUAL_INT(expected, actual.type._int);
}

static void roundOverHalf(void)
{

  Instruction p[3] = {{{CONST}, 0}, {.data._double = 1.6, INSTRUCTION_DOUBLE}, {{ROUND}, 0}};
  expres.p_size = 3;
  expres.program = p;

  // Action
  Number actual = call(&expres);
  int expected = 2;
  // Assert
  TEST_ASSERT(NUMBER_INT == actual.unionCase);
  TEST_ASSERT_EQUAL_INT(expected, actual.type._int);
}

static void absoluteValue1(void)
{

  Instruction p[3] = {{{CONST}, 0}, {.data._int = 5, INSTRUCTION_INT}, {{ABS}, 0}};
  expres.p_size = 3;
  expres.program = p;

  // Action
  Number actual = call(&expres);
  double expected = 5;

  // Assert
  TEST_ASSERT(NUMBER_DOUBLE == actual.unionCase);
  TEST_ASSERT(actual.type._double >= expected - 0.001 && actual.type._double <= expected + 0.001);
}

static void absoluteValue2(void)
{

  Instruction p[3] = {{{CONST}, 0}, {.data._int = -5, INSTRUCTION_INT}, {{ABS}, 0}};
  expres.p_size = 3;
  expres.program = p;

  // Action
  Number actual = call(&expres);
  double expected = 5;

  // Assert
  TEST_ASSERT(NUMBER_DOUBLE == actual.unionCase);
  TEST_ASSERT(actual.type._double >= expected - 0.001 && actual.type._double <= expected + 0.001);
}

static void lessThanEqual1(void)
{

  Instruction p[5] = {{{CONST}, 0}, {.data._int = 2, INSTRUCTION_INT}, {{CONST}, 0}, {.data._int = 3, INSTRUCTION_INT}, {{LTEQ}, 0}};

  expres.program = p;
  expres.p_size = 5;
  // Act
  Number actual = call(&expres);
  int expected = 1;

  // Assert
  TEST_ASSERT(NUMBER_INT == actual.unionCase);
  TEST_ASSERT_EQUAL_INT(expected, actual.type._int);
}

static void lessThanEqual2(void)
{

  Instruction p[5] = {{{CONST}, 0}, {.data._int = 3, INSTRUCTION_INT}, {{CONST}, 0}, {.data._int = 3, INSTRUCTION_INT}, {{LTEQ}, 0}};
  expres.program = p;
  expres.p_size = 5;
  // Action
  Number actual = call(&expres);
  int expected = 1;

  // Assert
  TEST_ASSERT(NUMBER_INT == actual.unionCase);
  TEST_ASSERT_EQUAL_INT(expected, actual.type._int);
}

static void greaterThanEqual1(void)
{

  Instruction p[5] = {{{CONST}, 0}, {.data._int = 4, INSTRUCTION_INT}, {{CONST}, 0}, {.data._int = 3, INSTRUCTION_INT}, {{GTEQ}, 0}};
  expres.program = p;
  expres.p_size = 5;
  // Action
  Number actual = call(&expres);
  int expected = 1;

  // Assert
  TEST_ASSERT(NUMBER_INT == actual.unionCase);
  TEST_ASSERT_EQUAL_INT(expected, actual.type._int);
}

static void greaterThanEqual2(void)
{

  Instruction p[5] = {{{CONST}, 0}, {.data._int = 3, INSTRUCTION_INT}, {{CONST}, 0}, {.data._int = 3, INSTRUCTION_INT}, {{GTEQ}, 0}};
  expres.program = p;
  expres.p_size = 5;
  // Action
  Number actual = call(&expres);
  int expected = 1;

  // Assert
  TEST_ASSERT(NUMBER_INT == actual.unionCase);
  TEST_ASSERT_EQUAL_INT(expected, actual.type._int);
}

static void og_test_execute_query_with_result(void)
{
  //[CONST, 2, VAR, 0, MUL]
  Instruction p[5] = {{{CONST}, 0}, {.data._int = 2, INSTRUCTION_INT}, {{VAR}, 0}, {.data._int = 0, INSTRUCTION_INT}, {{MUL}, 0}};

  Number envVal;
  envVal.type._int = 4;
  envVal.unionCase = NUMBER_INT;

  expres.program = p;
  expres.p_size = 5;
  set_value(expres.env, 0, envVal);

  Map map;
  map.attribute = 1;
  map.expression = &expres;

  Operation op;
  op.operation.map = &map;
  op.unionCase = 0;

  Query query;
  query.operations = &op;
  query.amount = 1;

  // Action
  QueryResponse queryRes;
  executeQuery(query, &queryRes, expres.env);

  // Assert
  TEST_ASSERT(NUMBER_INT == expres.env->stack->stack[0].unionCase);
  TEST_ASSERT_EQUAL_INT(8, expres.env->stack->stack[0].type._int);

  TEST_ASSERT_EQUAL_INT(-1, expres.env->stack->top); // stack should be empty
}

static void og_test_execute_query_without_result(void)
{

  Instruction mapPro[5] = {{{CONST}, 0}, {.data._int = 2, INSTRUCTION_INT}, {{VAR}, 0}, {.data._int = 0, INSTRUCTION_INT}, {{MUL}, 0}};
  Instruction filterPro[5] = {{{VAR}, 0}, {.data._int = 1, INSTRUCTION_INT}, {{CONST}, 0}, {.data._int = 2, INSTRUCTION_INT}, {{LT}, 0}};

  Number envVal;
  envVal.type._int = 4;
  envVal.unionCase = NUMBER_INT;

  Env *env = init_env();
  set_value(env, 0, envVal);

  Expression mapExp;
  mapExp.program = mapPro;
  mapExp.p_size = 5;
  mapExp.env = env;

  Expression filterExp;
  filterExp.program = filterPro;
  filterExp.p_size = 5;
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
  TEST_ASSERT(NUMBER_INT == env->stack->stack[0].unionCase);
  TEST_ASSERT_EQUAL_INT(0, env->stack->stack[0].type._int);
}

static void og_test_execute_quries_single_result(void)
{

  Instruction p[5] = {{{CONST}, 0}, {.data._int = 2, INSTRUCTION_INT}, {{VAR}, 0}, {.data._int = 0, INSTRUCTION_INT}, {{MUL}, 0}};

  Number envVal;
  envVal.type._int = 4;
  envVal.unionCase = NUMBER_INT;

  expres.program = p;
  expres.p_size = 5;
  set_value(expres.env, 0, envVal);

  Map map;
  map.attribute = 1;
  map.expression = &expres;

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
  executeQueries(msg, &output, expres.env);

  // Assert
  TEST_ASSERT(NUMBER_INT == expres.env->stack->stack[0].unionCase);
  TEST_ASSERT_EQUAL_INT(8, expres.env->stack->stack[0].type._int);

  TEST_ASSERT_EQUAL_INT(NUMBER_INT, expres.env->stack->stack[1].unionCase);
  TEST_ASSERT_EQUAL_INT(4, expres.env->stack->stack[1].type._int);
}

static void og_test_execute_quries_multiple_results(void)
{

  Instruction mapPro[5] = {{{CONST}, 0}, {.data._int = 2, INSTRUCTION_INT}, {{VAR}, 0}, {.data._int = 0, INSTRUCTION_INT}, {{MUL}, 0}};
  Instruction filterPro[5] = {{{VAR}, 0}, {.data._int = 1, INSTRUCTION_INT}, {{CONST}, 0}, {.data._int = 2, INSTRUCTION_INT}, {{LT}, 0}};

  Number envVal;
  envVal.type._int = 4;
  envVal.unionCase = NUMBER_INT;

  Env *env = init_env();
  set_value(env, 0, envVal);

  Expression mapExp;
  mapExp.program = mapPro;
  mapExp.p_size = 5;
  mapExp.env = env;

  Expression filterExp;
  filterExp.program = filterPro;
  filterExp.p_size = 5;
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

  Instruction mapPro2[5] = {{{CONST}, 0}, {.data._int = 2, INSTRUCTION_INT}, {{VAR}, 0}, {.data._int = 0, INSTRUCTION_INT}, {{MUL}, 0}};

  Expression mapExp2;
  mapExp2.program = mapPro2;
  mapExp2.p_size = 5;
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
  TEST_ASSERT_EQUAL_INT(2, output.amount);
  // output of query 1
  TEST_ASSERT_EQUAL_INT(1, output.responses[0].amount);
  TEST_ASSERT_EQUAL_INT(2, output.responses[0].response->unionCase);
  TEST_ASSERT_EQUAL_INT(0, output.responses[0].response->data._int);

  //output of query 2
  TEST_ASSERT_EQUAL_INT(1, output.responses[1].amount);
  TEST_ASSERT_EQUAL_INT(2, output.responses[1].response->unionCase);
  TEST_ASSERT_EQUAL_INT(8, output.responses[1].response->data._int);

  //check that stack is empty
  TEST_ASSERT_EQUAL_INT(-1, env->stack->top);
}
TestRef tests_expression(void)
{
  EMB_UNIT_TESTFIXTURES(fixtures){
      new_TestFixture(testPushToStack),
      new_TestFixture(testPushVarToStack),
      new_TestFixture(testAnd1),
      new_TestFixture(testAnd2),
      new_TestFixture(testAnd3),
      new_TestFixture(testOr1),
      new_TestFixture(testOr2),
      new_TestFixture(testNot1),
      new_TestFixture(testNot2),
      new_TestFixture(testNot3),
      new_TestFixture(testNot4),
      new_TestFixture(testLT1),
      new_TestFixture(testLT2),
      new_TestFixture(testGT1),
      new_TestFixture(testGT2),
      new_TestFixture(testEqual),
      new_TestFixture(testAdd),
      new_TestFixture(testSub),
      new_TestFixture(testMul),
      new_TestFixture(testDiv),
      new_TestFixture(testMod),
      new_TestFixture(testLog),
      new_TestFixture(testPow),
      new_TestFixture(testSqrt),
      new_TestFixture(expValue),
      new_TestFixture(ceilValue),
      new_TestFixture(ceilValueBig),
      new_TestFixture(floorValue),
      new_TestFixture(floorValueBig),
      new_TestFixture(roundUnderHalf),
      new_TestFixture(roundOverHalf),
      new_TestFixture(absoluteValue1),
      new_TestFixture(absoluteValue2),
      new_TestFixture(lessThanEqual1),
      new_TestFixture(lessThanEqual2),
      new_TestFixture(greaterThanEqual1),
      new_TestFixture(greaterThanEqual2),
      // The original testsuite
      new_TestFixture(og_test_execute_query_with_result),
      new_TestFixture(og_test_execute_query_without_result),
      new_TestFixture(og_test_execute_quries_single_result),
      new_TestFixture(og_test_execute_quries_multiple_results),
  };
  EMB_UNIT_TESTCALLER(expression_tests, &setUp, &tearDown, fixtures);
  return (TestRef)&expression_tests;
}
