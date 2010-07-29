#define LOG_NDEBUG 0
#define LOG_TAG "MediaPlayerTest"
#include <utils/Log.h>

#include <utils/String8.h>
#include <media/mediaplayer.h>
#include "CppUTest/TestHarness.h"

const char* SDCARD_LOCATION = "/mnt/sdcard/";
const char* WMA_TESTFILE = "No_Woman_No_Cry_128K.wma";

using namespace android;

TEST_GROUP(MediaPlayer)
{
    sp<MediaPlayer> mediaplayer;

    void setup()
    {
        String8 fileName(SDCARD_LOCATION);
        fileName += WMA_TESTFILE;
        mediaplayer = new MediaPlayer;
        mediaplayer->setDataSource(fileName.string(), NULL);
    }

    void teardown()
    {
        mediaplayer->reset();
        mediaplayer->disconnect();
        mediaplayer.clear();
    }

    void testPlay()
    {
        status_t retCode = mediaplayer->prepare();
        CHECK(OK == retCode);
        retCode = mediaplayer->start();
        CHECK(OK == retCode);
    }
};

TEST(MediaPlayer, play)
{
    testPlay();
}

TEST(MediaPlayer, seek)
{
    testPlay();
    status_t retCode = mediaplayer->seekTo(30*1000);
    CHECK(OK == retCode);
}
