#include <stdio.h>
#include "runTest.h"
#include "ExpressionTests.h"
#include "protocolTests.h"
#include "testType.h"


void runTests(TestToRun testToRun) {
  TestResult res;
  res.total = 0;
  res.passed = 0;
  printf("Running tests\n\n");

  if(testToRun == ALL || testToRun == EXPRESSION) executeExpressionTests(&res);

  if(testToRun == ALL || testToRun == PROTOCOL) executeProtocolTests(&res);

  if(testToRun == ALL){
    printf("\n---------------------------------\n");
    printf("All Tests Result: %d/%d passed", res.passed, res.total);
    printf("\n---------------------------------\n\n");
  }
}

void executeExpressionTests(TestResult *res){
  TestResult expressionTestRes = runExpressionTests();

  printTests(&expressionTestRes, expressionTestRes.total, "Expression Tests");

  res->total += expressionTestRes.total;
  res->passed += expressionTestRes.passed;

}

void executeProtocolTests(TestResult *res){
  TestResult protocolTestRes = runProtocolTests();
  
  printTests(&protocolTestRes, protocolTestRes.total, "Protocol Tests");

  res->total += protocolTestRes.total;
  res->passed += protocolTestRes.passed;

}

void printTests(TestResult *testSuite, int numTests, char *testSuiteName){
  printf("Printing %s\n\n", testSuiteName);
  for(int i = 0; i < numTests; i++) {
    printf("Test %d: ", i + 1);
    if(testSuite->tests[i].failed) {
        printf("Test %s failed 🚫\n", testSuite->tests[i].name);
        printf("\t- %s\n", testSuite->tests[i].message);
    } else {
        printf("Test %s passed ✅ \n", testSuite->tests[i].name);
        testSuite->passed++;
    }
  }

  printf("\n---------------------------------\n");
  printf("%s Result: %d/%d passed", testSuiteName, testSuite->passed, testSuite->total);
  printf("\n---------------------------------\n\n");
}