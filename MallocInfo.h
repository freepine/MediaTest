#ifndef MALLOCINFO_H
#define MALLOCINFO_H

const size_t MAX_LEAK_MESSAGE_LENGTH = 1024;
const size_t MAX_FILE_NAME_SIZE = 256;

class MallocInfo
{
public:
    MallocInfo();
    ~MallocInfo();

    void dumpPreTestMemory();
    void dumpPostTestMemory();
    bool hasMemoryLeak() const;
    char* getLeakMessage();
    
private:
    void dumpProcessMemory(size_t &count, size_t &totalMemory);
    bool copyFile(const char* sourceFile, const char* destFile);

    size_t mPreCount;
    size_t mPreTotalMemory;
    size_t mPostCount;
    size_t mPostTotalMemory;

    size_t mMemoryDumpId;
    char mLeakMessage[MAX_LEAK_MESSAGE_LENGTH];
    char mDumpFileName[MAX_FILE_NAME_SIZE];
};

#endif
