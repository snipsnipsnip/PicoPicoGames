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

#pragma once

#define PPGAME_KEY_SIDE_RIGHT "key_side_right"
#define PPGAME_KEY_SIDE_LEFT "key_side_left"

#ifdef __cplusplus
extern "C" {
#endif

const char* PPGameResourcePath(const char* name);
int PPGame_SetDefault(const char* name);
int PPGame_GetDefault(const char* name);
int PPGame_GetParam(const char* key,int defaultValue);
void PPGame_SetParam(const char* key,int value);
unsigned char* PPGame_GetData(const char* key,int* dataSize);
void PPGame_SetData(const char* key,unsigned char* data,int dataSize);
unsigned char* PPGame_LoadPNG(const char* path,unsigned long* width,unsigned long* height,unsigned long* bytesPerRow);

int PPGame_InitBGM(int no,const char* key);
void PPGame_ReleaseBGM();
void PPGame_IdleBGM(void* controller,int playBGM,bool playBGMOneTime,int chooseBGM);

int PPGame_GetLocale();

#ifdef _WIN32
#define M_PI		3.14159265358979323846
#endif

#ifdef __cplusplus
};
#endif
