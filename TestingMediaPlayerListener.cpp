#define LOG_NDEBUG 0
#define LOG_TAG "TestingMediaPlayerListener"
#include <utils/Log.h>
#include "TestingMediaPlayerListener.h"

using namespace android;

TestingMediaPlayerListener::TestingMediaPlayerListener()
{
    clearFlags();
}

TestingMediaPlayerListener::~TestingMediaPlayerListener()
{
}

void TestingMediaPlayerListener::notify(int msg, int ext1, int ext2)
{
    LOGV("TestingMediaPlayerListener::notify, msg=%d, ext1=%d, ext2=%d", msg, ext1, ext2);
    switch(msg)
    {
        case MEDIA_PREPARED:
            mPrepared = true;
            break;
        case MEDIA_PLAYBACK_COMPLETE:
            mPlaybackComplete = true;
            break;
        case MEDIA_BUFFERING_UPDATE:
            break;
        case MEDIA_SEEK_COMPLETE:
            mSeekComplete = true;
            break;
        case MEDIA_SET_VIDEO_SIZE:
        case MEDIA_ERROR:
        case MEDIA_INFO:
        default:
            break;
    }
}

void TestingMediaPlayerListener::clearFlags()
{
    mPrepared = false;
    mSeekComplete = false;
    mPlaybackComplete = false;
}
