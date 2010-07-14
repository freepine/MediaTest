#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>
#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/TestPlugin.h"

using namespace android;

int main(int ac, char** av)
{
    sp<ProcessState> proc = ProcessState::self();
    proc->startThreadPool();
    return CommandLineTestRunner::RunAllTests(ac, av);
}

IMPORT_TEST_GROUP( MediaPlayer);
