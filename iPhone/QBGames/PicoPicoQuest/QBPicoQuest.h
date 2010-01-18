#ifndef __QBPICOQUEST_H__
#define __QBPICOQUEST_H__

#include "QBGame.h"

enum {
	QBCHAR_PICOQUEST,
	QBCHAR_FONT,
	QBCHAR_KEY,
};

class QBPicoQuest : public QBGame {
public:
	QBPicoQuest();
	virtual ~QBPicoQuest();
	virtual int Idle();
	
	void Put(int x,int y,int pat);
	
	int mapx,mapy;
};

#endif
