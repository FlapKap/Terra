#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "embUnit.h"

#include "stack.h"
#include "environment.h"
#include "expression.h"
#include "expression_tests.h"
#include "execution.h"
#include <terraprotocol.pb.h>


static Expression expres;
static Number stack_memory[20];
static Stack stack;
static Number env_memory[20];
static Env env;

static void setUp(void)
{
  
  env_init_env(env_memory,sizeof(Number)/sizeof(env_memory), &env);
  stack_init_stack(stack_memory, sizeof(Number)/sizeof(stack_memory), &stack);
  expres.env = &env;
  expres.stack = &stack;
  expres.pc = 0;
}

static void tearDown(void)
{
  env_clear_env(expres.env);
  stack_clear_stack(expres.stack);

}

// static void Adding a const to the stack
static void testPushToStack(void)
{
  // Arrange
  TerraProtocol_Expression p = TerraProtocol_Expression_init_zero;
  p.instructions[0] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_CONST}};
  p.instructions[1] = (TerraProtocol_Data) {TerraProtocol_Data__int8_tag, {3}};
  p.instructions_count = 2;
  expres.program = &p;

  // Act
  int actual = expression_call(&expres).type._int;
  int expected = 3;
  // Assert
  TEST_ASSERT_EQUAL_INT(expected, actual);
}

// static void pushing var from env to stack
static void testPushVarToStack(void)
{
  // Arrange
  TerraProtocol_Expression p = TerraProtocol_Expression_init_zero;
  p.instructions[0] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_VAR}};
  p.instructions[1] = (TerraProtocol_Data) {TerraProtocol_Data__uint32_tag, {0}};
  p.instructions_count = 2;
  expres.program = &p;

  Number val = {{10}, NUMBER_INT32};
  env_set_value(expres.env, 0, val);

  // Act
  Number popped = expression_call(&expres);
  int actual = popped.type._int;
  int expected = 10;

  // Assert
  TEST_ASSERT_EQUAL_INT(expected, actual);
}

static void testAnd1(void)
{
  // Arrange
  TerraProtocol_Expression p = TerraProtocol_Expression_init_zero;
  p.instructions[0] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_CONST}};
  p.instructions[1] = (TerraProtocol_Data) {TerraProtocol_Data__int8_tag, {1}};
  p.instructions[2] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_CONST}};
  p.instructions[3] = (TerraProtocol_Data) {TerraProtocol_Data__int8_tag, {1}};
  p.instructions[4] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_AND}};
  p.instructions_count = 5;
  expres.program = &p;

  // Act
  int actual = expression_call(&expres).type._int;

  // Assert
  TEST_ASSERT(actual);
}

static void testAnd2(void)
{
  // Arrange
  TerraProtocol_Expression p = TerraProtocol_Expression_init_zero;
  p.instructions[0] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_CONST}};
  p.instructions[1] = (TerraProtocol_Data) {TerraProtocol_Data__int8_tag, {0}};
  p.instructions[2] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_CONST}};
  p.instructions[3] = (TerraProtocol_Data) {TerraProtocol_Data__int8_tag, {0}};
  p.instructions[4] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_AND}};
  p.instructions_count = 5;
  expres.program = &p;

  // Act
  int actual = expression_call(&expres).type._int;

  // Assert
  TEST_ASSERT(!actual);
}

static void testAnd3(void)
{
  // Arrange
  TerraProtocol_Expression p = TerraProtocol_Expression_init_zero;
  p.instructions[0] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_CONST}};
  p.instructions[1] = (TerraProtocol_Data) {TerraProtocol_Data__int8_tag, {1}};
  p.instructions[2] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_CONST}};
  p.instructions[3] = (TerraProtocol_Data) {TerraProtocol_Data__int8_tag, {0}};
  p.instructions[4] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_AND}};
  p.instructions_count = 5;
  expres.program = &p;

  // Act
  int actual = expression_call(&expres).type._int;

  // Assert
  TEST_ASSERT(!actual);
}


static void testOr1(void)
{
  // Arrange
  TerraProtocol_Expression p = TerraProtocol_Expression_init_zero;
  p.instructions[0] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_CONST}};
  p.instructions[1] = (TerraProtocol_Data) {TerraProtocol_Data__int8_tag, {1}};
  p.instructions[2] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_CONST}};
  p.instructions[3] = (TerraProtocol_Data) {TerraProtocol_Data__int8_tag, {0}};
  p.instructions[4] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_OR}};
  p.instructions_count = 5;
  expres.program = &p;

  // Act
  int actual = expression_call(&expres).type._int;

  // Assert
  TEST_ASSERT(actual);
}

static void testOr2(void)
{
  // Arrange
  TerraProtocol_Expression p = TerraProtocol_Expression_init_zero;
  p.instructions[0] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_CONST}};
  p.instructions[1] = (TerraProtocol_Data) {TerraProtocol_Data__int8_tag, {0}};
  p.instructions[2] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_CONST}};
  p.instructions[3] = (TerraProtocol_Data) {TerraProtocol_Data__int8_tag, {0}};
  p.instructions[4] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_OR}};
  p.instructions_count = 5;
  expres.program = &p;

  // Act
  int actual = expression_call(&expres).type._int;

  // Assert
  TEST_ASSERT(!actual);
}

static void testNot1(void)
{
  // Arrange
  TerraProtocol_Expression p = TerraProtocol_Expression_init_zero;
  p.instructions[0] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_CONST}};
  p.instructions[1] = (TerraProtocol_Data) {TerraProtocol_Data__int8_tag, {0}};
  p.instructions[2] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_NOT}};
  p.instructions_count = 3;
  
  expres.program = &p;

  // Act
  int actual = expression_call(&expres).type._int;

  // Assert
  TEST_ASSERT(actual);
}

static void testNot2(void)
{
  // Arrange
  TerraProtocol_Expression p = TerraProtocol_Expression_init_zero;
  p.instructions[0] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_CONST}};
  p.instructions[1] = (TerraProtocol_Data) {TerraProtocol_Data__double_tag, {0.0}};
  p.instructions[2] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_NOT}};
  p.instructions_count = 3;

  expres.program = &p;

  // Act
  int actual = expression_call(&expres).type._int;

  // Assert
  TEST_ASSERT(actual);
}

static void testNot3(void)
{
  // Arrange
  TerraProtocol_Expression p = TerraProtocol_Expression_init_zero;
  p.instructions[0] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_CONST}};
  p.instructions[1] = (TerraProtocol_Data) {TerraProtocol_Data__uint32_tag, {5}};
  p.instructions[2] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_NOT}};
  p.instructions_count = 3;

  expres.program = &p;

  // Act
  int actual = expression_call(&expres).type._int;

  // Assert
  TEST_ASSERT(!actual);
}

static void testNot4(void)
{
  // Arrange
  TerraProtocol_Expression p = TerraProtocol_Expression_init_zero;
  p.instructions[0] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_CONST}};
  p.instructions[1] = (TerraProtocol_Data) {TerraProtocol_Data__int32_tag, {1}};
  p.instructions[2] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_NOT}};
  p.instructions_count = 3;

  expres.program = &p;

  // Act
  int actual = expression_call(&expres).type._int;

  // Assert
  TEST_ASSERT(!actual);
}


static void testLT1(void)
{
  // Arrange
  TerraProtocol_Expression p = TerraProtocol_Expression_init_zero;
  p.instructions[0] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_CONST}};
  p.instructions[1] = (TerraProtocol_Data) {TerraProtocol_Data__double_tag, {1.0}};
  p.instructions[2] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_CONST}};
  p.instructions[3] = (TerraProtocol_Data) {TerraProtocol_Data__int32_tag, {5}};
  p.instructions[4] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_LT}};
  p.instructions_count = 5;

  expres.program = &p;

  // Act
  int actual = expression_call(&expres).type._int;

  // Assert
  TEST_ASSERT(actual);
}

static void testLT2(void)
{
  // Arrange
  TerraProtocol_Expression p = TerraProtocol_Expression_init_zero;
  p.instructions[0] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_CONST}};
  p.instructions[1] = (TerraProtocol_Data) {TerraProtocol_Data__uint32_tag, {7}};
  p.instructions[2] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_CONST}};
  p.instructions[3] = (TerraProtocol_Data) {TerraProtocol_Data__float_tag, {3.4f}};
  p.instructions[4] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_LT}};
  p.instructions_count = 5;

  expres.program = &p;

  // Act
  int actual = expression_call(&expres).type._int;

  // Assert
  TEST_ASSERT(!actual);
}

static void testGT1(void)
{
  // Arrange
  TerraProtocol_Expression p = TerraProtocol_Expression_init_zero;
  p.instructions[0] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_CONST}};
  p.instructions[1] = (TerraProtocol_Data) {TerraProtocol_Data__int32_tag, {-3}};
  p.instructions[2] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_CONST}};
  p.instructions[3] = (TerraProtocol_Data) {TerraProtocol_Data__uint32_tag, {5}};
  p.instructions[4] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_GT}};
  p.instructions_count = 5;

  expres.program = &p;

  // Act
  int actual = expression_call(&expres).type._int;

  // Assert
  TEST_ASSERT(!actual);
}

static void testGT2(void)
{
  // Arrange
  TerraProtocol_Expression p = TerraProtocol_Expression_init_zero;
  p.instructions[0] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_CONST}};
  p.instructions[1] = (TerraProtocol_Data) {TerraProtocol_Data__double_tag, {3.14}};
  p.instructions[2] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_CONST}};
  p.instructions[3] = (TerraProtocol_Data) {TerraProtocol_Data__int32_tag, {0}};
  p.instructions[4] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_GT}};
  p.instructions_count = 5;

  expres.program = &p;


  // Act
  int actual = expression_call(&expres).type._int;
  // Assert
  TEST_ASSERT(actual);
}

static void testEqual(void)
{
  // Arrange
  TerraProtocol_Expression p = TerraProtocol_Expression_init_zero;
  p.instructions[0] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_CONST}};
  p.instructions[1] = (TerraProtocol_Data) {TerraProtocol_Data__int32_tag, {10}};
  p.instructions[2] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_CONST}};
  p.instructions[3] = (TerraProtocol_Data) {TerraProtocol_Data__int32_tag, {10}};
  p.instructions[4] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_EQ}};
  p.instructions_count = 5;

  expres.program = &p;

  // Action
  Number actual = expression_call(&expres);

  // Assert
  TEST_ASSERT(actual.type._int);
}


static void testAdd(void)
{
  // Arrange
  TerraProtocol_Expression p = TerraProtocol_Expression_init_zero;
  p.instructions[0] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_CONST}};
  p.instructions[1] = (TerraProtocol_Data) {TerraProtocol_Data__int32_tag, {10}};
  p.instructions[2] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_CONST}};
  p.instructions[3] = (TerraProtocol_Data) {TerraProtocol_Data__int32_tag, {3}};
  p.instructions[4] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_ADD}};
  p.instructions_count = 5;

  expres.program = &p;

  // Action
  Number actual = expression_call(&expres);
  int expected = 13;

  // Assert
  TEST_ASSERT_EQUAL_INT(expected, actual.type._int);
}

static void testSub(void)
{
  // Arrange
  TerraProtocol_Expression p = TerraProtocol_Expression_init_zero;
  p.instructions[0] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_CONST}};
  p.instructions[1] = (TerraProtocol_Data) {TerraProtocol_Data__int32_tag, {10}};
  p.instructions[2] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_CONST}};
  p.instructions[3] = (TerraProtocol_Data) {TerraProtocol_Data__int32_tag, {3}};
  p.instructions[4] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_SUB}};
  p.instructions_count = 5;

  expres.program = &p;

  // Action
  Number actual = expression_call(&expres);
  int expected = 7;

  // Assert
  TEST_ASSERT_EQUAL_INT(expected, actual.type._int);
}

static void testMul(void)
{
  // Arrange
  TerraProtocol_Expression p = TerraProtocol_Expression_init_zero;
  p.instructions[0] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_CONST}};
  p.instructions[1] = (TerraProtocol_Data) {TerraProtocol_Data__int32_tag, {10}};
  p.instructions[2] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_CONST}};
  p.instructions[3] = (TerraProtocol_Data) {TerraProtocol_Data__int32_tag, {3}};
  p.instructions[4] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_MUL}};
  p.instructions_count = 5;

  expres.program = &p;

  // Action
  Number actual = expression_call(&expres);
  int expected = 30;

  // Assert
  TEST_ASSERT(NUMBER_INT32 == actual.unionCase);
  TEST_ASSERT_EQUAL_INT(expected, actual.type._int);
}

static void testDiv(void)
{
  // Arrange
  TerraProtocol_Expression p = TerraProtocol_Expression_init_zero;
  p.instructions[0] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_CONST}};
  p.instructions[1] = (TerraProtocol_Data) {TerraProtocol_Data__int32_tag, {10}};
  p.instructions[2] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_CONST}};
  p.instructions[3] = (TerraProtocol_Data) {TerraProtocol_Data__int32_tag, {3}};
  p.instructions[4] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_DIV}};
  p.instructions_count = 5;

  expres.program = &p;

  // Action
  Number actual = expression_call(&expres);
  int expected = 3;

  // Assert
  TEST_ASSERT(NUMBER_INT32 == actual.unionCase);
  TEST_ASSERT_EQUAL_INT(expected, actual.type._int);
}

static void testMod(void)
{
  // Arrange
  TerraProtocol_Expression p = TerraProtocol_Expression_init_zero;
  p.instructions[0] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_CONST}};
  p.instructions[1] = (TerraProtocol_Data) {TerraProtocol_Data__int32_tag, {10}};
  p.instructions[2] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_CONST}};
  p.instructions[3] = (TerraProtocol_Data) {TerraProtocol_Data__int32_tag, {3}};
  p.instructions[4] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_MOD}};
  p.instructions_count = 5;

  expres.program = &p;

  // Action
  Number actual = expression_call(&expres);
  int expected = 1;

  // Assert
  TEST_ASSERT(NUMBER_INT32 == actual.unionCase);
  TEST_ASSERT_EQUAL_INT(expected, actual.type._int);
}

void testLog(void)
{
  // Arrange
  TerraProtocol_Expression p = TerraProtocol_Expression_init_zero;
  p.instructions[0] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_CONST}};
  p.instructions[1] = (TerraProtocol_Data) {.which_data = TerraProtocol_Data__double_tag, .data._double = 10.0};
  p.instructions[2] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_LOG}};
  p.instructions_count = 3;

  expres.program = &p;

  // Action
  Number actual = expression_call(&expres);
  double expected = log(10);
  printf("actual: %lf, expected: %lf\n", actual.type._double, expected);
  printf("cases: %d, %d\n", actual.unionCase, NUMBER_DOUBLE);
  // Assert
  TEST_ASSERT(NUMBER_DOUBLE == actual.unionCase);
  TEST_ASSERT(actual.type._double >= expected - 0.001 && actual.type._double <= expected + 0.001);
  
}

static void testPow(void)
{
  // Arrange
  TerraProtocol_Expression p = TerraProtocol_Expression_init_zero;
  p.instructions[0] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_CONST}};
  p.instructions[1] = (TerraProtocol_Data) {TerraProtocol_Data__int32_tag, {2}};
  p.instructions[2] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_CONST}};
  p.instructions[3] = (TerraProtocol_Data) {TerraProtocol_Data__int32_tag, {3}};
  p.instructions[4] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_POW}};
  p.instructions_count = 5;

  expres.program = &p;

  // Action
  Number actual = expression_call(&expres);
  double expected = pow(2, 3);

  // Assert
  TEST_ASSERT(NUMBER_DOUBLE == actual.unionCase);
  TEST_ASSERT(actual.type._double >= expected - 0.001 && actual.type._double <= expected + 0.001);
}

static void testSqrt(void)
{
  // Arrange
  TerraProtocol_Expression p = TerraProtocol_Expression_init_zero;
  p.instructions[0] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_CONST}};
  p.instructions[1] = (TerraProtocol_Data) {TerraProtocol_Data__int32_tag, {4}};
  p.instructions[2] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_SQRT}};
  p.instructions_count = 3;

  expres.program = &p;

  // Action
  Number actual = expression_call(&expres);
  double expected = sqrt(4);

  // Assert
  TEST_ASSERT(NUMBER_DOUBLE == actual.unionCase);
  TEST_ASSERT(actual.type._double >= expected - 0.001 && actual.type._double <= expected + 0.001);
}


static void testExpValue(void)
{
  // Arrange
  TerraProtocol_Expression p = TerraProtocol_Expression_init_zero;
  p.instructions[0] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_CONST}};
  p.instructions[1] = (TerraProtocol_Data) {.which_data = TerraProtocol_Data__int32_tag, .data._int32 = 1};
  p.instructions[2] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_EXP}};
  p.instructions_count = 3;

  expres.program = &p;

  // Action
  Number actual = expression_call(&expres);
  double expected = exp(1);

  // Assert
  TEST_ASSERT(NUMBER_DOUBLE == actual.unionCase);
  TEST_ASSERT(actual.type._double >= expected - 0.001 && actual.type._double <= expected + 0.001);
}

static void testCeilValue(void)
{
  // Arrange
  TerraProtocol_Expression p = TerraProtocol_Expression_init_zero;
  p.instructions[0] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_CONST}};
  p.instructions[1] = (TerraProtocol_Data) {.which_data = TerraProtocol_Data__double_tag, .data._double = 1.4};
  p.instructions[2] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_CEIL}};
  p.instructions_count = 3;

  expres.program = &p;

  // Action
  Number actual = expression_call(&expres);
  int expected = 2;

  // Assert
  TEST_ASSERT(NUMBER_INT32 == actual.unionCase);
  TEST_ASSERT_EQUAL_INT(expected, actual.type._int);
}

static void testCeilValueBig(void)
{
  // Arrange
  TerraProtocol_Expression p = TerraProtocol_Expression_init_zero;
  p.instructions[0] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_CONST}};
  p.instructions[1] = (TerraProtocol_Data) {.which_data = TerraProtocol_Data__double_tag, .data._double = 1.9};
  p.instructions[2] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_CEIL}};
  p.instructions_count = 3;

  expres.program = &p;

  // Action
  Number actual = expression_call(&expres);
  int expected = 2;

  // Assert
  TEST_ASSERT(NUMBER_INT32 == actual.unionCase);
  TEST_ASSERT_EQUAL_INT(expected, actual.type._int);
}

static void testFloorValue(void)
{
  // Arrange
  TerraProtocol_Expression p = TerraProtocol_Expression_init_zero;
  p.instructions[0] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_CONST}};
  p.instructions[1] = (TerraProtocol_Data) {.which_data = TerraProtocol_Data__double_tag, .data._double = 1.4};
  p.instructions[2] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_FLOOR}};
  p.instructions_count = 3;

  expres.program = &p;

  // Action
  Number actual = expression_call(&expres);
  int expected = 1;

  // Assert
  TEST_ASSERT(NUMBER_INT32 == actual.unionCase);
  TEST_ASSERT_EQUAL_INT(expected, actual.type._int);
}

static void testFloorValueBig(void)
{
  // Arrange
  TerraProtocol_Expression p = TerraProtocol_Expression_init_zero;
  p.instructions[0] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_CONST}};
  p.instructions[1] = (TerraProtocol_Data) {.which_data = TerraProtocol_Data__double_tag, .data._double = 1.9};
  p.instructions[2] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_FLOOR}};
  p.instructions_count = 3;

  expres.program = &p;

  // Action
  Number actual = expression_call(&expres);
  int expected = 1;

  // Assert
  TEST_ASSERT(NUMBER_INT32 == actual.unionCase);
  TEST_ASSERT_EQUAL_INT(expected, actual.type._int);
}

static void testRoundUnderHalf(void)
{
  // Arrange
  TerraProtocol_Expression p = TerraProtocol_Expression_init_zero;
  p.instructions[0] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_CONST}};
  p.instructions[1] = (TerraProtocol_Data) {.which_data = TerraProtocol_Data__double_tag, .data._double = 1.4};
  p.instructions[2] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_ROUND}};
  p.instructions_count = 3;
  
  expres.program = &p;

  // Action
  Number actual = expression_call(&expres);
  int expected = 1;
  
  // Assert
  TEST_ASSERT(NUMBER_INT32 == actual.unionCase);
  TEST_ASSERT_EQUAL_INT(expected, actual.type._int);
}

static void testRoundOverHalf(void)
{
  // Arrange
  TerraProtocol_Expression p = TerraProtocol_Expression_init_zero;
  p.instructions[0] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_CONST}};
  p.instructions[1] = (TerraProtocol_Data) {.which_data = TerraProtocol_Data__double_tag, .data._double = 1.6};
  p.instructions[2] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_ROUND}};
  p.instructions_count = 3;

  expres.program = &p;

  // Action
  Number actual = expression_call(&expres);
  int expected = 2;
  // Assert
  TEST_ASSERT(NUMBER_INT32 == actual.unionCase);
  TEST_ASSERT_EQUAL_INT(expected, actual.type._int);
}

static void testAbsoluteValue1(void)
{
  // Arrange
  TerraProtocol_Expression p = TerraProtocol_Expression_init_zero;
  p.instructions[0] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_CONST}};
  p.instructions[1] = (TerraProtocol_Data) {.which_data = TerraProtocol_Data__int16_tag, .data._int16 = 5};
  p.instructions[2] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_ABS}};
  p.instructions_count = 3;

  expres.program = &p;

  // Action
  Number actual = expression_call(&expres);
  double expected = 5;

  // Assert
  TEST_ASSERT(NUMBER_DOUBLE == actual.unionCase);
  TEST_ASSERT(actual.type._double >= expected - 0.001 && actual.type._double <= expected + 0.001);
}

static void testAbsoluteValue2(void)
{
  // Arrange
  TerraProtocol_Expression p = TerraProtocol_Expression_init_zero;
  p.instructions[0] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_CONST}};
  p.instructions[1] = (TerraProtocol_Data) {.which_data = TerraProtocol_Data__int16_tag, .data._int16 = -5};
  p.instructions[2] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_ABS}};
  p.instructions_count = 3;

  expres.program = &p;

  // Action
  Number actual = expression_call(&expres);
  double expected = 5;

  // Assert
  TEST_ASSERT(NUMBER_DOUBLE == actual.unionCase);
  TEST_ASSERT(actual.type._double >= expected - 0.001 && actual.type._double <= expected + 0.001);
}

static void testLessThanEqual1(void)
{
  // Arrange
  TerraProtocol_Expression p = TerraProtocol_Expression_init_zero;
  p.instructions[0] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_CONST}};
  p.instructions[1] = (TerraProtocol_Data) {.which_data = TerraProtocol_Data__int16_tag, .data._int16 = 2};
  p.instructions[2] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_CONST}};
  p.instructions[3] = (TerraProtocol_Data) {.which_data = TerraProtocol_Data__int16_tag, .data._int16 = 3};
  p.instructions[4] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_LTEQ}};
  p.instructions_count = 5;

  expres.program = &p;

  // Act
  Number actual = expression_call(&expres);
  int expected = 1;

  // Assert
  TEST_ASSERT(NUMBER_INT32 == actual.unionCase);
  TEST_ASSERT_EQUAL_INT(expected, actual.type._int);
}

static void testLessThanEqual2(void)
{
  // Arrange
  TerraProtocol_Expression p = TerraProtocol_Expression_init_zero;
  p.instructions[0] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_CONST}};
  p.instructions[1] = (TerraProtocol_Data) {.which_data = TerraProtocol_Data__int16_tag, .data._int16 = 3};
  p.instructions[2] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_CONST}};
  p.instructions[3] = (TerraProtocol_Data) {.which_data = TerraProtocol_Data__int16_tag, .data._int16 = 3};
  p.instructions[4] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_LTEQ}};
  p.instructions_count = 5;

  expres.program = &p;

  // Action
  Number actual = expression_call(&expres);
  int expected = 1;

  // Assert
  TEST_ASSERT(NUMBER_INT32 == actual.unionCase);
  TEST_ASSERT_EQUAL_INT(expected, actual.type._int);
}

static void testGreaterThanEqual1(void)
{
  // Arrange
  TerraProtocol_Expression p = TerraProtocol_Expression_init_zero;
  p.instructions[0] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_CONST}};
  p.instructions[1] = (TerraProtocol_Data) {.which_data = TerraProtocol_Data__int16_tag, .data._int16 = 4};
  p.instructions[2] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_CONST}};
  p.instructions[3] = (TerraProtocol_Data) {.which_data = TerraProtocol_Data__int16_tag, .data._int16 = 3};
  p.instructions[4] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_GTEQ}};
  p.instructions_count = 5;

  expres.program = &p;

  // Action
  Number actual = expression_call(&expres);
  int expected = 1;

  // Assert
  TEST_ASSERT(NUMBER_INT32 == actual.unionCase);
  TEST_ASSERT_EQUAL_INT(expected, actual.type._int);
}

static void testGreaterThanEqual2(void)
{
  // Arrange
  TerraProtocol_Expression p = TerraProtocol_Expression_init_zero;
  p.instructions[0] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_CONST}};
  p.instructions[1] = (TerraProtocol_Data) {.which_data = TerraProtocol_Data__int16_tag, .data._int16 = 3};
  p.instructions[2] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_CONST}};
  p.instructions[3] = (TerraProtocol_Data) {.which_data = TerraProtocol_Data__int16_tag, .data._int16 = 3};
  p.instructions[4] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_GTEQ}};
  p.instructions_count = 5;

  expres.program = &p;

  // Action
  Number actual = expression_call(&expres);
  int expected = 1;

  // Assert
  TEST_ASSERT(NUMBER_INT32 == actual.unionCase);
  TEST_ASSERT_EQUAL_INT(expected, actual.type._int);
}


static void test_execute_query_with_result(void)
{
  //[CONST, 2, VAR, 0, MUL]

  // Arrange
  Number envVal;
  envVal.type._int = 4;
  envVal.unionCase = NUMBER_INT32;

  env_set_value(expres.env, 0, envVal);

  TerraProtocol_Query query = TerraProtocol_Query_init_zero;

  query.operations_count = 1;
  query.operations[0].which_operation = TerraProtocol_Operation_map_tag;
  query.operations[0].operation.map.attribute = 1;
  query.operations[0].operation.map.function.instructions_count = 5;
  query.operations[0].operation.map.function.instructions[0] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_CONST}};
  query.operations[0].operation.map.function.instructions[1] = (TerraProtocol_Data) {.which_data = TerraProtocol_Data__int16_tag, .data._int16 = 2};
  query.operations[0].operation.map.function.instructions[2] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_VAR}};
  query.operations[0].operation.map.function.instructions[3] = (TerraProtocol_Data) {TerraProtocol_Data__int16_tag, .data._int16 = 0};
  query.operations[0].operation.map.function.instructions[4] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_MUL}};

  // Action
  executeQuery(&query, expres.env, expres.stack);

  // Assert
  // that the environment has the result

  TEST_ASSERT(NUMBER_INT32 == expres.env->memory[1].unionCase);
  TEST_ASSERT_EQUAL_INT(8, expres.env->memory[1].type._int);

  TEST_ASSERT_EQUAL_INT(-1, expres.stack->top); // stack should be empty
}

static void test_execute_query_without_result(void)
{
  Number envVal;
  envVal.type._int = 4;
  envVal.unionCase = NUMBER_INT32;

  env_set_value(expres.env, 0, envVal);

  TerraProtocol_Query query = TerraProtocol_Query_init_zero;

  query.operations_count = 2;
  query.operations[0].which_operation = TerraProtocol_Operation_map_tag;
  query.operations[0].operation.map.attribute = 1;
  query.operations[0].operation.map.function.instructions_count = 5;
  query.operations[0].operation.map.function.instructions[0] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_CONST}};
  query.operations[0].operation.map.function.instructions[1] = (TerraProtocol_Data) {.which_data = TerraProtocol_Data__int16_tag, .data._int16 = 2};
  query.operations[0].operation.map.function.instructions[2] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_VAR}};
  query.operations[0].operation.map.function.instructions[3] = (TerraProtocol_Data) {TerraProtocol_Data__int16_tag, .data._int16 = 0};
  query.operations[0].operation.map.function.instructions[4] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_MUL}};
  query.operations[1].which_operation = TerraProtocol_Operation_filter_tag;
  query.operations[1].operation.filter.predicate.instructions_count = 5;
  query.operations[1].operation.filter.predicate.instructions[0] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_VAR}};
  query.operations[1].operation.filter.predicate.instructions[1] = (TerraProtocol_Data) {TerraProtocol_Data__int16_tag, .data._int16 = 1};
  query.operations[1].operation.filter.predicate.instructions[2] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_CONST}};
  query.operations[1].operation.filter.predicate.instructions[3] = (TerraProtocol_Data) {.which_data = TerraProtocol_Data__int16_tag, .data._int16 = 2};
  query.operations[1].operation.filter.predicate.instructions[4] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_LT}};
  
  // Action
  executeQuery(&query, expres.env, expres.stack);

  // Assert
  TEST_ASSERT(NUMBER_INT32 == expres.stack->stack_memory[0].unionCase);
  TEST_ASSERT_EQUAL_INT(0, expres.stack->stack_memory[0].type._int);
}


static void test_execute_query_with_multiple_results(void)
{
  Number envVal;
  envVal.type._int = 4;
  envVal.unionCase = NUMBER_INT32;

  env_set_value(expres.env, 0, envVal);


  TerraProtocol_Query query = TerraProtocol_Query_init_zero;

  query.operations_count = 2;
  query.operations[0].which_operation = TerraProtocol_Operation_map_tag;
  query.operations[0].operation.map.attribute = 1;
  query.operations[0].operation.map.function.instructions_count = 5;
  query.operations[0].operation.map.function.instructions[0] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_CONST}};
  query.operations[0].operation.map.function.instructions[1] = (TerraProtocol_Data) {.which_data = TerraProtocol_Data__int16_tag, .data._int16 = 2};
  query.operations[0].operation.map.function.instructions[2] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_VAR}};
  query.operations[0].operation.map.function.instructions[3] = (TerraProtocol_Data) {TerraProtocol_Data__int16_tag, .data._int16 = 0};
  query.operations[0].operation.map.function.instructions[4] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_MUL}};
  
  query.operations[1].which_operation = TerraProtocol_Operation_map_tag;
  query.operations[1].operation.map.attribute = 2;
  query.operations[1].operation.map.function.instructions_count = 5;
  query.operations[1].operation.map.function.instructions[0] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_VAR}};
  query.operations[1].operation.map.function.instructions[1] = (TerraProtocol_Data) {TerraProtocol_Data__int16_tag, .data._int16 = 1};
  query.operations[1].operation.map.function.instructions[2] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_CONST}};
  query.operations[1].operation.map.function.instructions[3] = (TerraProtocol_Data) {.which_data = TerraProtocol_Data__int16_tag, .data._int16 = 2};
  query.operations[1].operation.map.function.instructions[4] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_ADD}};
  

  executeQuery(&query, expres.env, expres.stack);

  // Assert

  // output of query 1
  TEST_ASSERT_EQUAL_INT(8, env.memory[1].type._int);
  TEST_ASSERT_EQUAL_INT(NUMBER_INT32, env.memory[1].unionCase);

  //output of query 2
  TEST_ASSERT_EQUAL_INT(10, env.memory[2].type._int);
  TEST_ASSERT_EQUAL_INT(NUMBER_INT32, env.memory[1].unionCase);

  //check that stack is empty
  TEST_ASSERT_EQUAL_INT(-1, expres.stack->top);
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
      new_TestFixture(testExpValue),
      new_TestFixture(testCeilValue),
      new_TestFixture(testCeilValueBig),
      new_TestFixture(testFloorValue),
      new_TestFixture(testFloorValueBig),
      new_TestFixture(testRoundUnderHalf),
      new_TestFixture(testRoundOverHalf),
      new_TestFixture(testAbsoluteValue1),
      new_TestFixture(testAbsoluteValue2),
      new_TestFixture(testLessThanEqual1),
      new_TestFixture(testLessThanEqual2),
      new_TestFixture(testGreaterThanEqual1),
      new_TestFixture(testGreaterThanEqual2),
      // // The original testsuite
      new_TestFixture(test_execute_query_with_result),
      new_TestFixture(test_execute_query_without_result),
      new_TestFixture(test_execute_query_with_multiple_results),
  };
  EMB_UNIT_TESTCALLER(expression_tests, &setUp, &tearDown, fixtures);
  return (TestRef)&expression_tests;
}
