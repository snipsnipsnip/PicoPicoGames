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

#ifndef __QBSOUND_H__
#define __QBSOUND_H__

//#define QBSOUND_OGG

#if TARGET_OS_IPHONE
typedef long long ogg_int64_t;
#else
#ifdef QBSOUND_OGG
#include "vorbis/vorbisfile.h"
#else
typedef long long ogg_int64_t;
#endif
#endif

//#define QBSOUND_STREAM_BUFFER_SIZE (64*512)
#define QBSOUND_STREAM_BUFFER_SIZE (64*836*4)
//#define QBSOUND_STREAM_BUFFER_SIZE (64*1024*4)

typedef struct _PSGTrack {
	float w;
	float f;
	float noise;
	signed long lastlevel;
	int v;
	float fv;
	
	char *note;
	char *noteptr;
	char *ptr;
	char *loopptr;

	unsigned long notect;
	int oct;
	int end;
	int type;
	
	int nlen;
	int tempo;

	signed short* pcmbuffer;
	unsigned long pcmptr;
	unsigned long pcmsize;
	int pcmchannel;
	bool pcmplay;
	bool pcmloop;
	
	int fadeout;
	int fadecount;
	int fademax;
	
	char streamfile[1024];
	bool streamopen;
	bool streamopenov;
	signed short streambuffer[QBSOUND_STREAM_BUFFER_SIZE];
	int streamp1;
	int streamp2;
	int streampoint;
	int streamchannel;
	bool streamplay;
	bool streamplayrun;
	bool streamdone;
	bool streamloop;
	unsigned char* streamdata;
	unsigned long streamsize;
	unsigned long streamptr;
#ifdef QBSOUND_OGG
	ogg_int64_t looppoint;
#endif
#if TARGET_OS_IPHONE
#else
#ifdef QBSOUND_OGG
	OggVorbis_File streamovf;
#endif
#endif
} PSGTrack;

typedef struct _QBPCM {
	signed short* pcmbuffer;
	unsigned long pcmsize;
	int pcmchannel;
} QBPCM;

class QBSound {
public:
	QBSound(int maxChannel);
	virtual ~QBSound() {};
	
	virtual int Init();
	virtual int Exit();
	virtual int Reset();
	virtual int play(const char* mml,int track=0,bool loop=false);
	virtual int playpcm(signed short* dataPtr,unsigned long dataSize,int dataChannel,int track,bool loop=false);
	virtual int playpcm(const char* filename,int track,bool loop=false,ogg_int64_t looppoint=0);
	virtual int playmempcm(unsigned char* data,unsigned long size,int track,bool loop=false,ogg_int64_t looppoint=0);
	virtual int stop(int track);
	virtual int isPlaying(int channel=0);
	virtual int fadeOut(int track,int speed);
	virtual int streamOpen(int track);
	virtual int streamLoad(int track,int p1,int p2,int* done);
	virtual int setVolume(int track,float volume);
	virtual float getVolume(int track);
	virtual int setMasterVolume(float volume);
	virtual float getMasterVolume();
	
	virtual int sysPause();
	virtual int sysPlay();
	
	virtual int p1(int track);
	virtual int p2(int track);
	virtual ogg_int64_t streampoint(int track);

	PSGTrack* mTrack;
	int mMaxTrack;
	
	float mMasterVolume;
	float mPause;

	int init_track(int maxChannel);
	virtual int fill_sound_buffer(void* buffer,int size);
	void analyze_note(PSGTrack *track);
	signed long get_level(PSGTrack *track);
	int GetNum(PSGTrack *track);

	static int oggMasterVolume;
};

#ifdef __cplusplus
extern "C" {
#endif

QBSound* QBSound_Instance();
int QBSound_Reset();
int QBSound_Start(int maxTrack);
int QBSound_Exit();
int QBSound_Play(const char* mml,int track);
int QBSound_LoopPlay(const char* mml,int track);
int QBSound_PlayPCM(QBPCM* pcm,int track);
int QBSound_LoopPlayPCM(QBPCM* pcm,int track);
int QBSound_StreamPlayPCM(const char* filename,int track);
int QBSound_LoopStreamPlayPCM(const char* filename,int track,ogg_int64_t looppoint=0);
int QBSound_StreamMemPlay(unsigned char* data,unsigned long size,int track);
int QBSound_LoopStreamMemPlay(unsigned char* data,unsigned long size,int track,ogg_int64_t looppoint=0);
int QBSound_Stop(int track);
int QBSound_IsPlaying(int track);
int QBSound_PreloadWave(unsigned char* data,unsigned long size,QBPCM* pcm,float volume=1.0);
int QBSound_Preload(const char* fname,QBPCM* pcm,float volume=1.0);
int QBSound_DeletePCM(QBPCM* pcm);
int QBSound_FadeOut(int track,int speed);
int QBSound_SetVolume(int track,float volume=1.0);
float QBSound_GetVolume(int track);
int QBSound_SetMasterVolume(float volume=1.0);
float QBSound_GetMasterVolume();

void H_InitMovie(void);
void H_MovieIdle(void);
void H_OpenMovieFile(const char *fname,bool loop=true);

#if TARGET_OS_IPHONE
int QBSound_InitACC();
int QBSound_LoopStreamPlayAAC(const char* filename,int looppoint=0);
int QBSound_StreamPlayAAC(const char* filename);
int QBSound_SetVolumeAAC(float volume=1.0);
float QBSound_GetVolumeAAC();
int QBSound_SetMasterVolumeAAC(float volume=1.0);
float QBSound_GetMasterVolumeAAC();
int QBSound_IsPlayingAAC();
int QBSound_StopAAC();
int QBSound_PauseAAC();
int QBSound_ResumeAAC();
int QBSound_SetCategory(const char* category);
#endif

#ifdef _WIN32
#include <windows.h>
void H_MovieIdleWin(WPARAM wParam, LPARAM lParam);
void H_CloseMovie(void);
#endif

#ifdef __cplusplus
};
#endif

#endif //__QBSOUND_H__
