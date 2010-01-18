/*
 *  PPGameUtilWin.c
 *  PicoPicoWin
 *
 *  Created by 山口 博光 on 10/01/07.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "PPGameUtil.h"
#include "QBGame.h"
#include "png.h"
#include <stdlib.h>

#define PNG_HEADER_SIZE 8

static int keySide = 0;
const char* QBGameResource(const char* name);
int QBSetDefault(const char* key,char* value);
int QBGetDefault(const char* key,char* value,int length);

const char* PPGameResourcePath(const char* name)
{
	//path is [application]/DATA/[name]
	return QBGameResource(name);
}

int PPGame_SetDefault(const char* name)
{
	if (strcmp(name,PPGAME_KEY_SIDE_RIGHT) == 0) {
		keySide = 1;
	} else
	if (strcmp(name,PPGAME_KEY_SIDE_LEFT) == 0) {
		keySide = 0;
	}
	return keySide;
}

int PPGame_GetDefault(const char* name)
{
	if (strcmp(name,PPGAME_KEY_SIDE_RIGHT) == 0) {
		if (keySide == 1) return 1;
	} else
	if (strcmp(name,PPGAME_KEY_SIDE_LEFT) == 0) {
		if (keySide == 0) return 1;
	}
	return 0;
}

int PPGame_GetParam(const char* key,int defaultValue)
{
	char svalue[1024];
	QBGetDefault(key,svalue,1024);
	if (svalue[0] == 0) {
		return defaultValue;
	}
	return atoi(svalue);
}

void PPGame_SetParam(const char* key,int value)
{
	char svalue[1024];
	sprintf(svalue,"%d",value);
	QBSetDefault(key,svalue);
}

unsigned char* PPGame_GetData(const char* key,int* dataSize)
{
	const char* path = PPGameResourcePath(key);
	FILE* fp = fopen(path,"rb");
	if (fp) {
		fseek(fp,0,SEEK_END);
		int size = ftell(fp);
		fseek(fp,0,SEEK_SET);
		unsigned char* str = (unsigned char*)malloc(size);
		fread(str,size,1,fp);
		fclose(fp);
		*dataSize = size;
		return str;
	}
	return NULL;
}

void PPGame_SetData(const char* key,unsigned char* data,int dataSize)
{
	const char* path = PPGameResourcePath(key);
	FILE* fp = fopen(path,"wb");
	if (fp) {
		fwrite(data,dataSize,1,fp);
		fclose(fp);
	}
}

unsigned char* PPGame_LoadPNG(const char* fileName,unsigned long* imageWidth,unsigned long* imageHeight,unsigned long* bytesPerRow)
{

	FILE *PNG_file = fopen(fileName, "rb");

    fprintf(stderr, "open PNG file %s\n", fileName);

    if (PNG_file == NULL)
    {
        fprintf(stderr, "Can't open PNG file %s\n", fileName);
		return NULL;
    }
    
    unsigned char PNG_header[PNG_HEADER_SIZE];
    
    fread(PNG_header, 1, PNG_HEADER_SIZE, PNG_file);
    if (png_sig_cmp(PNG_header, 0, PNG_HEADER_SIZE) != 0)
    {
        fprintf(stderr, "%s is not a PNG file\n", fileName);
        fclose(PNG_file);
		return NULL;
    }
/*	
	{
		for (int i=0;i<8;i++) {
			printf("%02X,",PNG_header[i]);
		}
		printf("\n");
	}
*/
    png_structp PNG_reader
        = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (PNG_reader == NULL)
    {
        fprintf(stderr, "Can't start reading PNG file %s\n", fileName);
        fclose(PNG_file);
		return NULL;
    }

    png_infop PNG_info = png_create_info_struct(PNG_reader);
    if (PNG_info == NULL)
    {
        fprintf(stderr, "Can't get info for PNG file %s\n", fileName);
        png_destroy_read_struct(&PNG_reader, NULL, NULL);
        fclose(PNG_file);
		return NULL;
    }

    png_infop PNG_end_info = png_create_info_struct(PNG_reader);
    if (PNG_end_info == NULL)
    {
        fprintf(stderr, "Can't get end info for PNG file %s\n", fileName);
        png_destroy_read_struct(&PNG_reader, &PNG_info, NULL);
        fclose(PNG_file);
		return NULL;
    }
    
    if (setjmp(png_jmpbuf(PNG_reader)))
    {
        fprintf(stderr, "Can't load PNG file %s\n", fileName);
        png_destroy_read_struct(&PNG_reader, &PNG_info, &PNG_end_info);
        fclose(PNG_file);
		return NULL;
    }
    
    png_init_io(PNG_reader, PNG_file);
    png_set_sig_bytes(PNG_reader, PNG_HEADER_SIZE);
    
    png_read_info(PNG_reader, PNG_info);

	if (PNG_reader == NULL) {
        fclose(PNG_file);
		return NULL;
    }
    
    png_uint_32 width, height;
    width = png_get_image_width(PNG_reader, PNG_info);
    height = png_get_image_height(PNG_reader, PNG_info);

//printf("width %d\n",width);
//printf("height %d\n",height);
    
    png_uint_32 bit_depth, color_type;
    bit_depth = png_get_bit_depth(PNG_reader, PNG_info);
    color_type = png_get_color_type(PNG_reader, PNG_info);

//printf("bit_depth %d\n",bit_depth);
//printf("rowbytes %d\n",png_get_rowbytes(PNG_reader, PNG_info));
    
    if (color_type == PNG_COLOR_TYPE_PALETTE)
    {
    //png_set_palette_to_rgb(PNG_reader);
    png_set_palette_to_rgb(PNG_reader);
    }
/*
    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) 
    {
    png_set_gray_1_2_4_to_8(PNG_reader);
    }
*/
    if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
    {
    png_set_gray_to_rgb(PNG_reader);
    }
    if (png_get_valid(PNG_reader, PNG_info, PNG_INFO_tRNS))
    {
    png_set_tRNS_to_alpha(PNG_reader);
    }
    else
    {
    png_set_filler(PNG_reader, 0xff, PNG_FILLER_AFTER);
    }
    if (bit_depth == 16)
    {
        png_set_strip_16(PNG_reader);
    }
    
    png_read_update_info(PNG_reader, PNG_info);
    
    png_byte* PNG_image_buffer = (png_byte*)malloc(4 * width * height);
    png_byte** PNG_rows = (png_byte**)malloc(height * sizeof(png_byte*));
    
    unsigned int row;
    for (row = 0; row < height; ++row)
    {
        //PNG_rows[height - 1 - row] = PNG_image_buffer + (row * 4 * width);
        PNG_rows[row] = PNG_image_buffer + (row * 4 * width);
    }
    
    png_read_image(PNG_reader, PNG_rows);
    
    free(PNG_rows);
    
    png_destroy_read_struct(&PNG_reader, &PNG_info, &PNG_end_info);
    fclose(PNG_file);

    *imageWidth = width;
    *imageHeight = height;
	*bytesPerRow = width*4;

printf("width %d,height %d\n",width,height);

	{
		unsigned long width,height;
		unsigned char* image;
		width = *imageWidth;
		height = *imageHeight;
		image = PNG_image_buffer;
		if (image) {
			unsigned char* pixel = (unsigned char*)malloc(height*width*4);
			if (pixel) {
				int i,j;
				for (i = 0; i < height; i++) {
					for (j = 0; j < width; j++) {
#if 0
						pixel[j*4+0+i*width*4] = 0xff;
						pixel[j*4+1+i*width*4] = 0xff;
						pixel[j*4+2+i*width*4] = 0xff;
						pixel[j*4+3+i*width*4] = 0xff;
#else
						unsigned char r,g,b,a;
						r = image[j*4+0+i*width*4];
						g = image[j*4+1+i*width*4];
						b = image[j*4+2+i*width*4];
						a = image[j*4+3+i*width*4];

						if (a == 0) {
							pixel[j*4+0+i*width*4] = 0;
							pixel[j*4+1+i*width*4] = 0;
							pixel[j*4+2+i*width*4] = 0;
							pixel[j*4+3+i*width*4] = 0;
						} else {
							pixel[j*4+0+i*width*4] = r;
							pixel[j*4+1+i*width*4] = g;
							pixel[j*4+2+i*width*4] = b;
							pixel[j*4+3+i*width*4] = a;
						}
#endif
					}
				}
				free(image);
				PNG_image_buffer = pixel;
			}
		}
	}

	return PNG_image_buffer;
}

int PPGame_InitBGM(int no,const char* key)
{
	return 0;
}

void PPGame_ReleaseBGM()
{
}

void PPGame_IdleBGM(void* controller,int playBGM,bool playBGMOneTime,int chooseBGM)
{
}

int PPGame_GetLocale()
{
	return QBGAME_LOCALE_JAPANESE;
}
