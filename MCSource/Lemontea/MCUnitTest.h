#include <assert.h>
#include "MCContext.h"
#include "MCException.h"
/* Assert Functions */
//void assert( int expression);
//all the exception will be catched inside the MCUnitTestCase, user should not catch them
void assertYES(int expression) throws(MCAssertYESException);
void assertNO(int expression) throws(MCAssertNOException);
void assertNil(void* ptr) throws(MCAssertNilException);
void assertNotNil(void* ptr) throws(MCAssertNotNilException);
void assertSame(mo obj1, mo obj2) throws(MCAssertSameException);
void assertNotSame(mo obj1, mo obj2) throws(MCAssertNotSameException);
void assertEquals(int exp1, int exp2) throws(MCAssertEqualsException);
void fail(char* message);

/* Test Result */

#ifndef MCUnitTestResult_
#define MCUnitTestResult_
	
class(MCUnitTestResult, MCObject);

public(MCUnitTestResult, void, bye, voida);
public(MCUnitTestResult, void, addSuccessInfo, char* succinfo);
public(MCUnitTestResult, void, addFailInfo, char* failinfo);
#endif

/* Test Case */

#ifndef MCUnitTestCase_
#define MCUnitTestCase_ 

class(MCUnitTestCase, MCObject,
	MCUnitTestResult* unitTestResultRef;
	struct MCUnitTestCaseStruct* next_case;
);

public(MCUnitTestCase, MCUnitTestCase*, initWithTestResult, MCUnitTestResult* resultRef);
public(MCUnitTestCase, void, bye, voida);
public(MCUnitTestCase, void, setUp, voida);
public(MCUnitTestCase, void, tearDown, voida);
public(MCUnitTestCase, void, runTests, voida);
public(MCUnitTestCase, void, runATestMethod, char* methodName);
#endif

/* Test Suite */

#ifndef MCUnitTestSuite_
#define MCUnitTestSuite_

class(MCUnitTestSuite, MCObject,
	MCUnitTestCase *first_case;
    MCUnitTestCase **last_case_p;
	int test_case_count;
	struct MCUnitTestSuiteStruct* next_suite;
);

public(MCUnitTestSuite, void, bye, voida);
public(MCUnitTestSuite, void, addTestCase, MCUnitTestCase* volatile tcase);
public(MCUnitTestSuite, void, runTestCases, voida);
#endif

/* Test Runner */

#ifndef MCUnitTestRunner_
#define MCUnitTestRunner_

class(MCUnitTestRunner, MCObject,
	MCUnitTestResult* unitTestResult;
	MCUnitTestSuite* first_suite;
	int test_suite_count;
);

public(MCUnitTestRunner, void, bye, voida);
public(MCUnitTestRunner, void, addTestSuite, MCUnitTestSuite* testSuite);
public(MCUnitTestRunner, void, runTestSuites, voida);

#endif