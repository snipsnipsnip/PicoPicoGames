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

#define PG_CARRACE_SIZE 20

#ifdef _PG_GLOBAL_
//  fileno,    x,    y,    w,    h,   dx,   dy,   tx,   ty
static short carrace[20][9] = {
    {    0,    0,    0,   32,   32,    0,    0,    0,    0},//0(0)
    {    0,   32,    0,   32,   32,    0,    0,   32,    0},//1(1)
    {    0,   64,    0,   32,   32,    0,    0,   64,    0},//2(2)
    {    0,   96,    0,   32,   32,    0,    0,   96,    0},//3(3)
    {    0,  128,    0,   32,   32,    0,    0,  128,    0},//4(4)
    {    0,  160,    0,   32,   32,    0,    0,  160,    0},//5(5)
    {    0,  192,    0,   32,   32,    0,    0,  192,    0},//6(6)
    {    0,  224,    0,   32,   32,    0,    0,  224,    0},//7(7)
    {    0,  256,    0,   32,   32,    0,    0,    0,   32},//8(8)
    {    0,  288,    0,   32,   32,    0,    0,   32,   32},//9(9)
    {    0,    0,   32,   32,   32,    0,    0,   64,   32},//10(10)
    {    0,   32,   32,   32,   32,    0,    0,   96,   32},//11(11)
    {    0,   64,   52,   32,    8,    0,   20,  160,   44},//12(12)
    {    0,   96,   52,   32,   12,    0,   20,  192,   32},//13(13)
    {    0,  128,   60,   32,    4,    0,   28,  224,   48},//14(14)
    {    0,  160,   48,   32,   12,    0,   16,  160,   32},//15(15)
    {    0,  192,   44,   32,    8,    0,   12,  224,   32},//16(16)
    {    0,  224,   40,   32,    8,    0,    8,  224,   40},//17(17)
    {    0,  256,   36,   32,    4,    0,    4,  192,   48},//18(18)
    {    0,  288,   36,   32,   20,    0,    4,  128,   32},//19(19)
};

PPGameImage carrace_group[]={
    {carrace[   0],32,32,10,320,64},		//CarRace_32x32.png
    {NULL,0,0,0,0,0}
};
#else
extern PPGameImage carrace_group[];
#endif

enum {
    PG_CARRACE,
};
