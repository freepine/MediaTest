#define LOG_NDEBUG 0
#define LOG_TAG "MemoryLeakPlugin"
#include <utils/Log.h>

#include "MemoryLeakPlugin.h"
#include "MallocInfo.h"
#include "CppUTest/SimpleString.h"
#include "CppUTest/Utest.h"
#include "CppUTest/Failure.h"
#include "CppUTest/TestResult.h"

extern "C" void get_malloc_leak_info(uint8_t** info, size_t* overallSize,
        size_t* infoSize, size_t* totalMemory, size_t* backtraceSize);
extern "C" void free_malloc_leak_info(uint8_t* info);

MemoryLeakPlugin::MemoryLeakPlugin() : TestPlugin("MemoryLeakDetection")
{
}

MemoryLeakPlugin::~MemoryLeakPlugin()
{
}

void MemoryLeakPlugin::preTestAction(Utest& test, TestResult& testresult)
{
    mMemory.dumpPreTestMemory();
}

void MemoryLeakPlugin::postTestAction(Utest& test, TestResult& testresult)
{
    mMemory.dumpPostTestMemory();
    if(mMemory.hasMemoryLeak())
    {
        Failure f(&test, mMemory.getLeakMessage());
        testresult.addFailure(f);
    }
}

