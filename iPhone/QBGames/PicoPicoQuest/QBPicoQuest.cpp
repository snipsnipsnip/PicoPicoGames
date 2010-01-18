#include "QBPicoQuest.h"
#include "shadowChar.h"

QBPicoQuest::QBPicoQuest()
{
}

QBPicoQuest::~QBPicoQuest()
{
}

void QBPicoQuest::Put(int x,int y,int pat)
{
	QBGame::Put(x,y,pat,PG_SHADOWBUSTER,QBCHAR_PICOQUEST);
}

int QBPicoQuest::Idle()
{
	Clear();
	switch (step) {
	case 0:
		LoadMap("QBPicoQuest.plist");
		mapx = 0;
		mapy = 0;
		step ++;
		break;
	case 1:
		{
			for (int i=0;i<3;i++) {
				if (map[i]) {
					for (int y=0;y<map[i]->height;y++) {
						for (int x=0;x<map[i]->width;x++) {
							if (map[i]->map[x+y*map[i]->width]) Put(x*32+mapx*32,y*32+mapy*32,map[i]->map[x+y*map[i]->width]);
						}
					}
				}
			}
		}
		if (KeyWait(PAD_UP)) {
			mapy ++;
		}
		if (KeyWait(PAD_DOWN)) {
			mapy --;
		}
		if (KeyWait(PAD_LEFT)) {
			mapx ++;
		}
		if (KeyWait(PAD_RIGHT)) {
			mapx --;
		}
		break;
	}
	return 0;
}
