#define LOG_NDEBUG 0
#define LOG_TAG "MallocInfo"
#include <utils/Log.h>
#include <sys/stat.h>
#include <errno.h>
#include <utils/String8.h>
#include "MallocInfo.h"

extern "C" void get_malloc_leak_info(uint8_t** info, size_t* overallSize,
        size_t* infoSize, size_t* totalMemory, size_t* backtraceSize);
extern "C" void free_malloc_leak_info(uint8_t* info);

const char* CPPUTEST_MEMORYDUMP_DIR = "/mnt/sdcard/cpputest";

using namespace android;

MallocInfo::MallocInfo()
    :mPreCount(0)
    ,mPreTotalMemory(0)
    ,mPostCount(0)
    ,mPostTotalMemory(0)
    ,mMemoryDumpId(0)
{
    if (mkdir(CPPUTEST_MEMORYDUMP_DIR, 0777) < 0)
    {
        ALOGW("mkdir %s failed: %d", CPPUTEST_MEMORYDUMP_DIR, errno);
    }
}

MallocInfo::~MallocInfo()
{
}

void MallocInfo::dumpPreTestMemory()
{
    mMemoryDumpId++;
    snprintf(mDumpFileName, MAX_FILE_NAME_SIZE, "%s/memory_dump_%d", CPPUTEST_MEMORYDUMP_DIR, mMemoryDumpId);
    dumpProcessMemory(mPreCount, mPreTotalMemory);
    mPostCount = 0;
    mPostTotalMemory = 0;
}

void MallocInfo::dumpPostTestMemory()
{
    char mapsFile[MAX_FILE_NAME_SIZE];
    strcpy(mapsFile, mDumpFileName);
    strcat(mapsFile, ".maps");
    copyFile("/proc/self/maps", mapsFile);
    if(strlen(mDumpFileName) >= MAX_FILE_NAME_SIZE-5)
    {
        ALOGE("mDumpFileName exceeds size limit: %s", mDumpFileName);
    }
    else
    {
        strcat(mDumpFileName, ".pst");
    }
    dumpProcessMemory(mPostCount, mPostTotalMemory);
}

bool MallocInfo::hasMemoryLeak() const
{
    ALOGI("MallocInfo::hasMemoryLeak, mPreCount=%d, mPostCount=%d, mPreTotalMemory=%d, mPostTotalMemory=%d", 
               mPreCount, mPostCount, mPreTotalMemory, mPostTotalMemory);
    return ((mPostCount != mPreCount) || (mPreTotalMemory != mPostTotalMemory));
}

char* MallocInfo::getLeakMessage()
{
    snprintf(mLeakMessage, MAX_LEAK_MESSAGE_LENGTH, "Possible memory leak, allocations from %d to %d, total memory from %d to %d\n",
                            mPreCount, mPostCount, mPreTotalMemory, mPostTotalMemory);
    strcat(mLeakMessage, "Memory dump stored in:");
    strcat(mLeakMessage, mDumpFileName);
    return mLeakMessage;
}

//Mostly copied from MediaPlayerService::memStatus()
void MallocInfo::dumpProcessMemory(size_t &count, size_t &totalMemory)
{
    const size_t SIZE = 256;
    char buffer[SIZE];
    String8 result;

    typedef struct {
        size_t size;
        size_t dups;
        intptr_t * backtrace;
    } AllocEntry;
    uint8_t *info = NULL;
    size_t overallSize = 0;
    size_t infoSize = 0;
    size_t backtraceSize = 0;
    count = 0;
    totalMemory = 0;
    get_malloc_leak_info(&info, &overallSize, &infoSize, &totalMemory, &backtraceSize);
    ALOGI("returned from get_malloc_leak_info, info=0x%x, overallSize=%d, infoSize=%d, totalMemory=%d, backtraceSize=%d", (int)info, overallSize, infoSize, totalMemory, backtraceSize);
    if (info)
    {
        FILE *f = fopen(mDumpFileName, "w+");
        if(f == NULL)
        {
            ALOGE("Open file failed: %s!", mDumpFileName);
            free_malloc_leak_info(info);
            return;
        }
        uint8_t *ptr = info;
        count = overallSize / infoSize;

        snprintf(buffer, SIZE, " Allocation count %i\n", count);
        result.append(buffer);
        snprintf(buffer, SIZE, " Total memory %i\n", totalMemory);
        result.append(buffer);
        AllocEntry * entries = new AllocEntry[count];

        for (size_t i = 0; i < count; i++) {
            // Each entry should be size_t, size_t, intptr_t[backtraceSize]
            AllocEntry *e = &entries[i];

            e->size = *reinterpret_cast<size_t *>(ptr);
            ptr += sizeof(size_t);

            e->dups = *reinterpret_cast<size_t *>(ptr);
            ptr += sizeof(size_t);

            e->backtrace = reinterpret_cast<intptr_t *>(ptr);
            ptr += sizeof(intptr_t) * backtraceSize;
        }

        // Now we need to sort the entries.  They come sorted by size but
        // not by stack trace which causes problems using diff.
        bool moved;
        do {
            moved = false;
            for (size_t i = 0; i < (count - 1); i++) {
                AllocEntry *e1 = &entries[i];                AllocEntry *e2 = &entries[i+1];

                bool swap = e1->size < e2->size;
                if (e1->size == e2->size) {
                    for(size_t j = 0; j < backtraceSize; j++) {
                        if (e1->backtrace[j] == e2->backtrace[j]) {
                            continue;
                        }
                        swap = e1->backtrace[j] < e2->backtrace[j];
                        break;
                    }
                }
                if (swap) {
                    AllocEntry t = entries[i];
                    entries[i] = entries[i+1];
                    entries[i+1] = t;
                    moved = true;
                }
            }
        } while (moved);
        for (size_t i = 0; i < count; i++) {
            AllocEntry *e = &entries[i];

            snprintf(buffer, SIZE, "size %8i, dup %4i, ", e->size, e->dups);
            result.append(buffer);
            for (size_t ct = 0; (ct < backtraceSize) && e->backtrace[ct]; ct++) {
                if (ct) {
                    result.append(", ");
                }
                snprintf(buffer, SIZE, "0x%08x", e->backtrace[ct]);
                result.append(buffer);
            }
            result.append("\n");
        }

        delete[] entries;
        write(fileno(f), result.string(), result.size());
        fclose(f);

        free_malloc_leak_info(info);
    }
}

bool MallocInfo::copyFile(const char* sourceFile, const char* destFile)
{
    const size_t READ_BLOCK_SIZE = 4096;
    FILE* src = fopen(sourceFile, "r");
    if(src == NULL)
    {
        ALOGE("Open source file failed: %s!", sourceFile);
        return false;
    }
    FILE* dest = fopen(destFile, "w+");
    if(dest == NULL)
    {
        ALOGE("Open dest file failed: %s!", destFile);
        fclose(src);
        return false;
    }
    char buffer[READ_BLOCK_SIZE];
    int readNum = 0;
    while(!feof(src))
    {
        readNum = fread(buffer, 1, READ_BLOCK_SIZE, src);
        if(readNum > 0)
        {
            fwrite(buffer, 1, readNum, dest);
        }
        else
        {
            ALOGE("Read error, readNum=%d, errno=%d", readNum, errno);
            break;
        }
    }
    fclose(src);
    fclose(dest);
    return true;
}
