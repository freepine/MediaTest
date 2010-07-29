#ifndef MEMORYLEAKPLUGIN_H
#define MEMORYLEAKPLUGIN_H

#include "MallocInfo.h"
#include "CppUTest/TestPlugin.h"

class MemoryLeakPlugin: public TestPlugin
{
public:

        MemoryLeakPlugin();
        virtual ~MemoryLeakPlugin();

        virtual void preTestAction(Utest&, TestResult&);
        virtual void postTestAction(Utest&, TestResult&);

private:
        MallocInfo mMemory;
};

#endif
