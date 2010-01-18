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

#ifndef __QBSOUNDMAC_H__
#define __QBSOUNDMAC_H__

#include <pthread.h>
#include <AudioToolbox/AudioToolbox.h>
#include <AudioUnit/AUComponent.h>
#include "QBSound.h"

class QBSoundMac : public QBSound {
public:
	QBSoundMac(int maxChannel);
	virtual ~QBSoundMac() {
		if (mGraph) AUGraphStop(mGraph);
	};

	virtual int Init();
	virtual int Exit();
	virtual int Reset();
	virtual int play(const char* mml,int channel=0,bool loop=false);
	virtual int playpcm(signed short* dataPtr,unsigned long dataSize,int dataChannel,int track,bool loop=false);
	virtual int playpcm(const char* filename,int track,bool loop=false,ogg_int64_t looppoint=0);
	virtual int playmempcm(unsigned char* data,unsigned long size,int track,bool loop=false,ogg_int64_t looppoint=0);
	virtual int stop(int trackindex);
	virtual int fill_sound_buffer(void* buffer,int size);
	virtual int isPlaying(int channel=0);
	virtual int fadeOut(int track,int speed);
	virtual int streamLoad();
	virtual int setVolume(int track,float volume=1.0);
	virtual float getVolume(int track);
	virtual int setMasterVolume(float volume);
	virtual float getMasterVolume();

	virtual int sysPause();
	virtual int sysPlay();
	
	virtual int p1(int track);
	virtual int p2(int track);

	AudioUnit mAudioUnit;
	AudioStreamBasicDescription mAudioFormat;
	AUNode mOutputNode;
	AUGraph mGraph;
	
	pthread_mutex_t mMutex;
	
	pthread_t mLoaderThread;
	
	bool mThreadEnd;
};

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
};
#endif

#endif //__QBSOUNDMAC_H__
