#define LOG_NDEBUG 0
#define LOG_TAG "CppUTestLog"
#include <utils/Log.h>

#include "LogTestPlugin.h"
#include "CppUTest/SimpleString.h"
#include "CppUTest/Utest.h"
#include "CppUTest/TestResult.h"

LogTestPlugin::LogTestPlugin() : TestPlugin("AndroidLog")
{
}

void LogTestPlugin::preTestAction(Utest& test, TestResult& testresult)
{
    ALOGV("Before running %s::%s", test.getGroup().asCharString(), test.getName().asCharString());
}

void LogTestPlugin::postTestAction(Utest& test, TestResult& testresult)
{
    ALOGV("After running %s::%s, fail count=%d", test.getGroup().asCharString(), test.getName().asCharString(), testresult.getFailureCount());
}

