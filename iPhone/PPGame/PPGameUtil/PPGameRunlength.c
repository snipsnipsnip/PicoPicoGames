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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "PPGameRunlength.h"
//#include "GAMESYS.h"

#define ALIAN_CHG_SHORT(x) (x)
/*#define ALIAN_CHG_SHORT(x) (((((unsigned short)x) & 0xff) << 8) | ((((unsigned short)x) & 0xff00) >> 8))*/
//#define ABtoL(x) (((x & 0xff000000) >> 24) | ((x & 0x00ff0000) >> 8) | ((x & 0x0000ff00) << 8) | ((x & 0x000000ff) << 24))
#define ABtoL(x) (x)

void *PPGameRunlength_Compress(unsigned short *ptr,long *size)
{
	unsigned short *buf;
	long i,j;
	unsigned short pat;
	unsigned short patct;
	int step;

	buf = (unsigned short *)calloc(1,*size+4096);

	j = 0;
	step = 0;
	((long *)buf)[0] = *size;	
	j += 4;

	for (i=0;i<*size/sizeof(short);i++) {
		switch (step) {
		case 0:
			pat = ptr[i];
			patct = 1;
			step = 1;
			break;
		case 1:
			if (ptr[i] == pat) {
				patct ++;
			} else {
				if (patct > 1) {
					unsigned short tmp;
					tmp = patct | 0x8000;
					tmp = ALIAN_CHG_SHORT(tmp);
					buf[j++] = tmp;
				}
				buf[j++] = pat;
				pat = ptr[i];
				patct = 1;
			}
			break;
		}
	}
	if (patct > 1) {
		unsigned short tmp;
		tmp = patct | 0x8000;
		tmp = ALIAN_CHG_SHORT(tmp);
		buf[j++] = tmp;
	}
	buf[j++] = pat;
	if (j % 2) j ++;
	*size = j*sizeof(short);
	((long *)buf)[1] = *size;	
	return buf;
}

void *PPGameRunlength_Decompress(unsigned short *ptr,long *datsize)
{
	unsigned short *buf;
	long fsize;
	long size;
	long i,j,k;
	unsigned short pat;
	unsigned short patct;

	fsize = ((long *)ptr)[1];
/*
{
printf("size %d\n",(int)size);
	unsigned char* p = (unsigned char*)ptr;
	for (int i=0;i<fsize;i++) {
		printf("%02X,",p[i]);
		if ((i % 16) == 15) printf("\n");
	}
}
*/	
	size = *datsize = ((long *)ptr)[0];
	
	buf = (unsigned short *)malloc(size+4096);
	
	for (i=0,j=4;i<size/sizeof(short);) {
		pat = ptr[j++];
		pat = ALIAN_CHG_SHORT(pat);
		if (pat & 0x8000) {
			patct = pat & (~0x8000);
			pat = ptr[j++];
			for (k=0;k<patct;k++) {
				buf[i++] = pat;
				if (i >= size/sizeof(short)) {
					break;
				}
			}
		} else {
			buf[i++] = ALIAN_CHG_SHORT(pat);
		}
		if (i >= size/sizeof(short)) {
			break;
		}
	}
	return buf;
}
