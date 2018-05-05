#ifndef MAIN_H_
#define MAIN_H_

#define _CRT_SECURE_NO_DEPRECATE

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gl\GL.h>
#include <gl\GLU.h>


#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600
#define SCREEN_DEPTH  16


#include "Octree.h"

extern bool		 g_bFullScreen;
extern HWND		 g_hWnd;
extern RECT		 g_rRect;
extern HDC		 g_hHDC;
extern HGLRC	 g_hHRC;
extern HINSTANCE g_hHINSTANCE;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, hPrev, PSTR cmdline, int ishow);

LRESULT CALLBACK WinProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);





#endif
