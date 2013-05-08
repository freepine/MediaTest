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
#define LOG_TAG "MediaThumbnailTest"
#include <utils/Log.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>

#include <media/mediametadataretriever.h>
#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>

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
    sp<MediaMetadataRetriever> retriever = new MediaMetadataRetriever;

    ALOGI("set datasource: %s", argv[1]);
    retriever->setDataSource(argv[1], NULL);
    ALOGI("get frame.");
    retriever->getFrameAtTime(-1, 0);
    ALOGI("done.");
    retriever.clear();
    ALOGI("quiting...");
    IPCThreadState::self()->stopProcess();
}

