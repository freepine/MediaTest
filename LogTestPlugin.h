#include "CppUTest/TestPlugin.h"

class LogTestPlugin: public TestPlugin
{
public:

        LogTestPlugin();
        virtual ~LogTestPlugin()
        {
        }

        virtual void preTestAction(Utest&, TestResult&);
        virtual void postTestAction(Utest&, TestResult&);
};
