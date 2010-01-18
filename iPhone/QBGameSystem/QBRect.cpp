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
#include <math.h>
//#include "QBShotMain.h"
#include "QBRect.h"

#if 0
int main(int argc, char *argv[])
{
	float p1[] = {50,50,100,0,100,100,0,100,50,50};
	float p2[] = {0,0,10+10,3,10+210,10,3+210,10,3+5,3};
	
	if (calc_hitcheck(p1,p2)) {
		printf("hit\n");
	}
	
	return 0;
}
#endif

void QBRect::set_point(float cx,float cy,float r)
{
	calc_rotate(x  -cx,y  -cy,r,&p[0]);
	calc_rotate(x+w-cx,y  -cy,r,&p[2]);
	calc_rotate(x+w-cx,y+h-cy,r,&p[4]);
	calc_rotate(x  -cx,y+h-cy,r,&p[6]);
	p[0] += cx;
	p[1] += cy;
	p[2] += cx;
	p[3] += cy;
	p[4] += cx;
	p[5] += cy;
	p[6] += cx;
	p[7] += cy;
	p[8] = p[0];
	p[9] = p[1];
	calc_bound(p,b);
	rot = r;
}

void QBRect::set_point(float _x,float _y,float _w,float _h,float cx,float cy,float r)
{
	x = _x;
	y = _y;
	w = _w;
	h = _h;
	calc_rotate(x  -cx,y  -cy,r,&p[0]);
	calc_rotate(x+w-cx,y  -cy,r,&p[2]);
	calc_rotate(x+w-cx,y+h-cy,r,&p[4]);
	calc_rotate(x  -cx,y+h-cy,r,&p[6]);
	p[0] += cx;
	p[1] += cy;
	p[2] += cx;
	p[3] += cy;
	p[4] += cx;
	p[5] += cy;
	p[6] += cx;
	p[7] += cy;
	p[8] = p[0];
	p[9] = p[1];
	calc_bound(p,b);
	rot = r;
}

/*
bool QBRect::check(QBRect* rect)
{
	if (hit && rect->hit) {
#if 1
		if (rot == 0 && rect->rot == 0) {
			unsigned long ww = x - rect->x + w;
			unsigned long hh = y - rect->y + h;
			if (ww < w+rect->w && hh < h+rect->h) return true;
			return false;
		}
#endif
		return calc_hitcheck(p,rect->p,rect->b);
	}
	return false;
}
*/

bool QBRect::tri_hitcheck(float* p1,float x,float y)
{
	int i;
	for (i=0;i<3;i++) {
		float d[2];
		d[1] = (p1[i*2+2]-p1[i*2+0]);
		d[0] = -(p1[i*2+3]-p1[i*2+1]);
		//for (j=0;j<4;j++) {
		float n = d[0]*(x-p1[i*2+0])+d[1]*(y-p1[i*2+1]);
//printf("%d:%f:%f:%f:%f:%f\n",i,n,d[0],d[1],x,y);
		if (n < 0) return false;
		//}
	}
	//return false;
	return true;
}

bool QBRect::calc_hitcheck(float* p1,float* p2,float* b2)
{
	//float b1[4];
	//float b2[4];
	//calc_bound(p1,b1);
	//calc_bound(p2,b2);
	if (calc_rectcheck(b,b2)) 
	{
		int i,j;
		/*線分の接触判定/*/
#if 0
		i = 0;
		j = 0;
				if (calc_linecross(&p1[i*2],&p2[j*2])) {
//printf("cross\n");
					return true;
				}
#else
		for (i=0;i<4;i++) {
			for (j=0;j<4;j++) {
				if (calc_linecross(&p1[i*2],&p2[j*2])) {
//printf("cross\n");
					return true;
				}
			}
		}
#endif
#if 1
		/*頂点の内包判定/*/
		for (i=0;i<4;i++) {
			float d[2];
			d[0] = p1[i*2+2]-p1[i*2+0];
			d[1] = p1[i*2+3]-p1[i*2+1];
			for (j=0;j<4;j++) {
				float n = d[0]*(p2[j*2+0]-p1[i*2+0])+d[1]*(p2[j*2+1]-p1[i*2+1]);
//printf("%d,%d,%lf\n",i,j,n);
				if (n < 0) return false;
			}
		}
		return true;
#endif
	}
	return false;
}

bool QBRect::calc_rectcheck(float* r1,float* r2)
{
	float w1 = r1[2]-r1[0];
	float w2 = r2[2]-r2[0];
	float h1 = r1[3]-r1[1];
	float h2 = r2[3]-r2[1];
	float ww = r2[0]-r1[0]+w2;
	float hh = r2[1]-r1[1]+h2;
	if (ww >= 0 && ww < w1+w2 && hh >= 0 && hh < h1+h2) {
		return true;
	}
	return false;
}

void QBRect::calc_bound(float* p,float* o)
{
	o[0] = p[0];
	o[1] = p[1];
	o[2] = p[0];
	o[3] = p[1];
	for (int i=1;i<4;i++) {
		if (o[0] > p[i*2+0]) o[0] = p[i*2+0];
		if (o[1] > p[i*2+1]) o[1] = p[i*2+1];
		if (o[2] < p[i*2+0]) o[2] = p[i*2+0];
		if (o[3] < p[i*2+1]) o[3] = p[i*2+1];
	}
//printf("%f,%f,%f,%f\n",o[0],o[1],o[2],o[3]);
}

void QBRect::calc_rotate(float x,float y,float r,float* o)
{
	if (r != 0) {
		o[0] = x*cos(r)-y*sin(r);
		o[1] = x*sin(r)+y*cos(r);
	} else {
		o[0] = x;
		o[1] = y;
	}
}

bool QBRect::calc_linecross(float* l1,float* l2)
{
	float sp[2];
	float v1[2];
	float v2[2];
	float sn[2];

	sp[0] = l1[2];//-l1[0];
	sp[1] = l1[3];//-l1[1];

	v1[0] = l2[0];//-l1[0];
	v1[1] = l2[1];//-l1[1];

	v2[0] = l2[2];//-l1[0];
	v2[1] = l2[3];//-l1[1];

	sn[0] = l1[3]-l1[1];
	sn[1] = -(l1[2]-l1[0]);

	//float l = sqrt(sn[0]*sn[0]+sn[1]*sn[1]);
	//sn[0] /= l;
	//sn[1] /= l;
	
	float d0,d1,d2,t;
	d0 = sp[0]*sn[0]+sp[1]*sn[1];
	d1 = v1[0]*sn[0]+v1[1]*sn[1];
	d2 = v2[0]*sn[0]+v2[1]*sn[1];

//printf("d0 %f d1 %f d2 %f\n",d0,d1,d2);

	if (d1 == d2) return false;
	t = (d0 - d2) / (d1 - d2);

	if ((d0 > d1 && d0 > d2) || (d0 <= d1 && d0 <= d2)) {
		return false;
	}
	if (t >= 0 && t < 1.0) {
		float x = l2[2]+t*(l2[0]-l2[2]);
		float y = l2[3]+t*(l2[1]-l2[3]);
#if 0
		{
			unsigned char colors[4] = {0,0,255,255};
			unsigned char colors2[4] = {255,0,255,255};
			colors[3] = 255;
			G(SetTriPoly(x*2-4,y*2-4,0,colors));
			G(SetTriPoly(x*2+4,y*2-4,0,colors));
			G(SetTriPoly(x*2-4,y*2+4,0,colors));
			G(SetTriPoly(x*2+4,y*2+4,0,colors));
			G(EndTriPoly());

			G(SetTriPoly(l1[0]*2-4,l1[1]*2-4,0,colors));
			G(SetTriPoly(l1[0]*2+4,l1[1]*2-4,0,colors));
			G(SetTriPoly(l1[0]*2-4,l1[1]*2+4,0,colors));
			G(SetTriPoly(l1[0]*2+4,l1[1]*2+4,0,colors));
			G(EndTriPoly());

			G(SetTriPoly(l1[2]*2-4,l1[3]*2-4,0,colors2));
			G(SetTriPoly(l1[2]*2+4,l1[3]*2-4,0,colors2));
			G(SetTriPoly(l1[2]*2-4,l1[3]*2+4,0,colors2));
			G(SetTriPoly(l1[2]*2+4,l1[3]*2+4,0,colors2));
			G(EndTriPoly());


			G(SetTriPoly(l2[0]*2-4,l2[1]*2-4,0,colors));
			G(SetTriPoly(l2[0]*2+4,l2[1]*2-4,0,colors));
			G(SetTriPoly(l2[0]*2-4,l2[1]*2+4,0,colors));
			G(SetTriPoly(l2[0]*2+4,l2[1]*2+4,0,colors));
			G(EndTriPoly());

			G(SetTriPoly(l2[2]*2-4,l2[3]*2-4,0,colors2));
			G(SetTriPoly(l2[2]*2+4,l2[3]*2-4,0,colors2));
			G(SetTriPoly(l2[2]*2-4,l2[3]*2+4,0,colors2));
			G(SetTriPoly(l2[2]*2+4,l2[3]*2+4,0,colors2));
			G(EndTriPoly());
		}
#endif

		float dx = 0;
		if (l1[2]-l1[0] != 0) dx = (x-l1[0])/(l1[2]-l1[0]);
		float dy = 0;
		if (l1[3]-l1[1] != 0) dy = (y-l1[1])/(l1[3]-l1[1]);

//printf("%f,%f\n",dx,dy);
		
		if (dx >= 0 && dx < 1.0 && dy >= 0 && dy < 1.0) {
			return true;
		}
	}
	return false;
}

#if 0
float H_InnerProduct(H_VECTOR *v1,H_VECTOR *v2)
{
	return v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
}

int H_CalcClossPoint(H_VECTOR *sp,H_VECTOR *sn,H_VECTOR *v1,H_VECTOR *v2,H_VECTOR *a)
{
	float d0,d1,d2,t;
	d0 = H_InnerProduct(sp,sn);
	d1 = H_InnerProduct(v1,sn);
	d2 = H_InnerProduct(v2,sn);
	if (d1 == d2) return FALSE;
	t = (d0 - d2) / (d1 - d2);
	a->x = v2->x + t * (v1->x - v2->x);
	a->y = v2->y + t * (v1->y - v2->y);
	a->z = v2->z + t * (v1->z - v2->z);
	if ((d0 > d1 && d0 > d2) || (d0 <= d1 && d0 <= d2))
		return FALSE;
	return TRUE;
}
#endif
