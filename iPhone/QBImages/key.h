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

#include "PPGameDef.h"

#define PG_KEY_SIZE 15

#ifdef _PG_GLOBAL_
//  fileno,    x,    y,    w,    h,   dx,   dy,   tx,   ty
static short key[15][9] = {
    {    0,    4,  238,   71,   70,    8,  398,   96,    0},//0(5)
    {    1,    6,  160,   69,   70,  245,  399,   96,   70},//1(6)
    {    2,  410,  239,   70,   70,  410,  239,  167,    0},//2(2)
    {    3,    6,  160,   69,   70,  411,  160,   96,   70},//3(6)
    {    4,   18,  209,   96,   97,   18,  209,    0,    0},//4(4)
    {    5,    4,  238,   71,   70,    4,  238,   96,    0},//5(5)
    {    6,    6,  160,   69,   70,    6,  160,   96,   70},//6(6)
    {    7,   18,  209,   96,   97,  370,  209,    0,    0},//7(4)
    {    8,    4,  238,   71,   70,  249,  396,   96,    0},//8(5)
    {    9,    6,  160,   69,   70,  160,  397,   96,   70},//9(6)
    {   10,   18,  209,   96,   97,   16,  383,    0,    0},//10(4)
    {   11,    4,  238,   71,   70,    5,  397,   96,    0},//11(5)
    {   12,    6,  160,   69,   70,   95,  398,   96,   70},//12(6)
    {   13,   18,  209,   96,   97,  210,  383,    0,    0},//13(4)
    {   14,   18,  209,   96,   97,  112,  383,    0,    0},//14(4)
};

PPGameImage key_group[]={
    {key[   0],320,480,1,320,480},		//button_1_320x480.png
    {key[   1],320,480,1,320,480},		//button_2_320x480.png
    {key[   2],480,320,1,480,320},		//button_HL1_480x320.png
    {key[   3],480,320,1,480,320},		//button_HL2_480x320.png
    {key[   4],480,320,1,480,320},		//button_HLc_480x320.png
    {key[   5],480,320,1,480,320},		//button_HR1_480x320.png
    {key[   6],480,320,1,480,320},		//button_HR2_480x320.png
    {key[   7],480,320,1,480,320},		//button_HRc_480x320.png
    {key[   8],320,480,1,320,480},		//button_VL1_320x480.png
    {key[   9],320,480,1,320,480},		//button_VL2_320x480.png
    {key[  10],320,480,1,320,480},		//button_VLc_320x480.png
    {key[  11],320,480,1,320,480},		//button_VR1_320x480.png
    {key[  12],320,480,1,320,480},		//button_VR2_320x480.png
    {key[  13],320,480,1,320,480},		//button_VRc_320x480.png
    {key[  14],320,480,1,320,480},		//button_c_320x480.png
    {NULL,0,0,0,0,0}
};
#else
extern PPGameImage key_group[];
#endif

enum {
    PG_BUTTON_1,
    PG_BUTTON_2,
    PG_BUTTON_HL1,
    PG_BUTTON_HL2,
    PG_BUTTON_HLC,
    PG_BUTTON_HR1,
    PG_BUTTON_HR2,
    PG_BUTTON_HRC,
    PG_BUTTON_VL1,
    PG_BUTTON_VL2,
    PG_BUTTON_VLC,
    PG_BUTTON_VR1,
    PG_BUTTON_VR2,
    PG_BUTTON_VRC,
    PG_BUTTON_C,
};
