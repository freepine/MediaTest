#define LOG_NDEBUG 0
#define LOG_TAG "MediaPlayerTest"
#include <utils/Log.h>

#include <utils/String8.h>
#include <media/mediaplayer.h>
#include "TestingMediaPlayerListener.h"
#include "CppUTest/TestHarness.h"

const char* SDCARD_LOCATION = "/mnt/sdcard/";
const char* WMA_TESTFILE = "Beethoven_2.wma";

const int MAXIMUM_RESUME_DELAY_IN_MSEC = 2*1000;

using namespace android;

TEST_GROUP(MediaPlayer)
{
    sp<MediaPlayer> mediaplayer;
    sp<TestingMediaPlayerListener> listener;
    int durationInMsec;

    void setup()
    {
        durationInMsec = 0;
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
        retCode = mediaplayer->getDuration(&durationInMsec);
        LOGV("Track duration: %d", durationInMsec);
        CHECK(OK == retCode);
        retCode = mediaplayer->start();
        CHECK(OK == retCode);
    }

    void testSeek(int seekPosition)
    {
        const int SEEK_MAXIMUM_TOLERATION_IN_MSEC = 2*1000;
        LOGV("seek to: %d", seekPosition);
        if(seekPosition > durationInMsec)
        {
            LOGW("Seek beyond clip duration, durationInMsec=%d", durationInMsec);
        }
        status_t retCode = mediaplayer->seekTo(seekPosition);
        CHECK(OK == retCode);
        for(int i=0; i<10; i++)
        {
            if(listener->isSeekComplete())
            {
                int positionInMsec = getPosition();
                LOGV("after seek, current position = %d", positionInMsec);
                CHECK(positionInMsec + SEEK_MAXIMUM_TOLERATION_IN_MSEC > seekPosition);
                CHECK(positionInMsec < SEEK_MAXIMUM_TOLERATION_IN_MSEC + seekPosition);
                return;
            }
            usleep(200*1000);
        }
        FAIL("Seek didn't complete after 2 seconds!");
    }

    int getPosition()
    {
        int positionInMsec = 0;
        status_t retCode = mediaplayer->getCurrentPosition(&positionInMsec);
        CHECK(OK == retCode);
        LOGV("current position is %d", positionInMsec);
        return positionInMsec;
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
    int positionInMsec = getPosition();
    sleep(TIME_INTERVAL);
    int positionInMsec2 = getPosition();
    CHECK(positionInMsec2 == positionInMsec);
    retCode = mediaplayer->start();
    CHECK(OK == retCode);
    sleep(TIME_INTERVAL);
    int positionAfterResumeInMsec = getPosition();
    CHECK(positionAfterResumeInMsec > positionInMsec2 + TIME_INTERVAL*1000 - MAXIMUM_RESUME_DELAY_IN_MSEC);
}

TEST(MediaPlayer, seek)
{
    const int SEEK_POSITION_IN_MSEC = 30*1000;
    const int TIME_IN_MSEC_BEFORE_EOS = 1*1000;
    const int TIME_INTERVAL = 3;
    testPlay();
    testSeek(SEEK_POSITION_IN_MSEC);
    mediaplayer->pause();
    int seekPosition = durationInMsec/2;
    testSeek(seekPosition);
    mediaplayer->start();
    sleep(TIME_INTERVAL);
    int positionAfterResumeInMsec = getPosition();
    CHECK(positionAfterResumeInMsec > seekPosition + TIME_INTERVAL*1000 - MAXIMUM_RESUME_DELAY_IN_MSEC);
    seekPosition = durationInMsec - TIME_IN_MSEC_BEFORE_EOS;
    testSeek(seekPosition);
    sleep((TIME_IN_MSEC_BEFORE_EOS + MAXIMUM_RESUME_DELAY_IN_MSEC)/1000);
    CHECK(true == listener->isPlaybackComplete());
}
