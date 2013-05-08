/*
**
** Copyright 2010, freepine@gmail.com
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/

// System headers required for setgroups, etc.

#define LOG_NDEBUG 0
#define LOG_TAG "Mediatest"
#include <utils/Log.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>

#include <media/mediaplayer.h>
#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>
#include <gui/SurfaceComposerClient.h>
#include <gui/Surface.h>
#include <gui/ISurfaceComposer.h>

using namespace android;

int main(int argc, char** argv)
{
    ALOGI("entering main..."); 
    if(argc <= 1)
    {
        ALOGI("Please specify the test content: ./mediatest <content_url>");
        return 1;
    }

    sp<ProcessState> proc = ProcessState::self();
    proc->startThreadPool();
    sp<MediaPlayer> mediaplayer = new MediaPlayer;

    ALOGI("set datasource: %s", argv[1]);
    int inFd = open(argv[1], O_RDONLY);
    mediaplayer->setDataSource(inFd, 0, 0x7FFFFFFF);
    ALOGI("create SurfaceComposerClient");
    int pid = getpid();
    int nState = 0;
    sp<SurfaceComposerClient> videoClient = new SurfaceComposerClient;

    ALOGI("create video surface");
    sp<SurfaceControl> videoSurface(videoClient->createSurface(String8("video"),
            480, 320,
            PIXEL_FORMAT_OPAQUE));
    videoClient->openGlobalTransaction();
    nState = videoSurface->setLayer(INT_MAX);
    ALOGI("videosurface->setLayer, %d", nState);
    nState = videoSurface->show();
    ALOGI("videosurface->show, %d", nState);
    videoClient->closeGlobalTransaction();

    ALOGI("set video surface to player");
    mediaplayer->setVideoSurfaceTexture(videoSurface->getSurface()->getISurfaceTexture());

    ALOGI("prepare...");
    status_t retCode = mediaplayer->prepare();
    if(retCode < 0)
    {
        ALOGE("prepare failed: %d\n", retCode);
        IPCThreadState::self()->stopProcess();
        return -1;
    };

    ALOGI("start playback.");
    mediaplayer->start();
    sleep(70);
    ALOGI("release player.");
    mediaplayer.clear();
    ALOGI("quiting...");
    close(inFd);
    IPCThreadState::self()->stopProcess();
}

