///////////////////////////////////////////////////////////////////////////////
//
// LOGTESTRESULT.H
// 
// Log the test result in Android log system.
// 
///////////////////////////////////////////////////////////////////////////////



#ifndef LOGTESTRESULT_H
#define LOGTESTRESULT_H

#include "TestResult.h"

class LogTestResult: public TestResult
{
public:
					LogTestResult (); 
					virtual ~LogTestResult() {}

	virtual void	testsStarted ();
	virtual void	addFailure (const Failure& failure);
	virtual void	testsEnded ();
};

#endif
