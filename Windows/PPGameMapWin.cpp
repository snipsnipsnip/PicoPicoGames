#include "PPGameMap.h"
#include "QBNodeSystemPlist.h"

PPGameMap::PPGameMap(const char* path)
{
	QBNodePool* pool = QBNodePoolAlloc();
	if (pool) {
		FILE* fp = fopen(path,"r");
		fseek(fp,0,SEEK_END);
		int size = ftell(fp);
		fseek(fp,0,SEEK_SET);
		char* str = (char*)malloc(size);
		fread(str,size,1,fp);
		fclose(fp);
		QBNode* root = PlistToQBNode(pool,str);
		QBNodeDescription(root);
		free(str);
		
		map[0].init();
		map[1].init();
		map[2].init();
		map[0].decompress(QBNodeBytesWithKeyPath(root,".bg0"));
		map[1].decompress(QBNodeBytesWithKeyPath(root,".bg1"));
		map[2].decompress(QBNodeBytesWithKeyPath(root,".bg2"));
		
		QBNodePoolFree(pool);
	}
}

PPGameMap::~PPGameMap()
{
	map[0].freedata();
	map[1].freedata();
	map[2].freedata();
}
