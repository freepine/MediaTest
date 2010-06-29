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

#include <media/mediaplayer.h>
#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>
#include <surfaceflinger/SurfaceComposerClient.h>
#include <surfaceflinger/Surface.h>
#include <surfaceflinger/ISurfaceComposer.h>

using namespace android;

int main(int argc, char** argv)
{
    LOGI("entering main..."); 
    if(argc <= 1)
    {
        LOGI("Please specify the test content: ./mediatest <content_url>");
        return 1;
    }

    sp<ProcessState> proc = ProcessState::self();
    proc->startThreadPool();
    sp<MediaPlayer> mediaplayer = new MediaPlayer;

    LOGI("set datasource: %s", argv[1]);
    mediaplayer->setDataSource(argv[1], NULL);
    LOGI("create SurfaceComposerClient");
    int pid = getpid();
    int nState = 0;
    sp<SurfaceComposerClient> videoClient = new SurfaceComposerClient;

    LOGI("create video surface");
    sp<SurfaceControl> videoSurface(videoClient->createSurface(pid, 0, 176, 144, PIXEL_FORMAT_OPAQUE, ISurfaceComposer::eFXSurfaceNormal|ISurfaceComposer::ePushBuffers));
    videoClient->openTransaction();
    nState = videoSurface->setLayer(INT_MAX);
    LOGI("videosurface->setLayer, %d", nState);
    nState = videoSurface->show();
    LOGI("videosurface->show, %d", nState);
    videoClient->closeTransaction();

    LOGI("set video surface to player");
    mediaplayer->setVideoSurface(videoSurface->getSurface());

    LOGI("prepare...");
    status_t retCode = mediaplayer->prepare();
    if(retCode < 0)
    {
        LOGE("prepare failed: %d\n", retCode);
        IPCThreadState::self()->stopProcess();
        return -1;
    };

    LOGI("start playback.");
    mediaplayer->start();
    for(int i=0; i<10; i++)
    {
        sleep(1);
        LOGI("playing, i=%d\n", i);
    }
    LOGI("release player.");
    mediaplayer.clear();
    LOGI("quiting...");
    IPCThreadState::self()->stopProcess();
}

