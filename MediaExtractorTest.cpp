#define LOG_NDEBUG 0
#define LOG_TAG "MediaExtractorTest"
#include <utils/Log.h>

#include <utils/String8.h>
#include <media/stagefright/DataSource.h>
#include <media/stagefright/MediaSource.h>
#include <media/stagefright/MetaData.h>
#include <media/stagefright/MediaExtractor.h>
#include <media/stagefright/MediaBuffer.h>
#include <media/stagefright/MediaErrors.h>
#include "CppUTest/TestHarness.h"

using namespace android;

typedef struct _MediaAttribute
{
    const char* sFileName;
    uint16_t nTrackNum;
    const char* sMimeType;
} MediaAttribute;

MediaAttribute testMatrix[] = {
    {"/mnt/sdcard/media_api/album_photo/Beethoven_2.wma", 1, "audio/x-ms-wma"},
    {"/mnt/sdcard/media_api/video/bugs.wmv", 2, "video/x-ms-wmv"},
    {"/mnt/sdcard/media_api/video/flyby_xvid_default.avi", 2, "video/x-msvideo"},
    {"/mnt/sdcard/media_api/video/flyby_xvid_video.avi", 1, "video/x-msvideo"},
    {"/mnt/sdcard/avi/flyby-xvid-default.avi", 2, "video/x-msvideo"},
    {"/mnt/sdcard/media_api/video/H264_320_AAC_64.3gp", 2, "video/mp4"},
    {"/mnt/sdcard/bugs_230_piff4.ismv", 2, "video/mp4"},
};

const size_t TEST_FILE_NUM = sizeof(testMatrix) / sizeof(MediaAttribute);

TEST_GROUP(MediaExtractor)
{
    sp<MediaExtractor> mExtractor;

    void setup()
    {
        DataSource::RegisterDefaultSniffers();
    }

    void teardown()
    {
    }

    void testFile(uint32_t file_index)
    {
        LOGV("Testing %s", testMatrix[file_index].sFileName);
        sp<DataSource> dataSource = DataSource::CreateFromURI(testMatrix[file_index].sFileName, NULL);
        CHECK(dataSource.get() != NULL);
        mExtractor = MediaExtractor::Create(dataSource);
        CHECK(mExtractor.get() != NULL);
        testMetaData(testMatrix[file_index].nTrackNum, testMatrix[file_index].sMimeType);
        testTrackFormat(testMatrix[file_index].nTrackNum);
        testTracks(testMatrix[file_index].nTrackNum);
        mExtractor.clear();
    }

    void testMetaData(uint16_t trackNum_expected, const char* mime_expected)
    {
        CHECK(mExtractor->countTracks() == trackNum_expected);
        sp<MetaData> metaData = mExtractor->getMetaData();
        const char *mime;
        bool ret = metaData->findCString(kKeyMIMEType, &mime);
        LOGV("mime=%s", mime);
        CHECK(ret);
        STRCMP_EQUAL(mime_expected, mime);
        metaData.clear();
    }

    void testTrackFormat(uint16_t trackCount)
    {
        LOGV("testTrackFormat, trackCount=%d", trackCount);
        for(uint32_t i=0; i<trackCount; i++)
        {
            LOGV("Get track %d format info.", i);
            sp<MetaData> metaData = mExtractor->getTrackMetaData(i, 0);
            const char *mime;
            CHECK(metaData->findCString(kKeyMIMEType, &mime));
            if(strncmp(mime, "audio/", strlen("audio/")) == 0)
            {
                LOGV("Audio track");
                int32_t sampleRate;
                CHECK(metaData->findInt32(kKeySampleRate, &sampleRate));
                CHECK(sampleRate > 0);
                int32_t channelCount;
                CHECK(metaData->findInt32(kKeyChannelCount, &channelCount));
                CHECK(channelCount > 0);
                LOGV("audio: %s, sampleRate=%d, channelCount=%d", mime, sampleRate, channelCount);
            }
            else if(strncmp(mime, "video/", strlen("video/")) == 0)
            {
                LOGV("Video track");
                int32_t height;
                CHECK(metaData->findInt32(kKeyHeight, &height));
                CHECK(height > 0);
                int32_t width;
                CHECK(metaData->findInt32(kKeyWidth, &width));
                CHECK(width > 0);
                LOGV("video: %s, height=%d, width=%d", mime, height, width);
            }
        }
    }

    void testTracks(uint16_t trackCount)
    {
        LOGV("testTracks, trackCount=%d", trackCount);
        for(uint32_t i=0; i<trackCount; i++)
        {
            LOGV("Get track %d.", i);
            sp<MediaSource> track = mExtractor->getTrack(i);
            CHECK(track.get() != NULL);
            testMediaSource(track);
        }
    }

    void testMediaSource(sp<MediaSource> track)
    {
        sp<MetaData> metaData = track->getFormat();
        int64_t duration;
        CHECK(metaData->findInt64(kKeyDuration, &duration));
        LOGV("track duration = %lld", duration);
        CHECK(track->start() == OK);
        testReadToEnd(track, duration);
        LOGV("seeking...");
        testMediaSourceSeek(track, (uint64_t)5*1000*1000);
        testMediaSourceSeek(track, duration/2);
        testMediaSourceSeek(track, duration - 1*1000*1000);
        testReadToEnd(track, duration);
    }

    void testReadToEnd(sp<MediaSource> track, int64_t duration)
    {
        MediaBuffer* out;
        status_t ret;
        while((ret = track->read(&out)) == OK)
        {
            int64_t timestamp;
            CHECK(out->meta_data()->findInt64(kKeyTime, &timestamp));
            CHECK(timestamp <= duration);
            out->release();
        }
        CHECK(ret == ERROR_END_OF_STREAM);
    }

    void testMediaSourceSeek(sp<MediaSource> track, int64_t seekTime)
    {
        LOGV("seek to %lld", seekTime);
        MediaSource::ReadOptions options;
        options.setSeekTo(seekTime);
        MediaBuffer* out;
        status_t ret = track->read(&out, &options);
        CHECK(ret == OK);
        int64_t timestamp;
        CHECK(out->meta_data()->findInt64(kKeyTime, &timestamp));
        LOGV("timestamp = %lld after seeking to %lld", timestamp, seekTime);
        out->release();
    }

};

TEST(MediaExtractor, WMA)
{
    testFile(0);
}

TEST(MediaExtractor, WMV)
{
    testFile(1);
}

TEST(MediaExtractor, AVI)
{
    testFile(2);
}

TEST(MediaExtractor, AVI2)
{
    testFile(3);
}

TEST(MediaExtractor, AVI3)
{
    testFile(4);
}

TEST(MediaExtractor, MP4)
{
    testFile(5);
}

TEST(MediaExtractor, fMP4)
{
    testFile(6);
}

