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

#define PG_BLUEBALLCHAR_SIZE 128

#ifdef _PG_GLOBAL_
//  fileno,    x,    y,    w,    h,   dx,   dy,   tx,   ty
static short BlueBallChar[128][9] = {
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    4,  100,   12,   18,    4,    0,  100,   20},//80(80)
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,  120,   20,   20,    0,    0,    0,    0},//96(96)
    {    0,   20,  120,   20,   20,    0,    0,   20,    0},//97(97)
    {    0,   40,  120,   20,   20,    0,    0,   40,    0},//98(98)
    {    0,   60,  120,   20,   20,    0,    0,   60,    0},//99(99)
    {    0,   80,  120,   20,   20,    0,    0,   80,    0},//100(100)
    {    0,  100,  120,   20,   20,    0,    0,  100,    0},//101(101)
    {    0,  120,  120,   20,   20,    0,    0,  120,    0},//102(102)
    {    0,  140,  120,   20,   20,    0,    0,  140,    0},//103(103)
    {    0,   40,  120,   20,   20,    0,    0,   40,    0},//104(98)
    {    0,  180,  120,   20,   20,    0,    0,  160,    0},//105(105)
    {    0,  200,  120,   20,   20,    0,    0,  180,    0},//106(106)
    {    0,  220,  120,   20,   20,    0,    0,  200,    0},//107(107)
    {    0,  240,  120,   20,   20,    0,    0,  220,    0},//108(108)
    {    0,  260,  120,   20,   20,    0,    0,    0,   20},//109(109)
    {    0,  280,  120,   20,   20,    0,    0,   20,   20},//110(110)
    {    0,  300,  120,   20,   20,    0,    0,   40,   20},//111(111)
    {    0,    0,  140,   20,   20,    0,    0,   60,   20},//112(112)
    {    0,   20,  140,   20,   20,    0,    0,   80,   20},//113(113)
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
};

PPGameImage BlueBallChar_group[]={
    {BlueBallChar[   0],20,20,16,320,160},		//BlueBall_20x20.png
    {NULL,0,0,0,0,0}
};
#else
extern PPGameImage BlueBallChar_group[];
#endif

enum {
    PG_BLUEBALL,
};
