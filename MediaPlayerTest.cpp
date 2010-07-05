#include "TestHarness.h"
#include <utils/String8.h>
#include <media/mediaplayer.h>

const char* SDCARD_LOCATION = "/mnt/sdcard/";
const char* WMA_TESTFILE = "No_Woman_No_Cry_128K.wma";

using namespace android;

TEST( MediaPlayer, playWMA)
{
    String8 fileName(SDCARD_LOCATION);
    fileName += WMA_TESTFILE;
    sp<MediaPlayer> mediaplayer = new MediaPlayer;
    mediaplayer->setDataSource(fileName.string(), NULL);
    status_t retCode = mediaplayer->prepare();
    CHECK(OK == retCode);
    retCode = mediaplayer->start();
    CHECK(OK == retCode);
    mediaplayer.clear();
}
