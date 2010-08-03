#ifndef TESTING_MEDIAPLAYERLISTENER_H
#define TESTING_MEDIAPLAYERLISTENER_H

#include <media/mediaplayer.h>

class TestingMediaPlayerListener: public android::MediaPlayerListener
{
public:
    TestingMediaPlayerListener();
    ~TestingMediaPlayerListener();

    virtual void notify(int msg, int ext1, int ext2);

    inline bool isPrepared() const {return mPrepared; }
    inline bool isSeekComplete() const {return mSeekComplete; }
    inline bool isPlaybackComplete() const {return mPlaybackComplete; }

    void clearFlags();

private:
    bool mPrepared;
    bool mSeekComplete;
    bool mPlaybackComplete;
};

#endif
