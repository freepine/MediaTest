#define LOG_NDEBUG 0
#define LOG_TAG "Main"
#include <utils/Log.h>

#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>
#include "LogTestPlugin.h"
#include "MemoryLeakPlugin.h"
#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/TestPlugin.h"
#include "CppUTest/TestRegistry.h"

using namespace android;

int main(int ac, char** av)
{
    ALOGV("entering main");
    sp<ProcessState> proc = ProcessState::self();
    proc->startThreadPool();
    ALOGV("install memory leak plugin.");
    MemoryLeakPlugin* memoryLeakPlugin = new MemoryLeakPlugin();
    TestRegistry::getCurrentRegistry()->installPlugin(memoryLeakPlugin);
    ALOGV("install logger plugin.");
    LogTestPlugin* logPlugin = new LogTestPlugin();
    TestRegistry::getCurrentRegistry()->installPlugin(logPlugin);
    ALOGV("before run all tests.");
    int result = CommandLineTestRunner::RunAllTests(ac, av);
    ALOGV("after run all tests, reset plugin");
    TestRegistry::getCurrentRegistry()->resetPlugins();
    delete logPlugin;
    delete memoryLeakPlugin;

    IPCThreadState::self()->flushCommands();
    IPCThreadState::self()->stopProcess();
    //IPCThreadState::shutdown();
    sleep(1);
    ALOGV("exit main.");
    return result;
}

IMPORT_TEST_GROUP( MediaPlayer);
//IMPORT_TEST_GROUP( ASFExtractor);
