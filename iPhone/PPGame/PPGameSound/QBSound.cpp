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

#ifdef _WIN32
#else
#include <AudioToolbox/AudioToolbox.h>
#include <AudioUnit/AUComponent.h>
#endif
#ifndef _IPHONE
#define _IPHONE
#endif
#include "QBSound.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define _IPHONE_ACC

#define S_PI        3.14159265358979323846264338327950288f   /* pi */

#ifdef _WIN32
#include "QBSoundWin.h"
#else
#include "QBSoundMac.h"
#endif

int QBSound::oggMasterVolume=128;

#define NOISE_LONG 0x4000
static unsigned char noise_lut[NOISE_LONG];

static void create_noise(unsigned char *buf, const int bits, int size)
{
	static int m = 0x0011;
	int xor_val, i;

	for (i = 0; i < size; i++) {
		xor_val = m & 1;
		m >>= 1;
		xor_val ^= (m & 1);
		m |= xor_val << (bits - 1);
		buf[i] = m;
	}
}

static void create_pinknoise(unsigned char *buf, const int bits, int size)
{
	static int m = 0x0011;
	int xor_val, i,ii;

	for (ii = 0; ii < size; ii++) {
		i = ii/8;
		xor_val = m & 1;
		m >>= 1;
		xor_val ^= (m & 1);
		m |= xor_val << (bits - 1);
/*
		if (i > 0) {
			buf[i] = buf[i-1] + (m & 0x3)-2;		//失敗//
		} else {
			buf[i] = m;
		}
*/
		buf[i] = m;
	}
}

//#include <pthread.h>
//#include <AudioToolbox/AudioToolbox.h>
//#include <AudioUnit/AUComponent.h>

//#define kNumberAudioDataBuffers 4
#define RING_SIZE 1024

static float freqtable[] = {
	10,
	23,
	55,
	110,
	220,
	440,
	880,
	1760,
	3520,
	7040,
	14080,
};

/*
class QBSound {
public:
	QBSound(int maxChannel);
	virtual ~QBSound() {
		if (mGraph) AUGraphStop(mGraph);
	};
	
	int play(const char* mml,int channel=0);
	int isPlaying(int channel=0);

	PSGTrack* mTrack;
	int mMaxTrack;

	AudioUnit mAudioUnit;
	AudioStreamBasicDescription mAudioFormat;
	AUNode mOutputNode;
	AUGraph mGraph;

	int init_track(int maxChannel);
	void fill_sound_buffer(void* buffer,int size);
	void analyze_note(PSGTrack *track);
	signed long get_level(PSGTrack *track);
	int GetNum(PSGTrack *track);
	
	pthread_mutex_t mMutex;
};
*/

static QBSound* snd=NULL;

QBSound* QBSound_Instance()
{
	return snd;
}

int QBSound_Start(int maxChannel)
{
	if (snd == NULL) {
#ifdef _WIN32
		snd = new QBSoundWin(maxChannel);
#else
		snd = new QBSoundMac(maxChannel);
#endif
		snd->Init();
		create_noise(noise_lut, 13, NOISE_LONG);
		//create_pinknoise(noise_lut, 13, NOISE_LONG);
	}
	return 0;
}

int QBSound_Reset()
{
	if (snd) {
		snd->Reset();
	}
	return 0;
}

int QBSound_Exit()
{
	if (snd) {
		snd->Exit();
		delete snd;
	}
	snd = NULL;
	return 0;
}

int QBSound_Play(const char* mml,int track)
{
	if (snd) {
		snd->play(mml,track);
	}
	return 0;
}

int QBSound_PlayPCM(QBPCM* pcm,int track)
{
	if (snd) {
		snd->playpcm(pcm->pcmbuffer,pcm->pcmsize,pcm->pcmchannel,track,false);
	}
	return 0;
}

int QBSound_LoopPlayPCM(QBPCM* pcm,int track)
{
	if (snd) {
		snd->playpcm(pcm->pcmbuffer,pcm->pcmsize,pcm->pcmchannel,track,true);
	}
	return 0;
}

int QBSound_StreamPlayPCM(const char* filename,int track)
{
	if (snd) {
		snd->playpcm(filename,track,false);
	}
	return 0;
}

int QBSound_LoopStreamPlayPCM(const char* filename,int track,ogg_int64_t looppoint)
{
	if (snd) {
		snd->playpcm(filename,track,true,looppoint);
	}
	return 0;
}

int QBSound_StreamMemPlay(unsigned char* data,unsigned long size,int track)
{
	if (snd) {
		snd->playmempcm(data,size,track,false);
	}
	return 0;
}

int QBSound_LoopStreamMemPlay(unsigned char* data,unsigned long size,int track,ogg_int64_t looppoint)
{
	if (snd) {
		snd->playmempcm(data,size,track,true,looppoint);
	}
	return 0;
}

int QBSound_LoopPlay(const char* mml,int track)
{
	if (snd) {
		snd->play(mml,track,true);
	}
	return 0;
}

int QBSound_Stop(int track)
{
	if (snd) {
		snd->stop(track);
	}
	return 0;
}

int QBSound_IsPlaying(int track)
{
	int r=0;
	if (snd) {
		r = snd->isPlaying(track);
	}
	return r;
}

int QBSound_SetVolume(int track,float volume)
{
	if (snd) {
		snd->setVolume(track,volume);
	}
	return 0;
}

float QBSound_GetVolume(int track)
{
	if (snd) {
		return snd->getVolume(track);
	}
	return 0;
}

//static char preload_buffer[64*1024];

typedef struct _WAVE_HEADER {
	unsigned long RIFF_ChunkID;
	unsigned long RIFF_ChunkSize;
	unsigned long RIFF_FormType;
	unsigned long FMT_ChunkID;
	unsigned long FMT_ChunkSize;
	
	unsigned short wFormatTag;
	unsigned short nChannels;
	unsigned long nSamplesPerSec;
	unsigned long nAvgBytesPerSec;
	unsigned short nBlockAlign;
	unsigned short wBitsPerSample;
	//unsigned short cbSize;
	
	unsigned long DATA_ChunkID;
	unsigned long DATA_ChunkSize;
} WAVE_HEADER;

static int qbsound_preloadWav(const char* fname,QBPCM* pcm,float volume)
{
	int r=-1;
	FILE* fp = fopen(fname,"rb");
	pcm->pcmbuffer = NULL;
	if (fp) {
		WAVE_HEADER head;
		int len = fread(&head,1,sizeof(WAVE_HEADER),fp);
		if (len == sizeof(head)) {
			pcm->pcmbuffer = (signed short* )calloc(1,head.DATA_ChunkSize);
			if (pcm->pcmbuffer) {
				pcm->pcmsize = head.DATA_ChunkSize;
				pcm->pcmchannel = head.nChannels;
				fread(pcm->pcmbuffer,1,head.DATA_ChunkSize,fp);
				for (int i=0;i<pcm->pcmsize/sizeof(short);i++) {
					pcm->pcmbuffer[i] *= volume;
				}
			}
			r = 0;
		}
		fclose(fp);
	}
	return r;
}

static int qbsound_preloadOgg(const char* fname,QBPCM* pcm,float volume)
{
#ifdef _IPHONE
#else
	OggVorbis_File ovf;
	vorbis_info* info;
	pcm->pcmbuffer = NULL;
	int err;
	
	err = ov_fopen((char*)fname, &ovf);

	if ( err != 0 ) {
		switch( err ) {
		case OV_EREAD:       break;
		case OV_ENOTVORBIS:  break;
		case OV_EVERSION:    break;
		case OV_EBADHEADER:  break;
		case OV_EFAULT:      break;
		default:             break;
		}
		printf("%s %d\n",fname,err);
		return err; // エラー//
	}

	info = ov_info(&ovf,-1);

	{
		printf("version %d\n",info->version);
		printf("channels %d\n",info->channels);
		printf("rate %d\n",info->rate);
		printf("bitrate_upper %d\n",info->bitrate_upper);
		printf("bitrate_nominal %d\n",info->bitrate_nominal);
		printf("bitrate_lower %d\n",info->bitrate_lower);
		printf("bitrate_window %d\n",info->bitrate_window);
	}
	
	unsigned long totalsize = ov_pcm_total(&ovf,-1)*info->channels*2;
printf("totalsize %ld\n",totalsize);

	{
		int bitstream = 0;
		unsigned long readSize = 0;
		unsigned long sum=0;

		while (1) {
			readSize = ov_read(
			&ovf,
			preload_buffer,
			sizeof(preload_buffer),
			0,           // リトルエンディアン//
			2,           // WORDは16bit//
			1,           // 符号有り//
			&bitstream
			);
			if (readSize > 0) {
				if (pcm->pcmbuffer == NULL) {
					pcm->pcmbuffer = (short*)malloc(totalsize);
				}
				if (totalsize < sum+readSize) {
					pcm->pcmbuffer = (short*)realloc(pcm->pcmbuffer,sum+readSize);
					totalsize = sum+readSize;
				}
				memcpy(&pcm->pcmbuffer[sum/sizeof(short)],preload_buffer,readSize);
				sum += readSize;
			}
			if (readSize == 0) break;
		}
		pcm->pcmsize = sum;
		pcm->pcmchannel = info->channels;
		for (int i=0;i<pcm->pcmsize/sizeof(short);i++) {
			pcm->pcmbuffer[i] *= volume;
		}
printf("sum %ld\n",sum);
	}

	ov_clear( &ovf );
#endif
	return 0;
}

int QBSound_PreloadWave(unsigned char* data,unsigned long size,QBPCM* pcm,float volume)
{
	int r=-1;
	pcm->pcmbuffer = NULL;
	if (data) {
		WAVE_HEADER head;
		memcpy(&head,data,sizeof(WAVE_HEADER));
		pcm->pcmbuffer = (signed short* )calloc(1,head.DATA_ChunkSize);
		if (pcm->pcmbuffer) {
			pcm->pcmsize = head.DATA_ChunkSize;
			pcm->pcmchannel = head.nChannels;
			memcpy(pcm->pcmbuffer,&data[sizeof(WAVE_HEADER)],head.DATA_ChunkSize);
			for (int i=0;i<pcm->pcmsize/sizeof(short);i++) {
				pcm->pcmbuffer[i] *= volume;
			}
		}
		r = 0;
	}
	return r;
}

int QBSound_Preload(const char* fname,QBPCM* pcm,float volume)
{
	int l=strlen(fname);
	for (int i=l-1;i>0;i--) {
		if (fname[i] == '.') {
			char ext[256];
			strcpy(ext,&fname[i+1]);
			int e=strlen(ext);
			for (int j=0;j<e;j++) {
				if (ext[j] >= 'a' && ext[j] <= 'z') {
					ext[j] = ext[j]-'a'+'A';
				}
			}
			if (strcmp(ext,"OGG") == 0) {
				return qbsound_preloadOgg(fname,pcm,volume);
			} else
			if (strcmp(ext,"WAV") == 0) {
				return qbsound_preloadWav(fname,pcm,volume);
			}
		}
	}
	return -1;
}

int QBSound_DeletePCM(QBPCM* pcm)
{
	if (pcm) {
		if (pcm->pcmbuffer) free(pcm->pcmbuffer);
		pcm->pcmbuffer = NULL;
		return 0;
	}
	return -1;
}

int QBSound_FadeOut(int track,int speed)
{
	if (snd) {
		snd->fadeOut(track,speed);
	}
	return 0;
}

int QBSound_SetMasterVolume(float volume)
{
	if (snd) {
		snd->setMasterVolume(volume);
	}
	return 0;
}

float QBSound_GetMasterVolume()
{
	if (snd) {
		return snd->getMasterVolume();
	}
	return 0;
}

QBSound::QBSound(int maxChannel) : mTrack(NULL),mMaxTrack(maxChannel),mMasterVolume(1.0),mPause(1.0)
{
}

int QBSound::Init()
{
	init_track(mMaxTrack);
	return 0;
}

int QBSound::Exit()
{
	return 0;
}

int QBSound::Reset()
{
	int i;
	for (i=0;i<mMaxTrack;i++) {
		mTrack[i].v = 0x7fff*(32/64.0);//8;
		mTrack[i].fv = 1.0;
		mTrack[i].w = 0;
		mTrack[i].f = 0;
		mTrack[i].noise = (2.0*S_PI/(1000+pow(((10000-mTrack[i].f)/10000),10)*2000000));
		mTrack[i].lastlevel = 0;

//		mTrack[i].note    = (char*)calloc(1,sizeof(char)*RING_SIZE);
//		mTrack[i].noteptr = mTrack[i].note;
//		mTrack[i].ptr = mTrack[i].note;
//		mTrack[i].loopptr = NULL;
		
		mTrack[i].notect  = 0;
		mTrack[i].oct = 4;
//		mTrack[i].end = true;
		mTrack[i].type = 0;
		
		mTrack[i].nlen = 16;
		mTrack[i].tempo = 255;
		
//		mTrack[i].pcmbuffer = NULL;
//		mTrack[i].fadeout = 0;
//		mTrack[i].pcmplay = false;
		
//		mTrack[i].streamplay = false;
//		mTrack[i].streamopenov = false;
	}
	return 0;
}

int QBSound::init_track(int maxChannel)
{
	mTrack = (PSGTrack*)calloc(maxChannel,sizeof(PSGTrack));
	if (mTrack) {
		int i;
		for (i=0;i<mMaxTrack;i++) {
			mTrack[i].v = 0x7fff*(32/64.0);//8;
			mTrack[i].fv = 1.0;
			mTrack[i].w = 0;
			mTrack[i].f = 0;
			mTrack[i].noise = (2.0*S_PI/(1000+pow(((10000-mTrack[i].f)/10000),10)*2000000));
			mTrack[i].lastlevel = 0;

			mTrack[i].note    = (char*)calloc(1,sizeof(char)*RING_SIZE);
			mTrack[i].noteptr = mTrack[i].note;
			mTrack[i].ptr = mTrack[i].note;
			mTrack[i].loopptr = NULL;
			
			mTrack[i].notect  = 0;
			mTrack[i].oct = 4;
			mTrack[i].end = true;
			mTrack[i].type = 0;
			
			mTrack[i].nlen = 16;
			mTrack[i].tempo = 255;
			
			mTrack[i].pcmbuffer = NULL;
			mTrack[i].fadeout = 0;
			mTrack[i].pcmplay = false;
			
			mTrack[i].streamplay = false;
			mTrack[i].streamopenov = false;
		}
		return 0;
	}
	return -1;
}

int QBSound::play(const char *note,int trackindex,bool loop)
{
	//mTrack[trackindex].note    = (char*)calloc(1024,sizeof(char));
	//mTrack[trackindex].noteptr = mTrack[trackindex].note;
	//pthread_mutex_lock(&mMutex);
	int i=trackindex;
	char* nptr = mTrack[i].ptr;
	int l=strlen(note);
	int k=0;
	//int n = mTrack[i].ptr-mTrack[i].note;
	for (k=0;k<=l;k++) {
		*(mTrack[i].ptr) = note[k];
		if (note[k] > 0) {
			mTrack[i].ptr ++;
			if (mTrack[i].ptr >= mTrack[i].note+RING_SIZE) {
				mTrack[i].ptr = mTrack[i].note;
			}
		}
	}
	if (loop) {
		mTrack[i].loopptr = nptr;
	} else {
		mTrack[i].loopptr = NULL;
	}
	mTrack[i].end     = false;
#if 0
	mTrack[i].type = 0;
	mTrack[i].notect  = 0;
	mTrack[i].fadeout = 0;
	mTrack[i].v = 0x7fff*(8/64.0);//8;
	mTrack[i].fv = 1.0;
#endif
/*
{
	int j;
	for (j=0;j<RING_SIZE;j++) {
		if (mTrack[i].note[j] == 0) {
			printf("_");
		} else {
			printf("%c",mTrack[i].note[j]);
		}
	}
	printf("\n");
}
*/
	//pthread_mutex_unlock(&mMutex);
	//analyze_note(&mTrack[i]);
	return 0;
}

int QBSound::playpcm(signed short* dataPtr,unsigned long dataSize,int dataChannel,int track,bool loop)
{
	int i=track;
	mTrack[i].pcmbuffer = dataPtr;
	mTrack[i].pcmsize = dataSize;
	mTrack[i].pcmptr = 0;
	mTrack[i].pcmchannel = dataChannel;
	mTrack[i].pcmplay = true;
	mTrack[i].pcmloop = loop;
	//mTrack[i].streamlooppoint = looppoint;
	mTrack[i].fadeout = 0;
	mTrack[i].v = 0x7fff*(8/64.0);//8;
	mTrack[i].fv = 1.0;
	return 0;
}

int QBSound::playpcm(const char* filename,int track,bool loop,ogg_int64_t looppoint)
{
	int i=track;
	strcpy(mTrack[i].streamfile,filename);
	mTrack[i].streamdata = NULL;
	mTrack[i].streamopen = true;
	mTrack[i].streamloop = loop;
#ifdef QBSOUND_OGG
	mTrack[i].looppoint = looppoint;
#endif
	return 0;
}

int QBSound::playmempcm(unsigned char* data,unsigned long size,int track,bool loop,ogg_int64_t looppoint)
{
	int i=track;
	mTrack[i].streamdata = data;
	mTrack[i].streamsize = size;
	mTrack[i].streamopen = true;
	mTrack[i].streamloop = loop;
#ifdef QBSOUND_OGG
	mTrack[i].looppoint = looppoint;
#endif
	return 0;
}

int QBSound::sysPause()
{
	return 0;
}

int QBSound::sysPlay()
{
	return 0;
}

int QBSound::setMasterVolume(float volume)
{
	mMasterVolume = volume;
	return 0;
}

float QBSound::getMasterVolume()
{
	return mMasterVolume;
}

static int freeSize(int p1,int p2)
{
	if (p2 > p1) return QBSOUND_STREAM_BUFFER_SIZE-p2+p1;
	if (p1 > p2) return p1-p2;
	return QBSOUND_STREAM_BUFFER_SIZE;
}

static size_t qbsound_read(void* buffer,size_t size,size_t count,FILE* stream)
{
	PSGTrack* t=(PSGTrack*)stream;
	if (buffer == NULL) return 0;
	if (t == NULL) return 0;
	
	unsigned long s = size*count;
	if (t->streamsize-t->streamptr < s) {
		s = t->streamsize-t->streamptr;
	}
	
	memcpy(buffer,&t->streamdata[t->streamptr],s);
	t->streamptr += s;
	
	return s;
}

static int qbsound_seek(FILE *stream,ogg_int64_t off,int whence)
{
	PSGTrack* t=(PSGTrack*)stream;
	if (t == NULL) return -1;
	switch(whence) {
	case SEEK_CUR:
//printf("seek cur\n");
		t->streamptr += off;
		break;
	case SEEK_END:
//printf("seek end\n");
		t->streamptr = t->streamsize + off;
		break;
	case SEEK_SET:
//printf("seek set\n");
		t->streamptr = off;
		break;
	default:
		return -1;
	}
	if (t->streamptr < 0) {
		t->streamptr = 0;
		return -1;
	} else
	if (t->streamptr > t->streamsize) {
		t->streamptr = t->streamsize;
		return -1;
	}
	return 0;
}

static long qbsound_tell(FILE *stream)
{
//printf("tell\n");
	PSGTrack* t=(PSGTrack*)stream;
	if (t == NULL) return 0;
	return t->streamptr;
}

static int qbsound_close(FILE *stream)
{
//printf("close\n");
	PSGTrack* t=(PSGTrack*)stream;
	if (t == NULL) return -1;
	t->streamptr = 0;
	return 0;
}

int QBSound::streamOpen(int track)
{
	int i=track;
	if (mTrack[i].streamopen) {
		mTrack[i].streamopen = false;
		int err;
		int i=track;
		if (mTrack[i].streamopenov) {
#ifdef _IPHONE_ACC
#else
			ov_clear(&mTrack[i].streamovf);
#endif
			mTrack[i].streamopenov = false;
		}
		if (mTrack[i].streamdata) {
#ifdef _IPHONE_ACC
#else
			ov_callbacks callbacks = {
				(size_t (*)(void *, size_t, size_t, void *)) qbsound_read,
				(int (*)(void *, ogg_int64_t, int))          qbsound_seek,
				(int (*)(void *))                            qbsound_close,
				(long (*)(void *))                           qbsound_tell
			};
			mTrack[i].streamptr = 0;
 			err = ov_open_callbacks(&mTrack[i],&mTrack[i].streamovf,0,NULL,callbacks);
#endif
		} else {
#ifdef _IPHONE_ACC
#else
			err = ov_fopen((char*)mTrack[i].streamfile,&mTrack[i].streamovf);
#endif
			if (err == 0) {
#ifdef _IPHONE_ACC
#else
				vorbis_info* info;
				info = ov_info(&mTrack[i].streamovf,-1);
#endif
				mTrack[i].streamp1 = 0;
				mTrack[i].streamp2 = 0;
				//mTrack[i].streamloop = loop;
#ifdef _IPHONE_ACC
#else
				mTrack[i].streamchannel = info->channels;
#endif
				mTrack[i].streamplay = true;
				mTrack[i].streamplayrun = false;
				mTrack[i].streamdone = false;
				mTrack[i].fadeout = 0;
				mTrack[i].v = 0x7fff*(8/64.0);//8;
				mTrack[i].fv = 1.0;
				mTrack[i].streamopenov = true;
	//printf("playpcm %s\n",filename);
			}
		}
	}
	return 0;
}

int QBSound::streamLoad(int track,int p1,int p2,int* done)
{
	int i=track;
	int readSum = 0;
	int size=0;
	signed long readSize;
	while (freeSize(p1,p2) > QBSOUND_STREAM_BUFFER_SIZE/16) {
		size = QBSOUND_STREAM_BUFFER_SIZE-p1;
		if (p1 > p2) size = p1-p2-1;
		if (p1 < p2) size = QBSOUND_STREAM_BUFFER_SIZE-p2;
		if (size < 1) break;
#ifdef _IPHONE_ACC
#else
		int bitstream = 0;
		readSize = ov_read( &mTrack[i].streamovf,
							(char*)&mTrack[i].streambuffer[p2],
							size*sizeof(short),
							0,           // リトルエンディアン//
							2,           // WORDは16bit//
							1,           // 符号有り//
							&bitstream);
//printf("%d,%d\n",size,readSize);
#endif
		if (readSize <= 0) {
#ifdef _IPHONE_ACC
#else
			ov_pcm_seek(&mTrack[i].streamovf,mTrack[i].looppoint);
#endif
			mTrack[i].streampoint = 0;
			*done = 1;
			return p2;
		}
#ifdef _IPHONE_ACC
#else
		mTrack[i].streampoint = ov_pcm_tell(&mTrack[i].streamovf);
#endif
		p2 += readSize/sizeof(short);
		p2 %= QBSOUND_STREAM_BUFFER_SIZE;
		readSum += readSize;
/*
		if (readSum >= 32*1024 || p1 == p2) {
			break;
		}
*/
	}
	return p2;
}

int QBSound::p1(int track)
{
	return mTrack[track].streamp1;
}

int QBSound::p2(int track)
{
	return mTrack[track].streamp2;
}

ogg_int64_t QBSound::streampoint(int track)
{
	return mTrack[track].streampoint;
}

int QBSound::stop(int trackindex)
{
	if (trackindex < 0 || trackindex >= mMaxTrack) {
		int i;
		for (i=0;i<mMaxTrack;i++) {
			*(mTrack[i].noteptr) = 0;
			mTrack[i].ptr = mTrack[i].noteptr;
			mTrack[i].loopptr = NULL;
			mTrack[i].f = 0;
			mTrack[i].end = true;
			mTrack[i].pcmplay = false;
			mTrack[i].streamplay = false;
			mTrack[i].streamplayrun = false;
		}
	} else {
		int i = trackindex;
		*(mTrack[i].noteptr) = 0;
		mTrack[i].ptr = mTrack[i].noteptr;
		mTrack[i].loopptr = NULL;
		mTrack[i].f = 0;
		mTrack[i].end = true;
		mTrack[i].pcmplay = false;
		mTrack[i].streamplay = false;
		mTrack[i].streamplayrun = false;
	}
	return 0;
}

int QBSound::isPlaying(int track)
{
	if (track < 0 || track >= mMaxTrack) {
		int i;
		for (i=0;i<mMaxTrack;i++) {
			if (mTrack[i].pcmplay == true) {
				return 1;
			} else
			if (mTrack[i].streamplay == true || mTrack[i].streamopen == true) {
				return 1;
			} else
			if (mTrack[i].end == false) {
				return 1;
			}
		}
	} else {
		if (mTrack[track].pcmplay == true) {
			return 1;
		} else
		if (mTrack[track].streamplay == true || mTrack[track].streamopen == true) {
			return 1;
		} else
		if (mTrack[track].end == false) {
			return 1;
		}
	}
	return 0;
}

int QBSound::fadeOut(int track,int speed)
{
	if (track < 0 || track >= mMaxTrack) {
	} else {
		if (mTrack[track].v > 0) {
			mTrack[track].fadeout = speed*440000/10000/mTrack[track].v;
			mTrack[track].fademax = mTrack[track].v;//mTrack[track].fadeout;
			mTrack[track].fadecount = 0;//mTrack[track].v/speed;
		}
		return 1;
	}
	return 0;
}

int QBSound::fill_sound_buffer(void* buffer,int size)
{
	signed char* b = (signed char*)buffer;
	int i,j;
	int r=0;

	for (int j=0;j<mMaxTrack;j++) {
		if (mTrack[j].streamplay == true) {
			//printf("%d:%d,%d\n",j,mTrack[j].streamp1,mTrack[j].streamp2);
		}
	}

	int endFlag = 0;
	for (i=0;i<(size/2)/sizeof(signed short)||b==NULL;i++) {
		signed long s1 = 0;
		signed long s2 = 0;

		//pthread_mutex_lock(&mMutex);
		for (j=0;j<mMaxTrack;j++) {
			analyze_note(&mTrack[j]);
			s1 += get_level(&mTrack[j]);
			s2 = s1;
		}
//#ifndef _IPHONE_ACC
		for (j=0;j<mMaxTrack;j++) {
			if (mTrack[j].pcmbuffer != NULL && mTrack[j].pcmplay == true) {
				if (mTrack[j].pcmptr >= mTrack[j].pcmsize/sizeof(short)) {
					if (mTrack[j].pcmloop) {
						mTrack[j].pcmptr = 0;
					} else {
						mTrack[j].pcmplay = false;
						continue;
					}
				}
				s1 += mTrack[j].v*mTrack[j].fv*mTrack[j].pcmbuffer[mTrack[j].pcmptr]/(0x7fff*8/64);
				if (mTrack[j].pcmchannel > 1) {
					s2 += mTrack[j].v*mTrack[j].fv*mTrack[j].pcmbuffer[mTrack[j].pcmptr+1]/(0x7fff*8/64);
				} else {
					s2 = s1;
				}
				mTrack[j].pcmptr+=mTrack[j].pcmchannel;
			}
		}
		for (j=0;j<mMaxTrack;j++) {
			if (mTrack[j].streamplay == true && mTrack[j].streamplayrun == true) {
				//if (freeSize(mTrack[j].streamp1,mTrack[j].streamp2) <= QBSOUND_STREAM_BUFFER_SIZE/2) {
				if (mTrack[j].streamp1 != mTrack[j].streamp2) {
					s1 += mTrack[j].v*mTrack[j].fv*oggMasterVolume*mTrack[j].streambuffer[mTrack[j].streamp1]/(0x7fff*8*128/64);
					if (mTrack[j].streamchannel > 1) {
						s2 += mTrack[j].v*mTrack[j].fv*oggMasterVolume*mTrack[j].streambuffer[mTrack[j].streamp1+1]/(0x7fff*8*128/64);
					} else {
						s2 = s1;
					}
					mTrack[j].streamp1 += mTrack[j].streamchannel;
					mTrack[j].streamp1 %= QBSOUND_STREAM_BUFFER_SIZE;
				} else 
				if (mTrack[j].streamdone) {
					mTrack[j].streamplay = false;
					mTrack[j].streamplayrun = false;
				}
			}
		}

		for (j=0;j<mMaxTrack;j++) {
			if (mTrack[j].fadeout > 0) {
				mTrack[j].fadecount ++;
				if (mTrack[j].fadecount > mTrack[j].fadeout) {
					mTrack[j].fadecount = 0;
					//mTrack[j].fadeout --;
					mTrack[j].fv -= 1.0/mTrack[j].fademax;
					//mTrack[j].v --;
					if (mTrack[j].fv <= 0) 
					{
						mTrack[j].fv = 0;
						mTrack[j].fadeout = 0;
						QBSound::stop(j);
						//mTrack[j].pcmplay = false;
					}
				}
			}
		}
//#endif
		//pthread_mutex_unlock(&mMutex);
		if (s1 >= 0x7fff) s1 =  0x7fff;
		if (s1 < -0x7fff) s1 = -0x7fff;
		if (s2 >= 0x7fff) s2 =  0x7fff;
		if (s2 < -0x7fff) s2 = -0x7fff;
		
		s1 *= mMasterVolume*mPause;
		s2 *= mMasterVolume*mPause;

		if (b) {
#if __BIG_ENDIAN__
			b[i*4+0] = (s1 & 0xff00)>>8;
			b[i*4+1] = (s1 & 0xff);

			b[i*4+2] = (s2 & 0xff00)>>8;
			b[i*4+3] = (s2 & 0xff);
#else
			b[i*4+1] = (s1 & 0xff00)>>8;
			b[i*4+0] = (s1 & 0xff);

			b[i*4+3] = (s2 & 0xff00)>>8;
			b[i*4+2] = (s2 & 0xff);
#endif
		}

		if (endFlag == 0) {
			endFlag = 1;
			for (j=0;j<mMaxTrack;j++) {
				if (QBSound::isPlaying(j) == true) {
					endFlag = 0;
				}
			}
			if (endFlag == 1) {
				r = i*sizeof(short)*2;
				if (b == NULL) break;
			}
		}
	}
	if (endFlag == 0) {
		r = size;
	}
	
	return r;
}

void QBSound::analyze_note(PSGTrack *track)
{
	if (track->noteptr == NULL) goto End;
	if (track->note    == NULL) goto End;

	if (track->notect == 0) {
		if (*track->noteptr == 0) {
			if (track->loopptr == NULL) {
				track->f = 0;
				goto End;
			} else {
				track->noteptr = track->loopptr;
			}
		}
		while (1) {
			float freq;
Loop:
			if (track->noteptr >= track->note+RING_SIZE) {
				track->noteptr = track->note;
			}
			if (*track->noteptr == 0) {
				if (track->loopptr == NULL) {
					track->f = 0;
					goto End;
				} else {
					track->noteptr = track->loopptr;
				}
			}
			freq = freqtable[track->oct];
			switch (*track->noteptr) {
			case '>':		//オクターブ+//
				if (track->oct < 8) {
					track->oct ++;
				}
				track->noteptr ++;
				if (track->noteptr >= track->note+RING_SIZE) {
					track->noteptr = track->note;
				}
				goto Loop;
			case '<':		//オクターブ-//
				if (track->oct > 0) {
					track->oct --;
				}
				track->noteptr ++;
				if (track->noteptr >= track->note+RING_SIZE) {
					track->noteptr = track->note;
				}
				goto Loop;
			case 'O':		//オクターブ//
			case 'o':		//オクターブ//
				{
					char* p = track->noteptr;
					p ++;
					if (p >= track->note+RING_SIZE) {
						p = track->note;
					}
					char a = *p;
					if (a >= '0' && a <= '9') {
						track->noteptr ++;
						if (track->noteptr >= track->note+RING_SIZE) {
							track->noteptr = track->note;
						}
						track->oct = GetNum(track);
						if (track->oct > 8) {
							track->oct = 8;
						}
					}
				}
				goto Loop;
			case 'M':		//音色//
			case 'm':		//音色//
				{
					char* p = track->noteptr;
					p ++;
					if (p >= track->note+RING_SIZE) {
						p = track->note;
					}
					char a = *p;
					if (a >= '0' && a <= '9') {
						track->noteptr ++;
						if (track->noteptr >= track->note+RING_SIZE) {
							track->noteptr = track->note;
						}
						track->type = GetNum(track);
					}
				}
				goto Loop;
			case 'V':		//ボリューム//
			case 'v':		//ボリューム//
				{
					char* p = track->noteptr;
					p ++;
					if (p >= track->note+RING_SIZE) {
						p = track->note;
					}
					char a = *p;
					if (a >= '0' && a <= '9') {
						track->noteptr ++;
						if (track->noteptr >= track->note+RING_SIZE) {
							track->noteptr = track->note;
						}
						{
							int v = (GetNum(track)/2);
							if (v > 64) {
								v = 64;
							}
							track->v = 0x7fff*v/64.0;
						}
					}
				}
				goto Loop;
			case '+':		//ボリューム+//
				track->v += 0x7fff/64.0;
				if (track->v > 0x7fff) {
					track->v = 0x7fff;
				}
				track->noteptr ++;
				if (track->noteptr >= track->note+RING_SIZE) {
					track->noteptr = track->note;
				}
				goto Loop;
			case '-':		//ボリューム-//
				track->v -= 0x7fff/64.0;
				if (track->v < 0) {
					track->v = 0;
				}
				track->noteptr ++;
				if (track->noteptr >= track->note+RING_SIZE) {
					track->noteptr = track->note;
				}
				goto Loop;
			case 'L':		//音長//
			case 'l':		//音長//
				{
					char* p = track->noteptr;
					p ++;
					if (p >= track->note+RING_SIZE) {
						p = track->note;
					}
					char a = *p;
					if (a >= '0' && a <= '9') {
						track->noteptr ++;
						if (track->noteptr >= track->note+RING_SIZE) {
							track->noteptr = track->note;
						}
						track->nlen = GetNum(track);
					}
				}
				goto Loop;
			case 'T':
			case 't':
				{
					char* p = track->noteptr;
					p ++;
					if (p >= track->note+RING_SIZE) {
						p = track->note;
					}
					char a = *p;
					if (a >= '0' && a <= '9') {
						track->noteptr ++;
						if (track->noteptr >= track->note+RING_SIZE) {
							track->noteptr = track->note;
						}
#if 1
						track->tempo = GetNum(track);
#else
						track->tempo = 256-GetNum(track);
						if (track->tempo < 1) track->tempo = 1;
#endif
					}
				}
				goto Loop;
			//case '/':		//ループ//
				//track->noteptr = track->note;
				//goto Loop;
			default:
				if ((*track->noteptr >= 'A' && *track->noteptr <= 'G')
				  || (*track->noteptr >= 'a' && *track->noteptr <= 'g')
				  || *track->noteptr == 'R'|| *track->noteptr == 'r'
				  || *track->noteptr == 'P'|| *track->noteptr == 'p') {
					int note = 0;
					int nlen = 0;
					int noteFlag = 1;
//printf("%c ",(char)*track->noteptr);
					switch (*track->noteptr) {
					case 'C':
					case 'c':
						note = 3;
						break;
					case 'D':
					case 'd':
						note = 5;
						break;
					case 'E':
					case 'e':
						note = 7;
						break;
					case 'F':
					case 'f':
						note = 8;
						break;
					case 'G':
					case 'g':
						note = 10;
						break;
					case 'A':
					case 'a':
						note = 12;
						break;
					case 'B':
					case 'b':
						note = 14;
						break;
					case 'R':
					case 'r':
					case 'P':
					case 'p':
						noteFlag = 0;
						track->f = 0;
						break;
					}
					track->noteptr ++;
					if (track->noteptr >= track->note+RING_SIZE) {
						track->noteptr = track->note;
					}
					//シャープ//
					if (noteFlag) {
						if (*track->noteptr == '#') {
							track->noteptr ++;
							if (track->noteptr >= track->note+RING_SIZE) {
								track->noteptr = track->note;
							}
							note ++;
						}
					}
					//音高//
					if (noteFlag) {
						if (note == 0) {
							track->f = freq;
						} else {
							track->f = freq * powf(2.0,note/12.0);
						}
						if (track->type == 5) {
							track->noise = (2.0*S_PI/(1000+powf(((10000-track->f)/10000),10)*2000000));
						}
					}
					//音長//
					{
						char a = *track->noteptr;
						if (a >= '0' && a <= '9') {
							nlen = GetNum(track);
						} else {
							nlen = track->nlen;
						}
						switch (nlen) {
						case 1:
							nlen = 64;
							break;
						case 2:
							nlen = 32;
							break;
						case 4:
							nlen = 16;
							break;
						case 8:
							nlen = 8;
							break;
						case 16:
							nlen = 4;
							break;
						case 32:
							nlen = 2;
							break;
						case 64:
							nlen = 1;
							break;
						default:
							break;
						}
						//track->nlen = nlen;
						track->notect = nlen*255*track->tempo/255*2;
//printf("%d\n",track->notect);
						//if (track->notect <= 100) track->notect = 100;
					}
					break;
				} else {
					track->noteptr ++;
					if (track->noteptr >= track->note+RING_SIZE) {
						track->noteptr = track->note;
					}
					goto Loop;
				}
			}
			break;
		}
	}
	if (track->notect > 0) {
		track->notect --;
	}
	return;
End:
	track->end = true;
	return;
}

signed long QBSound::get_level(PSGTrack *track)
{
	signed long s = 0;
	float w;

	if (track->f == 0) {
		track->w = 0;
		if (track->lastlevel > 0) {
			track->lastlevel --;
		} else
		if (track->lastlevel < 0) {
			track->lastlevel ++;
		}
		return track->lastlevel;
	}

	switch (track->type) {
	case 5:
		//track->w += (2.0*S_PI/(1000));//(2.0*S_PI*track->f/44100.0)/powf(10,track->f/100);
		//track->w += (2.0*S_PI/(2000000));//(2.0*S_PI*track->f/44100.0)/powf(10,track->f/100);
		track->w += track->noise;//(2.0*S_PI/(1000+powf(((10000-track->f)/10000),10)*2000000));//(2.0*S_PI*track->f/44100.0)/powf(10,track->f/100);
		if (track->w >= 2*S_PI) track->w -= 2*S_PI;
		break;
	default:
		track->w += 2.0*S_PI*track->f/44100.0;
		if (track->w >= 2*S_PI) track->w -= 2*S_PI;
		break;
	}
	w = track->w;
	switch (track->type) {
	case 0:
		s += (signed short)(track->v*0.5*(w>S_PI?1:-1));			//矩形波(PSG的)
		break;
	case 1:
		s += (signed short)(track->v*0.5*(w<2*S_PI/12?1:-1));		//矩形波(ファミコン的)
		break;
	case 2:
		s += (signed short)(track->v*0.5*sinf((w+S_PI)));			//サイン波形
		break;
	case 3:
		s += (signed short)(track->v*0.5*(w-S_PI)/S_PI);			//三角波
		break;
	case 4:
		s += (signed short)(track->v*0.5*(((w>S_PI?(2*S_PI-w):w)/*-S_PI/2*/)/(S_PI/2))*4);		//三角波(ファミコン的)
		break;
	case 5:
		//s += (signed short)(track->v*((w>S_PI?1:-1)*(((((float)noise_lut[(int)(w/(2*S_PI)*NOISE_LONG)])/256.0)-1)*1.0)));			//矩形波(PSG的)
		s += (signed short)(track->v*1.5*(((((float)noise_lut[(int)(w/(2*S_PI)*NOISE_LONG)])/256.0)-1)));	//ノイズ
		break;
	}
	track->lastlevel = s;
	return s;
}

int QBSound::GetNum(PSGTrack *track)
{
	char a;
	int retval = 0;
Loop:
	a = *track->noteptr;
	if (a >= '0' && a <= '9') {
		track->noteptr ++;
		if (track->noteptr >= track->note+RING_SIZE) {
			track->noteptr = track->note;
		}
		retval *= 10;
		retval += a - '0';
		goto Loop;
	}
	return retval;
}

int QBSound::setVolume(int track,float volume)
{
	mTrack[track].v = (0x7fff*8/64.0)*volume;
	return 0;
}

float QBSound::getVolume(int track)
{
	return mTrack[track].v/(0x7fff*8/64.0);
}

//static bool bgm_loop = false;
#if defined(_IPHONE)

void H_OpenMovieFile(const char *fname,bool loop)
{
}

#elif (_MACOSX)

#include <QuickTime/Movies.h>

static int bgm_off=FALSE;
static Movie theMovie;
//static short movieFileRef;
static int MoviePlayFlag;
static int MoviePlayStop;

int H_PlayMovieChk(void);
void H_StopMovie(void);
void H_RestartMovie(void);

typedef struct MyAppData
{
    Movie         myMovie;
    Boolean       keepLooping;
    TimeRecord    movieStartTime;
    QTCallBack    myQTCallBack;
    QTCallBackUPP myCallBackUPP;
} MyAppData, *MyAppDataPtr;

static MyAppData gAppData;

static void H_OpenMovieFile_Core(const char *fname,bool loop);
OSErr setupMovieCallback(MyAppDataPtr inAppData);
pascal void myMovieCallback(QTCallBack cb, long refCon);

pascal void myMovieCallback(QTCallBack cb, long refCon)
{
    if (NULL == refCon) return;

    MyAppDataPtr myData;
    myData = (MyAppDataPtr)refCon;

    // do we want to continue looping?
    if (myData->keepLooping)
    {
        // reschedule callback event to trigger at movie stop
        OSErr err = CallMeWhen(myData->myQTCallBack,
                    myData->myCallBackUPP,
                    (long)myData,
                    triggerAtStop,
                    0,
                    0);

        // You must issue a Stop/SetTime/Start sequence to restart
        // at the movie loop start
        StopMovie(myData->myMovie);
        SetMovieTime(myData->myMovie, &myData->movieStartTime);
        StartMovie(myData->myMovie);
printf("myMovieCallback\n");
    }

    return;
}

OSErr setupMovieCallback(MyAppDataPtr inAppData)
{
    QTCallBack    theQTCallBack;
    QTCallBackUPP theCallBackUPP;

    OSErr err = paramErr;

    // Create a new callback event
    theQTCallBack = NewCallBack(GetMovieTimeBase(inAppData->myMovie),callBackAtExtremes);
    if (theQTCallBack)
    {
        // Now schedule our callback event to trigger at movie stop
        theCallBackUPP = NewQTCallBackUPP(&myMovieCallback);
        err = CallMeWhen(theQTCallBack,
                        theCallBackUPP,
                        (long)inAppData,
                        triggerAtStop,
                        0,
                        0);
    }

    // Save these so we can use them in our callback.
    // We'll also need to dispose of them properly later
    inAppData->myQTCallBack = theQTCallBack;
    inAppData->myCallBackUPP = theCallBackUPP;

    return err;
}

// continuously loop an audio file
static void loopAMovie(Movie inMovie)
{
    // save movie for easy reference in the callback
    gAppData.myMovie = inMovie;

    // setup timebase callback for when the movie stops
    setupMovieCallback(&gAppData);

    // we'll check this flag in the callback to decide whether
    // or not to continue movie looping
    gAppData.keepLooping = bgm_loop;

    // get & save movie start time for reference later in the callback
    GoToBeginningOfMovie (gAppData.myMovie);
    GetMovieTime(gAppData.myMovie, &gAppData.movieStartTime);

    // start the movie looping!
    StartMovie(gAppData.myMovie);
}

void H_InitMovie(void)
{
	MoviePlayFlag = FALSE;
	MoviePlayStop = FALSE;
}

void H_MovieIdle(void)
{
	if (bgm_off) return;
	if (MoviePlayFlag && !MoviePlayStop) {
		MoviesTask(theMovie, 0);
/*
		if (IsMovieDone(theMovie)){
			GoToBeginningOfMovie(theMovie);
			StartMovie(theMovie);
		}
*/
	}
}

void H_OpenMovieFile(const char *fname,bool loop)
{
printf("%s\n",fname);
	H_OpenMovieFile_Core(fname,loop);
	//pthread_mutex_lock(&mutex);
	//strcpy(playbgmname,fname);
	//pthread_mutex_unlock(&mutex);
}

static void H_OpenMovieFile_Core(const char *fname,bool loop)
{
	bgm_loop = loop;
#if 1
	//FSSpec theMovieFile;
	//char name[1024];

	if (bgm_off) return;
	if (MoviePlayFlag) {
		H_StopMovie();
		DisposeMovie(theMovie);
	}
	
	//EnterMoviesOnThread(0);
	
	//strcpy(name,"");
	//strcat(name,fname);

#if 0
	CFStringRef p = CFStringCreateWithFileSystemRepresentation(0, name);
	theMovie = createAudioRefMovie(p);
	if (theMovie) {
		GoToBeginningOfMovie(theMovie);
		StartMovie(theMovie);
		MoviePlayFlag = TRUE;
		MoviePlayStop = FALSE;
	}
	CFRelease(p);
#else
	FSRef fsref;
	Boolean dir;
	if (FSPathMakeRef((unsigned char*)fname,&fsref,&dir) == 0) 
	{
		//CtoPstr((char *)name);
		//FSMakeFSSpec(procFile.vRefNum,procFile.parID,(const unsigned char *)name,&theMovieFile);
		
		//OpenMovieFile(&theMovieFile,&movieFileRef,fsRdWrPerm);
		//NewMovieFromFile(&theMovie,movieFileRef,nil,nil,newMovieActive,nil);
		
		OSType type;
		Handle h=0;
		if (QTNewDataReferenceFromFSRef(&fsref,0,&h,&type) == 0) 
		{
			//OSType type=PointerDataHandlerSubType;
			short resid;
			//HLock(h);
			char* t= (char*)&type;
printf("type %c%c%c%c\n",t[0],t[1],t[2],t[3]);
			if (NewMovieFromDataRef(&theMovie,newMovieActive /*newMovieDontInteractWithUser | newMovieAsyncOK*//* | newMovieActive*//* | newMovieIdleImportOK | newMovieDontAutoAlternates*/,&resid,h,type) == 0) {
//printf("OpenMovie %s\n",name);
				//CloseMovieFile(movieFileRef);
#if 0
				LoadMovieIntoRam(theMovie,0,GetMovieDuration(theMovie),0);
				SetMoviePreferredRate(theMovie,0x00010000);
				GoToBeginningOfMovie(theMovie);
				//SetMoviePlayHints(theMovie,hintsScrubMode,hintsLoop | hintsUseSoundInterp | hintsAllowInterlace | hintsDontPurge);
				StartMovie(theMovie);
#endif
				loopAMovie(theMovie);
				MoviePlayFlag = TRUE;
				MoviePlayStop = FALSE;
			}
		}
	}
#endif
#endif
/*
	{
		struct sched_param sp;
		memset(&sp,0,sizeof(struct sched_param));
		sp.sched_priority = 100;
		if (pthread_setschedparam(pthread_self(),SCHED_RR,&sp) == -1) {
		}
	}
*/
	//AttachMovieToCurrentThread(theMovie);
	//DetachMovieFromCurrentThread(theMovie);
	//ExitMoviesOnThread();
}

int H_PlayMovieChk(void)
{
#if 1
	if (bgm_off) return FALSE;
	if (MoviePlayFlag && !MoviePlayStop) {
		if (!IsMovieDone(theMovie))
			return true;
	}
#endif
	return FALSE;
}

void H_StopMovie(void)
{
#if 1
	if (bgm_off) return;
	StopMovie(theMovie);
	MoviePlayStop = FALSE;
#endif
}

void H_RestartMovie(void)
{
#if 1
	if (bgm_off) return;
	StartMovie(theMovie);
	MoviePlayStop = true;
#endif
}

#else

#if 1
#include <windows.h>
#include <mmsystem.h>

struct EVENT {
	BYTE   state;   // ステータスバイト
	BYTE   data1;   // 第一データバイト
	BYTE   data2;   // 第二データバイト
	BYTE   type;    // タイプ
	int    nData;   // データ長
	LPBYTE lpData;  // 可変長データ
	DWORD  dwDelta; // デルタタイム

	struct EVENT *lpNext; // 次のイベントへのポインタ
};
typedef struct EVENT EVENT;
typedef struct EVENT *LPEVENT;

struct SHORTEVENT {
	DWORD dwDeltaTime;
	DWORD dwStreamID;
	DWORD dwEvent;
};
typedef struct SHORTEVENT SHORTEVENT;
typedef struct SHORTEVENT *LPSHORTEVENT;

const int dwMaxStreamSize = 65500;

static int       nStreamReset = 0;
static int       nStreamCount = 0;
static WORD      wTime        = 0;
static BOOL      bPlayMusic   = FALSE;
static HANDLE    hheap        = NULL;
static LPEVENT   lpHeader     = NULL;
static HMIDISTRM hms          = NULL;
static LPMIDIHDR lpStream     = NULL;

static void    Destroy(void);
static void    SetStream(void);
static void    PlayStream(BOOL bFirst);
static int     GetStreamCount(void);
static BOOL    LoadFile(HANDLE);
static void    Reverse(LPVOID, int);
static BOOL    OpenDialog(HWND, LPTSTR);
static void    ReadDelta(HANDLE, LPDWORD);
static BOOL    ReadTrack(HANDLE, LPEVENT *);
static LPVOID  Alloc(int);
static LPEVENT Marge(LPEVENT *, int);
static DWORD WINAPI ThreadProc(LPVOID);
static LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

static int GetStreamCount(void)
{
	DWORD   dwStreamSize = 0;
	LPEVENT lpEvent      = lpHeader;

	while (lpEvent != NULL) {
		dwStreamSize += sizeof(SHORTEVENT);
		if (lpEvent->state == 0xF0)
			dwStreamSize += ((lpEvent->nData + 3) / 4 ) * 4;

		lpEvent = lpEvent->lpNext;
	}

	return (dwStreamSize / dwMaxStreamSize) + 1;
}

static void SetStream(void)
{
	int          i;
	DWORD        dwSize;
	LPEVENT      lpEvent = lpHeader;
	LPSHORTEVENT lpShort;

	lpStream = (LPMIDIHDR)Alloc(sizeof(MIDIHDR) * nStreamCount);

	for (i = 0; i < nStreamCount; i++) {
		lpStream[i].lpData          = (LPSTR)Alloc(dwMaxStreamSize);
		lpStream[i].dwFlags         = 0;
		lpStream[i].dwBufferLength  = dwMaxStreamSize;
		lpStream[i].dwBytesRecorded = 0;
	}

	i = 0;

	lpShort = (LPSHORTEVENT)lpStream[i].lpData;
	lpShort->dwStreamID  = 0;
	lpShort->dwDeltaTime = 0;
	lpShort->dwEvent     = ((DWORD)MEVT_TEMPO << 24) | 500000;
	lpStream[i].dwBytesRecorded = sizeof(SHORTEVENT);

	while (lpEvent != NULL) {
		dwSize = sizeof(SHORTEVENT);
		if (lpEvent->state == 0xF0)
			dwSize += ((lpEvent->nData + 3) / 4 ) * 4;

		if (lpStream[i].dwBytesRecorded + dwSize > dwMaxStreamSize)
			i++;
		
		lpShort = (LPSHORTEVENT)(lpStream[i].lpData + lpStream[i].dwBytesRecorded);
		lpShort->dwStreamID  = 0;
		lpShort->dwDeltaTime = lpEvent->dwDelta;
		
		if (lpEvent->state == 0xFF) {
			if (lpEvent->type == 0x51) {
				lpShort->dwEvent  = (DWORD)(lpEvent->lpData[2] | (lpEvent->lpData[1] << 8) | (lpEvent->lpData[0] << 16));
				lpShort->dwEvent |= ((DWORD)MEVT_TEMPO << 24);
			}
			else
				lpShort->dwEvent = (DWORD)(MEVT_NOP << 24);
		}
		else if (lpEvent->state == 0xF0) {
			int       j;
			int       nData;
			LPBYTE    lp;
			MIDIEVENT *lpSysEx;

			nData = ((lpEvent->nData + 3) / 4 ) * 4;
			
			lpSysEx = (MIDIEVENT *)(lpStream[i].lpData + lpStream[i].dwBytesRecorded);
			lpSysEx->dwStreamID  = 0;
			lpSysEx->dwDeltaTime = lpEvent->dwDelta;
			lpSysEx->dwEvent     = MEVT_F_LONG | nData;

			lp = (LPBYTE)lpSysEx->dwParms;

			for (j = 0; j < lpEvent->nData; j++)
				lp[j] = lpEvent->lpData[j];
			
			for (; j < nData; j++)
				lp[j] = 0;
		}
		else {
			lpShort->dwEvent  = (DWORD)(lpEvent->state | (lpEvent->data1 << 8) | (lpEvent->data2 << 16));
			lpShort->dwEvent |= MEVT_F_SHORT;
		}
		
		lpStream[i].dwBytesRecorded += dwSize;

		lpEvent = lpEvent->lpNext;
	}
}

static void PlayStream(BOOL bFirst)
{
	int             i;
	MIDIPROPTIMEDIV mptv;
	
	if (bFirst) {
		midiOutSetVolume((HMIDIOUT)hms,0x60006000);
		mptv.cbStruct  = sizeof(MIDIPROPTIMEDIV);
		mptv.dwTimeDiv = wTime;
		midiStreamProperty(hms, (LPBYTE)&mptv, MIDIPROP_TIMEDIV | MIDIPROP_SET);
	}

	for (i = 0; i < nStreamCount; i++) {
		midiOutPrepareHeader((HMIDIOUT)hms, &lpStream[i], sizeof(MIDIHDR));
		midiStreamOut(hms, &lpStream[i], sizeof(MIDIHDR));
	}

	midiStreamRestart(hms);
}

static BOOL LoadFile(HANDLE hfile)
{
	int     i;
	WORD    wTrack;
	WORD    wFormat;
	DWORD   dwMagic;
	DWORD   dwResult;
	DWORD   dwDataLen;
	LPEVENT *lpaEvent; // 各トラック内の最初のイベントを指すポインタ配列

	ReadFile(hfile, &dwMagic, sizeof(DWORD), &dwResult, NULL);
	if (dwMagic != *(LPDWORD)"MThd")
		return FALSE;
	
	ReadFile(hfile, &dwDataLen, sizeof(DWORD), &dwResult, NULL);
	Reverse(&dwDataLen, sizeof(DWORD));
	if (dwDataLen != 6)
		return FALSE;
	
	ReadFile(hfile, &wFormat, sizeof(WORD), &dwResult, NULL);
	Reverse(&wFormat, sizeof(WORD));
	
	ReadFile(hfile, &wTrack, sizeof(WORD), &dwResult, NULL);
	Reverse(&wTrack, sizeof(WORD));
	
	ReadFile(hfile, &wTime, sizeof(WORD), &dwResult, NULL);
	Reverse(&wTime, sizeof(WORD));

	lpaEvent = (LPEVENT *)Alloc(sizeof(DWORD) * wTrack);
	
	for (i = 0; i < wTrack; i++) {
		if (!ReadTrack(hfile, &lpaEvent[i])) {
			MessageBox(NULL, TEXT("不正なトラックです。"), NULL, MB_ICONWARNING);
			return FALSE;
		}
	}

	if (wFormat == 0)
		lpHeader = lpaEvent[0];
	else
		lpHeader = Marge(lpaEvent, wTrack);
	
	return TRUE;
}

static void Destroy(void)
{
	if (bPlayMusic) {
		int i;

		midiStreamStop(hms);
		
		for (i = 0; i < nStreamCount; i++)
			midiOutUnprepareHeader((HMIDIOUT)hms, &lpStream[i], sizeof(MIDIHDR));

		bPlayMusic = FALSE;
		nStreamReset = nStreamCount;
	}
	
	if (hheap != NULL) {
		HeapDestroy(hheap);
		hheap = NULL;
	}
}

static BOOL ReadTrack(HANDLE hfile, LPEVENT *lplpEvent)
{
	BYTE    statePrev = 0; // 前のイベントのステータスバイト
	DWORD   dwLen;
	DWORD   dwMagic;
	DWORD   dwResult;
	LPEVENT lpEvent;
	
	ReadFile(hfile, &dwMagic, sizeof(DWORD), &dwResult, NULL);
	if (dwMagic != *(LPDWORD)"MTrk")
		return FALSE;
	
	ReadFile(hfile, &dwLen, sizeof(DWORD), &dwResult, NULL);
	Reverse(&dwLen, sizeof(DWORD));

	lpEvent = (LPEVENT)Alloc(sizeof(EVENT)); // 最初のイベントのメモリを確保

	*lplpEvent = lpEvent; // *lplpEventは常に最初のイベントを指す
	
	for (;;) {
		ReadDelta(hfile, &lpEvent->dwDelta); // デルタタイムを読み込む
		
		ReadFile(hfile, &lpEvent->state, 1, &dwResult, NULL); // ステータスバイトを読み込む
		if (!(lpEvent->state & 0x80)) { // ランニングステータスか
			lpEvent->state = statePrev; // 一つ前のイベントのステータスバイトを代入
			SetFilePointer(hfile, -1, NULL, FILE_CURRENT); // ファイルポインタを一つ戻す
		}
		
		switch (lpEvent->state & 0xF0) { // ステータスバイトを基にどのイベントか判別

		case 0x80:
		case 0x90:
		case 0xA0:
		case 0xB0:
		case 0xE0:
			ReadFile(hfile, &lpEvent->data1, 1, &dwResult, NULL);
			ReadFile(hfile, &lpEvent->data2, 1, &dwResult, NULL);
			break;
		case 0xC0:
		case 0xD0:
			ReadFile(hfile, &lpEvent->data1, 1, &dwResult, NULL);
			lpEvent->data2 = 0;
			break;
		
		case 0xF0:
			if (lpEvent->state == 0xF0) { // SysExイベント
				ReadFile(hfile, &lpEvent->nData, 1, &dwResult, NULL);

				lpEvent->lpData    = (LPBYTE)Alloc(lpEvent->nData + 1); // 先頭の0xF0を含める
				lpEvent->lpData[0] = lpEvent->state; // 可変長データの先頭は0xF0
				ReadFile(hfile, (lpEvent->lpData + 1), lpEvent->nData, &dwResult, NULL);
			
				lpEvent->nData++;
			}
			else if (lpEvent->state == 0xFF) { // メタイベント
				DWORD dw;
				DWORD tmp;
				
				ReadFile(hfile, &lpEvent->type, 1, &dwResult, NULL); // typeの取得

				dw = (DWORD)-1;

				switch (lpEvent->type) {

				case 0x00: dw = 2; break;
				case 0x01:
				case 0x02:
				case 0x03:
				case 0x04:
				case 0x05:
				case 0x06:
				case 0x07: break;
				case 0x20: dw = 1; break; 
				case 0x21: dw = 1; break; 
				case 0x2F: dw = 0; break; // エンドオブトラック
				case 0x51: dw = 3; break; // セットテンポ
				case 0x54: dw = 5; break;
				case 0x58: dw = 4; break;
				case 0x59: dw = 2; break;
				case 0x7F: break;

				default:
					MessageBox(NULL, TEXT("存在しないメタイベントです。"), NULL, MB_ICONWARNING);
					return FALSE;

				}
				
				tmp = dw;

				if (dw != -1) { // データ長は固定か
					ReadDelta(hfile, &dw);
					if (dw != tmp) {
						MessageBox(NULL, TEXT("固定長メタイベントのデータ長が不正です。"), NULL, MB_ICONWARNING);
						return FALSE;
					}
				}
				else 
					ReadDelta(hfile, &dw); // 任意のデータ長を取得

				lpEvent->nData  = dw;
				lpEvent->lpData = (LPBYTE)Alloc(lpEvent->nData);
				ReadFile(hfile, lpEvent->lpData, lpEvent->nData, &dwResult, NULL); // データの取得
				
				if (lpEvent->type == 0x2F) // トラックの終端
					return TRUE;
			}
			else
				;

			break;

		default:
			MessageBox(NULL, TEXT("ステータスバイト不正"), NULL, MB_ICONWARNING);
			return FALSE;

		}
		
		statePrev = lpEvent->state;
		
		lpEvent->lpNext = (LPEVENT)Alloc(sizeof(EVENT)); // 次のイベントのためにメモリを確保
		lpEvent         = lpEvent->lpNext;
		if (lpEvent == NULL)
			break;
	}

	return FALSE;
}

static LPEVENT Marge(LPEVENT *lplpEvent, int nTruck)
{
	int     i;
	int     nIndex;         // トラックのインデックス
	DWORD   dwAbsolute;     // 絶対時間
	DWORD   dwPrevAbsolute; // 一つ前の絶対時間
	LPEVENT lpHead;         // 新しい一連のイベントの先頭を指す
	LPEVENT lpEvent;        // 現在のイベント
	LPDWORD lpdwTotal;      // 各トラックの絶対時間

	lpHead = (LPEVENT)Alloc(sizeof(EVENT));
	
	lpEvent = lpHead;

	dwPrevAbsolute = 0;
	
	lpdwTotal = (LPDWORD)Alloc(sizeof(DWORD) * nTruck);

	for (;;) {
		nIndex     = -1;
		dwAbsolute = (DWORD)-1; // 0xFFFFFFFF

		for (i = 0; i < nTruck; i++) {
			if (lplpEvent[i]->lpNext == NULL) // トラックの終端まで走査した
				continue;

			if (lpdwTotal[i] + lplpEvent[i]->dwDelta < dwAbsolute) { // 最も絶対時間が低いイベントを見つける
				nIndex = i; // イベントがどのトラックのものかを識別するため
				dwAbsolute = lpdwTotal[i] + lplpEvent[i]->dwDelta;
			}
		}

		if (nIndex == -1) // 全てのトラックを走査した
			break;

		lpEvent->state   = lplpEvent[nIndex]->state;
		lpEvent->data1   = lplpEvent[nIndex]->data1;
		lpEvent->data2   = lplpEvent[nIndex]->data2;
		lpEvent->type    = lplpEvent[nIndex]->type;
		lpEvent->nData   = lplpEvent[nIndex]->nData;
		lpEvent->dwDelta = dwAbsolute - dwPrevAbsolute;

		if (lpEvent->nData != 0) {
			lpEvent->lpData = (LPBYTE)Alloc(lpEvent->nData);
			CopyMemory(lpEvent->lpData, lplpEvent[nIndex]->lpData, lpEvent->nData);
		}
		
		dwPrevAbsolute = dwAbsolute;
		
		lpdwTotal[nIndex] += lplpEvent[nIndex]->dwDelta; // 各トラックの絶対時間を更新

		lplpEvent[nIndex] = lplpEvent[nIndex]->lpNext;
		
		lpEvent->lpNext = (LPEVENT)Alloc(sizeof(EVENT));
		lpEvent         = lpEvent->lpNext;
	}

	return lpHead;
}

static BOOL OpenDialog(HWND hwnd, LPTSTR lpszFile)
{
	OPENFILENAME ofn = {0};

	lpszFile[0] = '\0'; // 要初期化

	ofn.Flags       = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.nMaxFile    = MAX_PATH;
	ofn.hwndOwner   = hwnd;
	ofn.lpstrFile   = lpszFile;
	ofn.lpstrTitle  = TEXT("MIDIファイル読み込み");
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.lpstrFilter = TEXT("MIDI File (*.mid)\0*.mid\0\0");
	
	if (!GetOpenFileName(&ofn))
		return FALSE;

	return TRUE;
}

static void ReadDelta(HANDLE hfile, LPDWORD lpdwDelta)
{
	int   i;
	BYTE  tmp;
	DWORD dwResult;
	
	*lpdwDelta = 0;

	for (i = 0; i < sizeof(DWORD); i++) {
		ReadFile(hfile, &tmp, 1, &dwResult, NULL);

		*lpdwDelta = ( (*lpdwDelta) << 7 ) | (tmp & 0x7F);

		if (!(tmp & 0x80)) // MSBが立っていないならば、次のバイトはデルタタイムではないので抜ける
			break;
	}
}

static void Reverse(LPVOID lpData, int nSize)
{
	int    i;
	BYTE   tmp;
	LPBYTE lp     = (LPBYTE)lpData;
	LPBYTE lpTail = lp + nSize - 1;
	
	for (i = 0; i < nSize / 2; i++) {
		tmp = *lp;
		*lp = *lpTail;
		*lpTail = tmp;

		lp++;
		lpTail--;
	}
}

static LPVOID Alloc(int nSize)
{
	return HeapAlloc(hheap, HEAP_ZERO_MEMORY, nSize);
}

extern HINSTANCE winGameInstance;
extern HWND winGameWindow;

void H_InitMovie(void)
{
		UINT     uId = MIDI_MAPPER;
		MMRESULT mr;

		mr = midiStreamOpen(&hms, &uId, 1, (DWORD)winGameWindow, 0, CALLBACK_WINDOW);
		
		MIDIOUTCAPS caps;
		midiOutGetDevCaps(uId,&caps,sizeof(MIDIOUTCAPS));
printf("midiOutGetDevCaps caps.dwSupport %08X\n",caps.dwSupport);
		if (caps.dwSupport & MIDICAPS_VOLUME) {
printf("midiOutSetVolume\n");
			midiOutSetVolume((HMIDIOUT)hms,0x60006000);
			//midiOutSetVolume((HMIDIOUT)hms,0xffffffff);
		}
	//	return mr == MMSYSERR_NOERROR ? 0 : -1;
}

void H_MovieIdle()
{
}

void H_MovieIdleWin(WPARAM wParam, LPARAM lParam)
{
	if (nStreamReset != 0) // midiStreamStopで返ってきた
		nStreamReset--;
	else {
		LPMIDIHDR lpMhdr = (LPMIDIHDR)lParam;
		midiOutUnprepareHeader((HMIDIOUT)wParam, lpMhdr, sizeof(MIDIHDR));
		
		if (lpMhdr->dwUser == (DWORD)(nStreamCount - 1))
			PlayStream(FALSE);
	}
}

void H_OpenMovieFile(const char *fname,bool loop)
{
	HANDLE hfile;
	
	bgm_loop = loop;

printf("H_OpenMovieFile %s\n",fname);

	Destroy();
	
	hheap = HeapCreate(0, 4096, 0);
	if (hheap == NULL)
		return;

	hfile = CreateFile(fname, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hfile == INVALID_HANDLE_VALUE) {
		//MessageBox(NULL, TEXT("ファイルのオープンに失敗しました。"), NULL, MB_ICONWARNING);
printf("open error\n");
		return;
	}

	if (!LoadFile(hfile)) {
		//MessageBox(NULL, TEXT("MIDIファイルの読み込みに失敗しました。"), NULL, MB_ICONWARNING);
printf("open error\n");
		CloseHandle(hfile);
		return;
	}
	
	CloseHandle(hfile);
	
	nStreamCount = GetStreamCount();
printf("nStreamCount %d\n",nStreamCount);
	
	SetStream();
	
	PlayStream(TRUE);
	
	bPlayMusic = TRUE;
}

void H_CloseMovie(void)
{
	Destroy();
}

#else
#include <windows.h>
#include <vfw.h>

static HWND	hwMci;
static bool MoviePlayFlag;
extern HINSTANCE winGameInstance;
extern HWND winGameWindow;

void H_InitMovie(void)
{
	MoviePlayFlag = false;
}

void H_MovieIdle()
{
	if (MoviePlayFlag) {
		if (bgm_loop) {
			long lEndPosition = MCIWndGetEnd(hwMci);
			long lCurrentPosition = MCIWndGetPosition(hwMci);
			if( lEndPosition <= lCurrentPosition ) {
				MCIWndPlayFrom(hwMci,0);
			}
		}
	}
}

void H_MovieIdleWin(WPARAM wParam, LPARAM lParam)
{
}

void H_OpenMovieFile(const char *fname,bool loop)
{
printf("H_OpenMovieFile %s\n",fname);
	if (hwMci) MCIWndStop(hwMci);
	hwMci=MCIWndCreate(winGameWindow,winGameInstance,MCIWNDF_NOPLAYBAR|MCIWNDF_NOPLAYBAR|MCIWNDF_NOMENU|MCIWNDF_NOOPEN,NULL);
	ShowWindow(hwMci,SW_HIDE);
	MCIWndOpen(hwMci,fname,0);
	MCIWndStop(hwMci);
	MCIWndSeek(hwMci,MCIWND_START);
	MCIWndPlay(hwMci);
	MoviePlayFlag = true;
	bgm_loop = loop;
}

void H_CloseMovie(void)
{
}

#endif

#endif
