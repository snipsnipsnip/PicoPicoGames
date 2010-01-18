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
#include <windows.h>
#include <stdio.h>
#include <Shlwapi.h>
#include <math.h>
#include <gl/gl.h>

#include "QBSound.h"
#include "QBGame.h"
#include "QBSoundWin.h"
#include "PPGameDef.h"
#include "PPGameSplite.h"
#include "PPGameButton.h"

#include "carrace.h"
#define _PG_GLOBAL_
#include "key.h"
#include "font.h"
#include "font2.h"

extern QBGame* QBGameFactory();

#define TIMER_ID   1

static QBGame* game;
static PPGameSplite* g;

static int arrowKeyNo;
static int buttonANo;
static int buttonBNo;
static int touchLocation_x;
static int touchLocation_y;
static bool touchScreen;

static int screenWidth = 480;
static int screenHeight = 320;
static HINSTANCE winGameInstance=NULL;
static HWND winGameWindow=NULL;
//static int activeApplication=1;
static char fpath[1024];
static int polyCount;
static PPGamePoly poly[PPGAME_MAX_POLY];

static void OnCreate(HWND hWnd, LPCREATESTRUCT lpcs);
static void OnDestroy(HWND hWnd);
static void CALLBACK TimerProc(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);
static BOOL bSetupPixelFormat(HDC hdc);

static void getApplicationPath(char* buffer,int length)
{
#ifdef _WIN32
	char szDLLPath[MAX_PATH];
	char szDrive[ 8 ];
	char szDir[ MAX_PATH ];
	GetModuleFileName( NULL, szDLLPath, MAX_PATH );
	_splitpath( szDLLPath, szDrive, szDir, NULL, NULL );
	wsprintf( buffer,(const char*)"%s%s", szDrive, szDir );
#else
	CFStringRef bundleid = CFStringCreateWithCString(NULL, EPSUI_GetModuleName(),0);
	CFBundleRef bundle = CFBundleGetBundleWithIdentifier(bundleid);
	CFURLRef url = CFBundleCopyBundleURL(bundle);
	CFURLRef baseurl = CFURLCreateCopyDeletingLastPathComponent(NULL,url);
	CFURLGetFileSystemRepresentation(baseurl,true,(UInt8*)buffer,length);
	CFRelease(baseurl);
	CFRelease(url);
	CFRelease(bundleid);
#endif
}

const char* QBGameResource(const char* name)
{
	char buf[1024];
	getApplicationPath(buf,1024);
	sprintf(fpath,"%s\\DATA\\%s",buf,name);
printf("%s\n",fpath);
	return fpath;
}

static const char* WindowsProgramName()
{
	return "PicoPicoGames";
}

static void setupKey()
{
	int layout = game->WindowLayout();
	switch (layout) {
	case QBGAME_CONTROLLER_TYPE_VERTICAL:
		if (PPGame_GetDefault(PPGAME_KEY_SIDE_RIGHT)) {
			layout = QBGAME_CONTROLLER_TYPE_V_RIGHT;
		} else {
			layout = QBGAME_CONTROLLER_TYPE_V_LEFT;
		}
		break;
	case QBGAME_CONTROLLER_TYPE_HORIZONTAL:
		if (PPGame_GetDefault(PPGAME_KEY_SIDE_RIGHT)) {
			layout = QBGAME_CONTROLLER_TYPE_H_RIGHT;
		} else {
			layout = QBGAME_CONTROLLER_TYPE_H_LEFT;
		}
		break;
	default:
		break;
	}
	switch (layout) {
	case QBGAME_CONTROLLER_TYPE_V_LEFT:
		arrowKeyNo = PG_BUTTON_VLC;
		buttonANo = PG_BUTTON_VL1;
		buttonBNo = PG_BUTTON_VL2;
		break;
	case QBGAME_CONTROLLER_TYPE_V_RIGHT:
		arrowKeyNo = PG_BUTTON_VRC;
		buttonANo = PG_BUTTON_VR1;
		buttonBNo = PG_BUTTON_VR2;
		break;
	case QBGAME_CONTROLLER_TYPE_H_LEFT:
		arrowKeyNo = PG_BUTTON_HLC;
		buttonANo = PG_BUTTON_HL1;
		buttonBNo = PG_BUTTON_HL2;
		break;
	case QBGAME_CONTROLLER_TYPE_H_RIGHT:
		arrowKeyNo = PG_BUTTON_HRC;
		buttonANo = PG_BUTTON_HR1;
		buttonBNo = PG_BUTTON_HR2;
		break;
	case QBGAME_CONTROLLER_TYPE_VERTICAL_CENTER:
	default:
		arrowKeyNo = PG_BUTTON_C;
		buttonANo = PG_BUTTON_1;
		buttonBNo = PG_BUTTON_2;
		break;
	}
}

static void drawKey(int textureId)
{
	setupKey();
	int i = polyCount;
	if (game->VisibleButton() & QBGAME_CONTROLLER_ENABLE_ARROW) {
		poly[i].color(255, 255, 255);
		poly[i].alpha = 128;
		poly[i++].splite(0,0,0,arrowKeyNo,textureId);
	}
	if (game->VisibleButton() & QBGAME_CONTROLLER_ENABLE_A) {
		poly[i].color(255, 255, 255);
		poly[i].alpha = 128;
		poly[i++].splite(0,0,0,buttonANo,textureId);
	}
	if (game->VisibleButton() & QBGAME_CONTROLLER_ENABLE_B) {
		poly[i].color(255, 255, 255);
		poly[i].alpha = 128;
		poly[i++].splite(0,0,0,buttonBNo,textureId);
	}
	polyCount = i;
}

int QBSetDefault(const char* key,char* value)
{
	if (WindowsProgramName()) {
		char szModulePath[1024];
		char szIniFile[1024];
		char pathIni[1024] ;

		// モジュールのパスを取得//
		getApplicationPath(szModulePath,1024);
		strcpy(pathIni,WindowsProgramName());
		PathRenameExtension(pathIni,".ini") ;
		wsprintf(szIniFile,"%s%s",szModulePath,pathIni);

		WritePrivateProfileString("ApplicationParameters",key,(const CHAR*)value, szIniFile);
		return 0;
	}
	return -1;
}

int QBGetDefault(const char* key,char* value,int length)
{
	if (value && length > 0) {
		value[0] = 0;
		if (WindowsProgramName()) {
			char szModulePath[1024];
			char szIniFile[1024];
			char pathIni[1024] ;

			// モジュールのパスを取得//
			getApplicationPath(szModulePath,1024);
			strcpy(pathIni,WindowsProgramName());
			PathRenameExtension(pathIni,".ini") ;
			wsprintf(szIniFile,"%s%s",szModulePath,pathIni);

			GetPrivateProfileString("ApplicationParameters",key,"",value,length,szIniFile);
			return 0;
		}
	}
	return -1;
}

static int SetWinCenter(HWND hWnd)
{
    HWND hDeskWnd;
    RECT deskrc, rc;
    int x, y;
    
    hDeskWnd = GetDesktopWindow();
    GetWindowRect(hDeskWnd, (LPRECT)&deskrc);
    GetWindowRect(hWnd, (LPRECT)&rc);
    x = (deskrc.right - (rc.right-rc.left)) / 2;
    y = (deskrc.bottom - (rc.bottom-rc.top)) / 2;
    SetWindowPos(hWnd, HWND_TOP, x, y, (rc.right-rc.left), (rc.bottom-rc.top),SWP_SHOWWINDOW); 
    return 0;
}

static BOOL bSetupPixelFormat(HDC hdc)
{ 
    PIXELFORMATDESCRIPTOR pfd, *ppfd; 
    int pixelformat; 
 
    ppfd = &pfd; 
 
    ppfd->nSize = sizeof(PIXELFORMATDESCRIPTOR); 
    ppfd->nVersion = 1; 
    ppfd->dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL |  
                        PFD_DOUBLEBUFFER; 
    ppfd->dwLayerMask = PFD_MAIN_PLANE; 
    ppfd->iPixelType = PFD_TYPE_COLORINDEX; 
    ppfd->cColorBits = 8; 
    ppfd->cDepthBits = 16; 
    ppfd->cAccumBits = 0; 
    ppfd->cStencilBits = 0; 
 
    pixelformat = ChoosePixelFormat(hdc, ppfd); 
 
    if ( (pixelformat = ChoosePixelFormat(hdc, ppfd)) == 0 ) 
    { 
        MessageBox(NULL, "ChoosePixelFormat failed", "Error", MB_OK); 
        return FALSE; 
    } 
 
    if (SetPixelFormat(hdc, pixelformat, ppfd) == FALSE) 
    { 
        MessageBox(NULL, "SetPixelFormat failed", "Error", MB_OK); 
        return FALSE; 
    } 
 
    return TRUE; 
}

static void glSetInterval(int isinterval)
{
	BOOL (WINAPI *wglSwapIntervalEXT)(int) = NULL;
	wglSwapIntervalEXT = (BOOL (WINAPI*)(int))wglGetProcAddress("wglSwapIntervalEXT");
	if( wglSwapIntervalEXT ) {
		wglSwapIntervalEXT(isinterval);
	}
}

static void OnCreate(HWND hWnd, LPCREATESTRUCT lpcs)
{
//#pragma unused (lpcs)
//	mainWindowHandle = hWnd;

	{
	    HDC hDC;    /* device context handles */
		HGLRC hRC;

	    /* Get the handle of the windows device context. */
	    hDC = GetDC(hWnd);

		bSetupPixelFormat(hDC);

	    /* Create a rendering context and make it the current context */
	    hRC = wglCreateContext(hDC);
	    if (!hRC)
	    {
	        MessageBox(NULL, "Cannot create context.", "Error", MB_OK);
	        return;
	    }
	    wglMakeCurrent(hDC, hRC);
	}
	
	glViewport(0,0,screenWidth,screenHeight);
/*
 	if (_gameSystem) {

		if (gFullscreenFlag) {
			_gameSystem->SetViewPort((640-screenWidth)/2,(480-screenHeight)/2,screenWidth,screenHeight);
		} else {
			_gameSystem->SetViewPort(0,0,screenWidth,screenHeight);
		}

	}
*/	
	glSetInterval(1);

	// タイマーの作成//
	SetTimer(hWnd, TIMER_ID, 1, TimerProc);
}

static void OnDestroy(HWND hWnd)
{
    // タイマーの削除//
    KillTimer(hWnd, TIMER_ID);

	{
	    HGLRC hRC;       /* rendering context handle */
	    HDC   hDC;       /* device context handle */

	    /* Release and free the device context and rendering context. */
	    hDC = wglGetCurrentDC();
	    hRC = wglGetCurrentContext();

	    wglMakeCurrent(NULL, NULL);

	    if (hRC)
	        wglDeleteContext(hRC);

	    if (hDC)
	        ReleaseDC(hWnd, hDC);

	}
/*
	if (_gameSystem) {
		_gameSystem->ExitGame();
	}
*/
    PostQuitMessage(0);
}

LRESULT CALLBACK WndMainProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
//	TCHAR lpszFn[MAX_PATH+1];
	static BOOL loaded;

    switch(Msg)
    {
	case WM_SYSCOMMAND:
			switch (wParam & 0xfff0) {
		    case SC_MONITORPOWER:
		    case SC_SCREENSAVE :
		        return 0;		//スクリーンセーバー対策//
		    }
		    return (DefWindowProc(hWnd, Msg, wParam, lParam));
    case WM_CREATE:
		winGameWindow = hWnd;
        OnCreate(hWnd, (LPCREATESTRUCT)lParam);
        break;

    case WM_DESTROY:
        OnDestroy(hWnd);
        break;

	case WM_DROPFILES: /* ファイルがドロップされた時の処理 */
		loaded=TRUE;
		break;

	case WM_WINDOWPOSCHANGED:
		glViewport(0,0,screenWidth,screenHeight);
/*
		if (_gameSystem) {
			_gameSystem->SetViewPort(0,0,screenWidth,screenHeight);
		}
*/
		break;

	case WM_ACTIVATE:
/*
		if (_gameSystem) {
			if(wParam == WA_INACTIVE){
				activeApplication = 0;
			} else {
				activeApplication = 1;
			}
		}
*/
		break;
		
	case WM_LBUTTONDOWN:
		{
			touchLocation_x = LOWORD(lParam);
			touchLocation_y = HIWORD(lParam);
			touchScreen = true;
		}
		break;
	case WM_LBUTTONUP:
		{
			touchScreen = false;
		}
		break;

    default:
        return DefWindowProc(hWnd, Msg, wParam, lParam);
    }

    return 0L;
}

static int arrowKey(GLButton* button,int count)
{
	int n=-1;
	if (touchScreen) {
//		NSPoint location = NSPointFromString(t);
//		CGPoint pos;
		int pos_x = touchLocation_x;
		int pos_y = touchLocation_y;
		{
			int hx,hy,hw,hh;
			hx = PPGame_CharDeltaX(button->group,button->image,button->pat);
			hy = PPGame_CharDeltaY(button->group,button->image,button->pat);
			hw = PPGame_CharDeltaW(button->group,button->image,button->pat);
			hh = PPGame_CharDeltaH(button->group,button->image,button->pat);
			pos_x = touchLocation_x - (hx+hw/2);
			pos_y = touchLocation_y - (hy+hh/2);
		}
		int len = 96*96;
		if (game) {
			len = game->KeySize();
		}
		float l = pos_x*pos_x+pos_y*pos_y;
		if (l < 8*8) {
		} else
		if (l > len) {
		} else {
			float dt = 360.0f/((float)count);
			float q = atan2f(pos_x,pos_y)*180/M_PI+180+dt/2;
			n = (int)(q / dt);
			if (n >= 8) n = 0;
//			break;
		}
	}
	return n;
}

static bool touchCheck(GLButton* button)
{
	if (touchScreen) {
		int hx,hy,hw,hh;
		int x,y;
//		NSPoint location = NSPointFromString(t);
		x = touchLocation_x;
		y = touchLocation_y;
		{
			hx = PPGame_CharDeltaX(button->group,button->image,button->pat);
			hy = PPGame_CharDeltaY(button->group,button->image,button->pat);
			hw = PPGame_CharDeltaW(button->group,button->image,button->pat);
			hh = PPGame_CharDeltaH(button->group,button->image,button->pat);
			if (x-button->dx > hx && y-button->dy > hy && x-button->dx < hx+hw && y-button->dy < hy+hh) {
				return true;
			}
		}
	}
	return false;
}

static unsigned long arrowKey4(GLButton* button)
{
	unsigned long key = 0;
	int n = arrowKey(button,4);
	if (n >= 0) {
		switch ((n+1) % 4) {
		case 0:
			key |= (PAD_RIGHT);
			break;
		case 1:
			key |= (PAD_UP);
			break;
		case 2:
			key |= (PAD_LEFT);
			break;
		case 3:
			key |= (PAD_DOWN);
			break;
		}
	}
	return key;
}

static unsigned long arrowKey8(GLButton* button)
{
	unsigned long key = 0;
	int n = arrowKey(button,8);
	if (n >= 0) {
		switch ((n+2) % 8) {
		case 0:
			key |= (PAD_RIGHT);
			break;
		case 1:
			key |= (PAD_UP | PAD_RIGHT);
			break;
		case 2:
			key |= (PAD_UP);
			break;
		case 3:
			key |= (PAD_UP | PAD_LEFT);
			break;
		case 4:
			key |= (PAD_LEFT);
			break;
		case 5:
			key |= (PAD_DOWN | PAD_LEFT);
			break;
		case 6:
			key |= (PAD_DOWN);
			break;
		case 7:
			key |= (PAD_DOWN | PAD_RIGHT);
			break;
		}
	}
	return key;
}

static unsigned long getKey()
{
	unsigned long key = 0;
	
	setupKey();

	if (game->VisibleButton() & QBGAME_CONTROLLER_ENABLE_ARROW) {
		GLButton b(key_group,arrowKeyNo,0);
		if (game->KeyCount() == 4) {
			key |= arrowKey4(&b);
		} else {
			key |= arrowKey8(&b);
		}
	}
	if (game->VisibleButton() & QBGAME_CONTROLLER_ENABLE_A) {
		GLButton b(key_group,buttonANo,0);
		if (touchCheck(&b)) {
			key |= PAD_A;
		}
	}
	if (game->VisibleButton() & QBGAME_CONTROLLER_ENABLE_B) {
		GLButton b(key_group,buttonBNo,0);
		if (touchCheck(&b)) {
			key |= PAD_B;
		}
	}
	
	//key |= [view staticButton]; 

	return key;
}

static void CALLBACK TimerProc(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	HDC hDC;//, hDCTemp;    /* device context handles */

	hDC = GetDC(hWnd);

	unsigned long key = 0;

	if (GetForegroundWindow() == hWnd) {
		if ((GetKeyState(VK_LEFT) & 0x8000)) {
			key |= PAD_LEFT;
		}
		if ((GetKeyState(VK_RIGHT) & 0x8000)) {
			key |= PAD_RIGHT;
		}
		if ((GetKeyState(VK_UP) & 0x8000)) {
			key |= PAD_UP;
		}
		if ((GetKeyState(VK_DOWN) & 0x8000)) {
			key |= PAD_DOWN;
		}
		if ((GetKeyState(VK_ESCAPE) & 0x8000)) {
			//ExitGame();
			PostQuitMessage(0);
		}
		if ((GetKeyState(VK_SPACE) & 0x8000)) {
			key |= PAD_A;
		}
#if 0
		if ((GetKeyState(VK_RETURN) & 0x8000)) {
			key |= JOY_PAD_RETURN;
		}
		if ((GetKeyState(VK_SHIFT) & 0x8000)) {
			key |= JOY_PAD_SHIFT;
		}
		if ((GetKeyState(VK_CONTROL) & 0x8000)) {
			key |= JOY_PAD_CONTROL;
		}
#endif
		if ((GetKeyState('X') & 0x8000)) {
			key |= PAD_B;
		}
		if ((GetKeyState('Z') & 0x8000)) {
			key |= PAD_A;
		}
		//key |= GetJoyKeyState();
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glViewport(0,0,640,400);
	glViewport(0,0,screenWidth,screenHeight);
	//glLoadIdentity();
	glLoadIdentity();

	glOrtho(-1.0f,1.0f,-1.0f,1.0f,-1.0f,1.0f);

	//gluPerspective(40.0, (double)viewPort.size.width / (double)viewPort.size.height, 0.1, 10000.0);
	//gluLookAt(0.0,0.0,0.0,0.0,0.0,-1.0,0.0,-1.0,0.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	
	glDisable(GL_CULL_FACE);

	g->screenWidth = screenWidth;//self.width;//160;
	g->screenHeight = screenHeight;//self.height;//416/2;

	if (game) {
		//unsigned long key = 0;
		game->setTouchCount(0);
		if (touchScreen) game->setTouchPosition(touchLocation_x,touchLocation_y);
		key |= getKey();
		polyCount = game->gameIdle(key,poly,g,NULL);
		if (game->KeyTextureID() >= 0) drawKey(game->KeyTextureID());

		if (game->TextureInfo()) {
			PPGameTextureInfo* info = game->TextureInfo();
			if (!g->initLoadTex) {
				if (g->SetTextureList(info) == 0) {
					PPGameTextureInfo* list = info;
					for (int i=0;list[i].name!=NULL;i++) {
						if (list[i].parge == false) {
							g->LoadTexture(i);
						} else {
							g->FreeTexture(i);
						}
					}
					g->initLoadTex = true;
				}
			}
		}

		g->ClearScreen2D(0.0f, 0.0f, 0.0f);

		g->InitOT();
		for (int i=0;i<polyCount;i++) {
			g->DrawPoly(&poly[i]);
		}
		g->DrawOT();

		if (game->exitGame) {
			game->exitGame = false;
			PostQuitMessage(0);
		}
	}

	//if (r == 0) {
		SwapBuffers(hDC);
	//}
	
	ReleaseDC(hWnd,hDC);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
//#pragma unused (hPrevInstance,lpCmdLine)
    HWND        hWnd = NULL;
    WNDCLASSEX  wcex;
    MSG         msg;

	QBSound_Start(4);
	QBSound_Reset();

	for (int i=0;i<PPGAME_MAX_POLY;i++) {
		poly[i].init();
	}

	game = QBGameFactory();///new QBMissilePanic();
	g = new PPGameSplite();
	g->Init();

	winGameInstance = hInstance;

    wcex.cbSize         = sizeof(wcex);
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndMainProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, "icon");
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = WindowsProgramName();
    wcex.hIconSm        = NULL;


	switch (game->WindowLayout()) {
	case QBGAME_CONTROLLER_TYPE_HORIZONTAL:
	case QBGAME_CONTROLLER_TYPE_H_LEFT:
	case QBGAME_CONTROLLER_TYPE_H_RIGHT:
		screenWidth = 480;
		screenHeight = 320;
		break;
	case QBGAME_CONTROLLER_TYPE_VERTICAL:
	case QBGAME_CONTROLLER_TYPE_V_LEFT:
	case QBGAME_CONTROLLER_TYPE_V_RIGHT:
	case QBGAME_CONTROLLER_TYPE_VERTICAL_CENTER:
	default:
		screenWidth = 320;
		screenHeight = 480;
		break;
	}

	QBSoundWin::BufferSize = BUFFSIZE;//GetParam("soundBuffer",BUFFSIZE);
	QBSoundWin::BufferSizeVista = BUFFSIZEVISTA;//GetParam("soundBufferVista",BUFFSIZEVISTA);

    if(!RegisterClassEx(&wcex))
    {
        return 0;
    }

	hWnd = CreateWindowEx( WS_EX_APPWINDOW, 
			WindowsProgramName(), 
			WindowsProgramName() , 
			(WS_OVERLAPPEDWINDOW | WS_POPUP) & ~(WS_THICKFRAME | WS_MAXIMIZEBOX),  
			CW_USEDEFAULT, 
			CW_USEDEFAULT, 
			screenWidth+6, 
			screenHeight+32, 
			NULL, 
			NULL, 
			hInstance, 
			NULL );
	SetWinCenter(hWnd);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    while(GetMessage(&msg, NULL, 0, 0) != 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
	
	delete game;

    return msg.wParam;
}
