/*
  PicoPicoGames

  Copyright (c) 2009, Hiromitsu Yamaguchi, All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are
  met:

  1. Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer. 

  2. Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in the
  documentation and/or other materials provided with the distribution. 

  3. Neither the name of the Yamagame nor the names of its contributors
  may be used to endorse or promote products derived from this software
  without specific prior written permission. 

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "QBSoundMac.h"
#include <pthread.h>
#include <AudioToolbox/AudioToolbox.h>
#include <AudioUnit/AUComponent.h>
#include <unistd.h>
#include <sys/time.h>

static OSStatus callback(
	void *inRefCon,
	AudioUnitRenderActionFlags* ioActionFlags,
	const AudioTimeStamp* inTimeStamp,
	UInt32 inBusNumber,
	UInt32 inNumberFrames,
	AudioBufferList* ioData)
{
	QBSoundMac* snd = (QBSoundMac*)inRefCon;
	if (snd) {
		UInt32 bytesToRead;
		bytesToRead = ioData->mBuffers[0].mDataByteSize;
		snd->fill_sound_buffer(ioData->mBuffers[0].mData,bytesToRead);
	}
	return 0;
}


static void InterruptionListener(void *inUserData,
                          UInt32 inInterruption)
{
/*
    AudioUnit *remoteIO = (AudioUnit*)inUserData;
	
    if (inInterruption == kAudioSessionEndInterruption) {
        AudioSessionSetActive(true);
        AudioOutputUnitStart(*remoteIO);
        printf("EndInterruption\n");
    }
    
    if (inInterruption == kAudioSessionBeginInterruption) {
        AudioOutputUnitStop(*remoteIO);
        printf("BeginInterruption\n");
    }
*/
}

int QBSound_SetCategory(const char* category)
{
#if TARGET_OS_IPHONE
	int type = 0;
	if (category == NULL) {
	} else
	if (strcmp(category,"ambient") == 0) {
		type = 1;
	}
	switch (type) {
	case 0:
		{
			AudioSessionInitialize(NULL, NULL, InterruptionListener, NULL);
			
			UInt32 category = kAudioSessionCategory_SoloAmbientSound;
			AudioSessionSetProperty(kAudioSessionProperty_AudioCategory, sizeof(category), &category);
			
			AudioSessionSetActive(true);
		}
		break;
	case 1:
		{
			AudioSessionInitialize(NULL, NULL, InterruptionListener, NULL);
			
			UInt32 category = kAudioSessionCategory_AmbientSound;
			AudioSessionSetProperty(kAudioSessionProperty_AudioCategory, sizeof(category), &category);
			
			AudioSessionSetActive(true);
		}
		break;
	}
#endif
	return 0;
}

QBSoundMac::QBSoundMac(int maxChannel) : mGraph(NULL),QBSound(maxChannel)
{
}

static void *LoaderThreadProc(void *arg)
{
	QBSoundMac* snd = (QBSoundMac*)arg;
	bool r=false;
	while (!r) {
		if (snd->streamLoad() != 0) {
			break;
		}
		pthread_mutex_lock(&snd->mMutex);
		r = snd->mThreadEnd;
		pthread_mutex_unlock(&snd->mMutex);
		usleep(1000);
	}
	return NULL;
}

int QBSoundMac::Exit()
{
	pthread_mutex_lock(&mMutex);
	mThreadEnd = true;
	pthread_mutex_unlock(&mMutex);
	pthread_join(mLoaderThread,NULL);
	return 0;
}

int QBSoundMac::Reset()
{
	int r=0;
	pthread_mutex_lock(&mMutex);
	r = QBSound::Reset();
	pthread_mutex_unlock(&mMutex);
	return r;
}

int QBSoundMac::Init()
{
	OSStatus err;
	
	QBSound::Init();

	pthread_mutex_init(&mMutex,NULL);

	mAudioFormat.mSampleRate = 44100.000000;
	mAudioFormat.mFormatID = kAudioFormatLinearPCM;
    mAudioFormat.mFormatFlags = 0
		| kLinearPCMFormatFlagIsPacked
        | kLinearPCMFormatFlagIsSignedInteger 
#if __BIG_ENDIAN__
        | kLinearPCMFormatFlagIsBigEndian
#endif
        ;
	mAudioFormat.mBytesPerPacket = 4;
	mAudioFormat.mFramesPerPacket = 1;
	mAudioFormat.mBytesPerFrame = 4;
	mAudioFormat.mChannelsPerFrame = 2;
	mAudioFormat.mBitsPerChannel = 16;
	mAudioFormat.mReserved = 0;

	err = NewAUGraph(&mGraph);

#if !TARGET_OS_IPHONE
    ComponentDescription description;
    description.componentSubType = kAudioUnitSubType_DefaultOutput;
#else
	AudioComponentDescription description;
	description.componentSubType = kAudioUnitSubType_RemoteIO;
#endif
    description.componentType = kAudioUnitType_Output;
    description.componentManufacturer = kAudioUnitManufacturer_Apple;
    description.componentFlags = 0;
    description.componentFlagsMask = 0;
#if 1
	err = AUGraphAddNode(mGraph,&description,&mOutputNode);
#else
	err = AUGraphNewNode(mGraph,&description, 0, NULL, &mOutputNode);
#endif

	err = AUGraphOpen(mGraph);

#if 1
	err = AUGraphNodeInfo(mGraph,mOutputNode,NULL,&mAudioUnit);
#else
	err = AUGraphGetNodeInfo(mGraph,mOutputNode,NULL,NULL,NULL,&mAudioUnit);
#endif

   err =  AudioUnitSetProperty(mAudioUnit,
                                  kAudioUnitProperty_StreamFormat,
                                  kAudioUnitScope_Input,
                                  0,
                                  &mAudioFormat,
                                  sizeof(AudioStreamBasicDescription));

	{
		AURenderCallbackStruct input;
		input.inputProc = callback;
		input.inputProcRefCon = this;
	err = 	AudioUnitSetProperty(mAudioUnit,
									  kAudioUnitProperty_SetRenderCallback,
									  kAudioUnitScope_Input,
									  0,
									  &input, sizeof(input));
	}

	err = AUGraphInitialize(mGraph);
	err = AUGraphStart(mGraph);
	
	//pthread_mutex_init(&mLoaderThreadMutex,NULL);
#ifndef _IPHONE
	mThreadEnd = false;
	pthread_create(&mLoaderThread,NULL,LoaderThreadProc,this);
#endif

	return err;
}

int QBSoundMac::play(const char *note,int track,bool loop)
{
	OSStatus err;
	int r=0;
	Boolean run;
	if (mGraph) {
		err = AUGraphIsRunning(mGraph,&run);
		if (err == noErr) {
			if (run) {
				pthread_mutex_lock(&mMutex);
				r = QBSound::play(note,track,loop);
				pthread_mutex_unlock(&mMutex);
			}
		}
	}
	return r;
}

int QBSoundMac::playpcm(signed short* dataPtr,unsigned long dataSize,int dataChannel,int track,bool loop)
{
	int r=0;
	pthread_mutex_lock(&mMutex);
	r = QBSound::playpcm(dataPtr,dataSize,dataChannel,track,loop);
	pthread_mutex_unlock(&mMutex);
	return r;
}

int QBSoundMac::playpcm(const char* filename,int track,bool loop,ogg_int64_t looppoint)
{
	int r=0;
	pthread_mutex_lock(&mMutex);
	r = QBSound::playpcm(filename,track,loop,looppoint);
	pthread_mutex_unlock(&mMutex);
	return r;
}

int QBSoundMac::playmempcm(unsigned char* data,unsigned long size,int track,bool loop,ogg_int64_t looppoint)
{
	int r=0;
	pthread_mutex_lock(&mMutex);
	r = QBSound::playmempcm(data,size,track,loop,looppoint);
	pthread_mutex_unlock(&mMutex);
	return r;
}

int QBSoundMac::fill_sound_buffer(void* buffer,int size)
{
	int r;
	pthread_mutex_lock(&mMutex);
	r = QBSound::fill_sound_buffer(buffer,size);
	pthread_mutex_unlock(&mMutex);
	return r;
}

int QBSoundMac::stop(int track)
{
	int r=0;
	pthread_mutex_lock(&mMutex);
	r = QBSound::stop(track);
	pthread_mutex_unlock(&mMutex);
	return r;
}

int QBSoundMac::isPlaying(int track)
{
	int r=0;
	pthread_mutex_lock(&mMutex);
	r = QBSound::isPlaying(track);
	pthread_mutex_unlock(&mMutex);
	return r;
}

int QBSoundMac::fadeOut(int track,int speed)
{
	int r=0;
	pthread_mutex_lock(&mMutex);
	r = QBSound::fadeOut(track,speed);
	pthread_mutex_unlock(&mMutex);
	return r;
}

int QBSoundMac::p1(int track)
{
	int r=0;
	pthread_mutex_lock(&mMutex);
	r = QBSound::p1(track);
	pthread_mutex_unlock(&mMutex);
	return r;
}

int QBSoundMac::p2(int track)
{
	int r=0;
	pthread_mutex_lock(&mMutex);
	r = QBSound::p2(track);
	pthread_mutex_unlock(&mMutex);
	return r;
}

int QBSoundMac::streamLoad()
{
	int i;
	for (i=0;i<mMaxTrack;i++) {
		pthread_mutex_lock(&mMutex);
		QBSound::streamOpen(i);
		if (mTrack[i].streamplay && mTrack[i].streamdone==false) {
			int p1 = mTrack[i].streamp1;
			int p2 = mTrack[i].streamp2;
			int done=0;
			pthread_mutex_unlock(&mMutex);
			p2 = QBSound::streamLoad(i,p1,p2,&done);
			pthread_mutex_lock(&mMutex);
			if (done) {
				if (!mTrack[i].streamloop) {
					mTrack[i].streamdone = true;
				}
			}
			mTrack[i].streamp2 = p2;
			mTrack[i].streamplayrun = true;
			pthread_mutex_unlock(&mMutex);
		} else {
			pthread_mutex_unlock(&mMutex);
		}
	}
	return 0;
}

int QBSoundMac::setVolume(int track,float volume)
{
	int r=0;
	pthread_mutex_lock(&mMutex);
	r = QBSound::setVolume(track,volume);
	pthread_mutex_unlock(&mMutex);
	return r;
}

float QBSoundMac::getVolume(int track)
{
	float r=0;
	pthread_mutex_lock(&mMutex);
	r = QBSound::getVolume(track);
	pthread_mutex_unlock(&mMutex);
	return r;
}

int QBSoundMac::sysPause()
{
#if 0
	OSStatus err;
	Boolean run;
	if (mGraph) {
		err = AUGraphIsRunning(mGraph,&run);
		if (err == noErr) {
			if (run) {
				//err = AUGraphStop(mGraph);
				err = AUGraphClose(mGraph);
				mGraph = NULL;
printf("sysPause\n");
			}
		}
	}
#else
	mPause = 0.0;
#endif
	return 0;
}

int QBSoundMac::sysPlay()
{
#if 0
	OSStatus err;
	Boolean run;
	if (mGraph) {
		err = AUGraphIsRunning(mGraph,&run);
		if (err == noErr) {
			if (!run) {
				err = AUGraphStart(mGraph);
printf("sysPlay\n");
			}
		}
	} else {
	mAudioFormat.mSampleRate = 44100.000000;
	mAudioFormat.mFormatID = kAudioFormatLinearPCM;
    mAudioFormat.mFormatFlags = 0
		| kLinearPCMFormatFlagIsPacked
        | kLinearPCMFormatFlagIsSignedInteger 
#if __BIG_ENDIAN__
        | kLinearPCMFormatFlagIsBigEndian
#endif
        ;
	mAudioFormat.mBytesPerPacket = 4;
	mAudioFormat.mFramesPerPacket = 1;
	mAudioFormat.mBytesPerFrame = 4;
	mAudioFormat.mChannelsPerFrame = 2;
	mAudioFormat.mBitsPerChannel = 16;
	mAudioFormat.mReserved = 0;

	err = NewAUGraph(&mGraph);

#if !TARGET_OS_IPHONE
    ComponentDescription description;
    description.componentSubType = kAudioUnitSubType_DefaultOutput;
#else
	AudioComponentDescription description;
	description.componentSubType = kAudioUnitSubType_RemoteIO;
#endif
    description.componentType = kAudioUnitType_Output;
    description.componentManufacturer = kAudioUnitManufacturer_Apple;
    description.componentFlags = 0;
    description.componentFlagsMask = 0;
#if 1
	err = AUGraphAddNode(mGraph,&description,&mOutputNode);
#else
	err = AUGraphNewNode(mGraph,&description, 0, NULL, &mOutputNode);
#endif

	err = AUGraphOpen(mGraph);

#if 1
	err = AUGraphNodeInfo(mGraph,mOutputNode,NULL,&mAudioUnit);
#else
	err = AUGraphGetNodeInfo(mGraph,mOutputNode,NULL,NULL,NULL,&mAudioUnit);
#endif

   err =  AudioUnitSetProperty(mAudioUnit,
                                  kAudioUnitProperty_StreamFormat,
                                  kAudioUnitScope_Input,
                                  0,
                                  &mAudioFormat,
                                  sizeof(AudioStreamBasicDescription));

	{
		AURenderCallbackStruct input;
		input.inputProc = callback;
		input.inputProcRefCon = this;
	err = 	AudioUnitSetProperty(mAudioUnit,
									  kAudioUnitProperty_SetRenderCallback,
									  kAudioUnitScope_Input,
									  0,
									  &input, sizeof(input));
	}

	err = AUGraphInitialize(mGraph);
	err = AUGraphStart(mGraph);
	}
#else
	mPause = 1.0;
#endif

	return 0;
}

int QBSoundMac::setMasterVolume(float volume)
{
	int r;
	pthread_mutex_lock(&mMutex);
	r = QBSound::setMasterVolume(volume);
	pthread_mutex_unlock(&mMutex);
	return r;
}

float QBSoundMac::getMasterVolume()
{
	float v;
	pthread_mutex_lock(&mMutex);
	v = QBSound::getMasterVolume();
	pthread_mutex_unlock(&mMutex);
	return v;
}
