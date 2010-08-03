#define LOG_NDEBUG 0
#define LOG_TAG "MediaPlayerTest"
#include <utils/Log.h>

#include <utils/String8.h>
#include <media/mediaplayer.h>
#include "TestingMediaPlayerListener.h"
#include "CppUTest/TestHarness.h"

const char* SDCARD_LOCATION = "/mnt/sdcard/";
const char* WMA_TESTFILE = "No_Woman_No_Cry_128K.wma";

using namespace android;

TEST_GROUP(MediaPlayer)
{
    sp<MediaPlayer> mediaplayer;
    sp<TestingMediaPlayerListener> listener;

    void setup()
    {
        String8 fileName(SDCARD_LOCATION);
        fileName += WMA_TESTFILE;
        mediaplayer = new MediaPlayer;
        listener = new TestingMediaPlayerListener;
        mediaplayer->setListener(listener);
        mediaplayer->setDataSource(fileName.string(), NULL);
    }

    void teardown()
    {
        mediaplayer->reset();
        mediaplayer->setListener(NULL);
        mediaplayer->disconnect();
        mediaplayer.clear();
        listener.clear();
    }

    void testPlay()
    {
        status_t retCode = mediaplayer->prepare();
        CHECK(OK == retCode);
        retCode = mediaplayer->start();
        CHECK(OK == retCode);
    }
};

TEST(MediaPlayer, PrepareAsync)
{
    status_t retCode = mediaplayer->prepareAsync();
    CHECK(OK == retCode);
    for(int i=0; i<10; i++)
    {
        if(listener->isPrepared())
            return;
        usleep(200*1000);
    }
    FAIL("Prepare didn't complete after 2 seconds!");
}

TEST(MediaPlayer, play)
{
    const int PLAYBACK_MAXIMUM_EXTRA_TIME_IN_SECONDS = 5;
    testPlay();
    int durationInMsec = 0;
    status_t retCode = mediaplayer->getDuration(&durationInMsec);
    LOGV("Track duration: %d", durationInMsec);
    CHECK(OK == retCode);
    usleep(durationInMsec * 1000);
    sleep(PLAYBACK_MAXIMUM_EXTRA_TIME_IN_SECONDS);
    CHECK(true == listener->isPlaybackComplete());
}

TEST(MediaPlayer, pauseAndResume)
{
    const int PAUSE_POSITION = 10;
    const int TIME_INTERVAL = 3;
    testPlay();
    sleep(PAUSE_POSITION);
    status_t retCode = mediaplayer->pause();
    CHECK(OK == retCode);
    int positionInMsec = 0;
    retCode = mediaplayer->getCurrentPosition(&positionInMsec);
    CHECK(OK == retCode);
    LOGV("current position is %d after pause", positionInMsec);
    sleep(TIME_INTERVAL);
    int positionInMsec2 = 0;
    retCode = mediaplayer->getCurrentPosition(&positionInMsec2);
    LOGV("current position is %d after pause a while", positionInMsec2);
    CHECK(positionInMsec2 == positionInMsec);
    retCode = mediaplayer->start();
    CHECK(OK == retCode);
    sleep(TIME_INTERVAL);
    int positionAfterResumeInMsec = 0;
    retCode = mediaplayer->getCurrentPosition(&positionAfterResumeInMsec);
    LOGV("current position is %d after resume several seconds", positionAfterResumeInMsec);
    CHECK(positionAfterResumeInMsec > positionInMsec2 + (TIME_INTERVAL-1)*1000);
}

TEST(MediaPlayer, seek)
{
    const int SEEK_POSITION_IN_MSEC = 30*1000;
    const int SEEK_MAXIMUM_TOLERATION_IN_MSEC = 2*1000;
    testPlay();
    status_t retCode = mediaplayer->seekTo(SEEK_POSITION_IN_MSEC);
    CHECK(OK == retCode);
    for(int i=0; i<10; i++)
    {
        if(listener->isSeekComplete())
        {
            int positionInMsec = 0;
            retCode = mediaplayer->getCurrentPosition(&positionInMsec);
            LOGV("current position is %d after seeking to %d", positionInMsec, SEEK_POSITION_IN_MSEC);
            CHECK(OK == retCode);
            CHECK(positionInMsec + SEEK_MAXIMUM_TOLERATION_IN_MSEC > SEEK_POSITION_IN_MSEC);
            CHECK(positionInMsec < SEEK_MAXIMUM_TOLERATION_IN_MSEC + SEEK_POSITION_IN_MSEC);
            return;
        }
        usleep(200*1000);
    }
    FAIL("Seek didn't complete after 2 seconds!");
}
