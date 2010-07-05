#define LOG_NDEBUG 0
#define LOG_TAG "LogTestResult"
#include <utils/Log.h>

#include "LogTestResult.h"
#include "Failure.h"

LogTestResult::LogTestResult ()
	: TestResult ()
{
}


void LogTestResult::testsStarted () 
{
    LOGV("LogTestResult::testsStarted");
}


void LogTestResult::addFailure (const Failure& failure) 
{
    TestResult::addFailure(failure);
    LOGE("%s%s%s%s%ld%s%s\n",
		"Failure: \"",
		failure.message.asCharString (),
		"\" " ,
		"line ",
		failure.lineNumber,
		" in ",
		failure.fileName.asCharString ());
}


void LogTestResult::testsEnded () 
{
    LOGV("LogTestResult::testsEnded, failureCount=%d", getFailureCount());
}
