#ifndef __QBSOUNDWIN_H__
#define __QBSOUNDWIN_H__

#include "QBSound.h"
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <mmsystem.h>

enum tagCONST {
	NumWaveBuff = 4,
	BUFFSIZE = 12288,
	BUFFSIZEVISTA = (1024*19)
};

class QBSoundWin : public QBSound {
public:
	QBSoundWin(int maxChannel);
	virtual ~QBSoundWin() {}

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
	virtual void streamLoad();
	virtual int setVolume(int track,float volume=1.0);

	void playSound();

	MMRESULT openWave();
	MMRESULT prepareWave(long cur);
	MMRESULT unprepareWave(long cur);
	MMRESULT writeWave(int cur);
	MMRESULT resetWave();
	MMRESULT closeWave();

	HWAVEOUT m_hWavePlay;
	WAVEFORMATEX m_wfxWaveForm;

	LPSTR m_pWaveBuff[NumWaveBuff];
	LPWAVEHDR m_pWaveHdr[NumWaveBuff];
	DWORD m_dwUser;
	long m_nCurBuff;
	long m_nCurPBuff;
	bool stopWave;
	float theta;

	CRITICAL_SECTION	mCriticalSection;

	static int BufferSize;
	static int BufferSizeVista;
};

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
};
#endif

#endif //__QBSOUNDWIN_H__
