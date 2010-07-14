#include "LogTestResult.h"
#include "TestHarness.h"
#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>

using namespace android;

int main()
{
        sp<ProcessState> proc = ProcessState::self();
        proc->startThreadPool();
        LogTestResult tr;
        TestRegistry::runAllTests(tr);
        IPCThreadState::self()->stopProcess();
        return tr.getFailureCount();
}
