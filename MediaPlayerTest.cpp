#include "LogTestResult.h"
#include "TestHarness.h"

int main()
{
        LogTestResult tr;
        TestRegistry::runAllTests(tr);
        return tr.getFailureCount();
}

TEST( MediaPlayer, helloword)
{
    int r = 0;
    CHECK ( 0 == r );
}

TEST( MediaPlayer, helloword_fail)
{
    int r = 0;
    CHECK ( 1 == r );
}
