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

#define PG_SHADOWCHAR_SIZE 88

#ifdef _PG_GLOBAL_
//  fileno,    x,    y,    w,    h,   dx,   dy,   tx,   ty
static short shadowChar[88][9] = {
    {    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,   32,    0,   32,   32,    0,    0,    0,    0},//1(1)
    {    0,   64,    0,   32,   32,    0,    0,   32,    0},//2(2)
    {    0,   96,    0,   32,   32,    0,    0,   64,    0},//3(3)
    {    0,  128,    0,   32,   32,    0,    0,   96,    0},//4(4)
    {    0,  160,    0,   32,   32,    0,    0,  128,    0},//5(5)
    {    0,  192,    0,   32,   32,    0,    0,  160,    0},//6(6)
    {    0,  224,    0,   32,   32,    0,    0,  192,    0},//7(7)
    {    0,  256,    0,   32,   32,    0,    0,  224,    0},//8(8)
    {    0,  288,    0,   32,   32,    0,    0,    0,   32},//9(9)
    {    0,  288,    0,   32,   32,    0,    0,    0,   32},//10(9)
    {    0,  288,    0,   32,   32,    0,    0,    0,   32},//11(9)
    {    0,  288,    0,   32,   32,    0,    0,    0,   32},//12(9)
    {    0,  288,    0,   32,   32,    0,    0,    0,   32},//13(9)
    {    0,  288,    0,   32,   32,    0,    0,    0,   32},//14(9)
    {    0,  288,    0,   32,   32,    0,    0,    0,   32},//15(9)
    {    0,  288,    0,   32,   32,    0,    0,    0,   32},//16(9)
    {    0,  288,    0,   32,   32,    0,    0,    0,   32},//17(9)
    {    0,  288,    0,   32,   32,    0,    0,    0,   32},//18(9)
    {    0,  288,    0,   32,   32,    0,    0,    0,   32},//19(9)
    {    0,  288,    0,   32,   32,    0,    0,    0,   32},//20(9)
    {    0,  288,    0,   32,   32,    0,    0,    0,   32},//21(9)
    {    0,    0,   64,   32,   32,    0,    0,   32,   32},//22(22)
    {    0,   32,   64,   32,   32,    0,    0,   64,   32},//23(23)
    {    0,   64,   64,   32,   32,    0,    0,   96,   32},//24(24)
    {    0,   96,   64,   32,   32,    0,    0,  128,   32},//25(25)
    {    0,  128,   64,   32,   32,    0,    0,  160,   32},//26(26)
    {    0,  160,   64,   32,   32,    0,    0,  192,   32},//27(27)
    {    0,  192,   64,   32,   32,    0,    0,  224,   32},//28(28)
    {    0,  224,   64,   32,   32,    0,    0,    0,   64},//29(29)
    {    0,  256,   64,   32,   32,    0,    0,   32,   64},//30(30)
    {    0,  288,   64,   32,   32,    0,    0,   64,   64},//31(31)
    {    0,  320,   64,   32,   32,    0,    0,   96,   64},//32(32)
    {    0,    0,   96,   32,   32,    0,    0,  128,   64},//33(33)
    {    0,   32,   96,   32,   32,    0,    0,  160,   64},//34(34)
    {    0,   64,   96,   32,   32,    0,    0,  192,   64},//35(35)
    {    0,  160,    0,   32,   32,    0,    0,  128,    0},//36(5)
    {    0,   64,    0,   32,   32,    0,    0,   32,    0},//37(2)
    {    0,  128,    0,   32,   32,    0,    0,   96,    0},//38(4)
    {    0,   96,    0,   32,   32,    0,    0,   64,    0},//39(3)
    {    0,  288,    0,   32,   32,    0,    0,    0,   32},//40(9)
    {    0,  288,    0,   32,   32,    0,    0,    0,   32},//41(9)
    {    0,  288,    0,   32,   32,    0,    0,    0,   32},//42(9)
    {    0,  288,    0,   32,   32,    0,    0,    0,   32},//43(9)
    {    0,    0,  128,   32,   32,    0,    0,  224,   64},//44(44)
    {    0,   32,  128,   32,   32,    0,    0,    0,   96},//45(45)
    {    0,   64,  128,   32,   32,    0,    0,   32,   96},//46(46)
    {    0,   96,  128,   32,   32,    0,    0,   64,   96},//47(47)
    {    0,  128,  128,   32,   32,    0,    0,   96,   96},//48(48)
    {    0,  160,  128,   32,   32,    0,    0,  128,   96},//49(49)
    {    0,  192,  128,   32,   32,    0,    0,  160,   96},//50(50)
    {    0,  224,  128,   32,   32,    0,    0,  192,   96},//51(51)
    {    0,  256,  128,   32,   32,    0,    0,  224,   96},//52(52)
    {    0,  288,  128,   32,   32,    0,    0,    0,  128},//53(53)
    {    0,  320,  128,   32,   32,    0,    0,   32,  128},//54(54)
    {    0,    0,  160,   32,   32,    0,    0,   64,  128},//55(55)
    {    0,   32,  160,   32,   32,    0,    0,   96,  128},//56(56)
    {    0,   64,  160,   32,   32,    0,    0,  128,  128},//57(57)
    {    0,   96,  160,   32,   32,    0,    0,  160,  128},//58(58)
    {    0,  128,  160,   32,   32,    0,    0,  192,  128},//59(59)
    {    0,  160,  160,   32,   32,    0,    0,  224,  128},//60(60)
    {    0,  192,  160,   32,   32,    0,    0,    0,  160},//61(61)
    {    0,  224,  160,   32,   32,    0,    0,   32,  160},//62(62)
    {    0,  256,  160,   32,   32,    0,    0,   64,  160},//63(63)
    {    0,  288,  160,   32,   32,    0,    0,   96,  160},//64(64)
    {    0,  320,  160,   32,   32,    0,    0,  128,  160},//65(65)
    {    0,   64,  128,   32,   32,    0,    0,   32,   96},//66(46)
    {    0,  192,  128,   32,   32,    0,    0,  160,   96},//67(50)
    {    0,   64,  160,   32,   32,    0,    0,  128,  128},//68(57)
    {    0,  288,    0,   32,   32,    0,    0,    0,   32},//69(9)
    {    0,  288,    0,   32,   32,    0,    0,    0,   32},//70(9)
    {    0,  288,    0,   32,   32,    0,    0,    0,   32},//71(9)
    {    0,  288,    0,   32,   32,    0,    0,    0,   32},//72(9)
    {    0,  288,    0,   32,   32,    0,    0,    0,   32},//73(9)
    {    0,  288,    0,   32,   32,    0,    0,    0,   32},//74(9)
    {    0,  288,    0,   32,   32,    0,    0,    0,   32},//75(9)
    {    0,  288,    0,   32,   32,    0,    0,    0,   32},//76(9)
    {    0,    0,  224,   32,   32,    0,    0,  160,  160},//77(77)
    {    0,   32,  224,   32,   32,    0,    0,  192,  160},//78(78)
    {    0,   64,  224,   32,   32,    0,    0,  224,  160},//79(79)
    {    0,   96,  224,   32,   32,    0,    0,    0,  192},//80(80)
    {    0,  192,    0,   32,   32,    0,    0,  160,    0},//81(6)
    {    0,  160,  224,   32,   32,    0,    0,   32,  192},//82(82)
    {    0,  192,  224,   32,   32,    0,    0,   64,  192},//83(83)
    {    0,  160,    0,   32,   32,    0,    0,  128,    0},//84(5)
    {    0,  256,  224,   32,   32,    0,    0,   96,  192},//85(85)
    {    0,  288,    0,   32,   32,    0,    0,    0,   32},//86(9)
    {    0,  288,    0,   32,   32,    0,    0,    0,   32},//87(9)
};

PPGameImage shadowChar_group[]={
    {shadowChar[   0],32,32,11,352,256},		//ShadowBuster_32x32.png
    {NULL,0,0,0,0,0}
};
#else
extern PPGameImage shadowChar_group[];
#endif

enum {
    PG_SHADOWBUSTER,
};
