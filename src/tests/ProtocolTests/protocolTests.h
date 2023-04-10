#ifndef PROTOCOLTESTS_H
#define PROTOCOLTESTS_H

#include "../testType.h"

TestResult runProtocolTests();

Test prepareInstructionTest();

Test prepareExpressionTest();

Test prepareMapTest();

Test prepareQueryTest();

Test prepareMessageTest();

Test instructionGetsEncoded();

Test expressionGetsEncoded();

Test mapGetsEncoded();

Test messageGetsEncoded();

#endif // PROTOCOLTESTS_H