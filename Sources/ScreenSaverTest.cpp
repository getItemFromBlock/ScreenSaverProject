#ifndef UNICODE
#define UNICODE
#endif

// "#pragma comment" lines only work in visual studio; otherwise include these libraries with the linker
#pragma comment(lib, "Gdiplus.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "comctl32.lib")
#ifdef UNICODE
#pragma comment(lib, "ScrnSavw.lib")
#else
#pragma comment(lib, "ScrnSave.lib")
#endif

#include <windows.h>
#include <gdiplus.h>
// include the special screen saver header
#include <scrnsave.h>

#include <algorithm>

#include "ScreenSaverTest.h"

#include "ScreenRenderer.hpp"

Gdiplus::Bitmap* bp = nullptr;
ScreenRenderer renderer = ScreenRenderer(Maths::IVec2(256, 256));

BOOL WINAPI RegisterDialogClasses(HANDLE hInst)
{
	return TRUE;
}

BOOL WINAPI ScreenSaverConfigureDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HWND hSpeed;   // handle to speed scroll bar 
	static HWND hOK;      // handle to OK push button  
	HRESULT result;

	switch (message)
	{
	case WM_INITDIALOG:

		// Retrieve the application name from the .rc file.  
		result = LoadString(hMainInstance, idsAppName, szAppName, APPNAMEBUFFERLEN);

		// Retrieve the .ini (or registry) file name. 
		result = LoadString(hMainInstance, idsIniFile, szIniFile, MAXFILELEN);

		// Retrieve a handle to the OK push button control.  
		hOK = GetDlgItem(hDlg, ID_OK);

		return TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_OK:

		case ID_CANCEL:
			EndDialog(hDlg, LOWORD(wParam) == ID_OK);

			return TRUE;
		default:
			return false;
		}
	case WM_CLOSE:
		EndDialog(hDlg, 0);
		return true;
	}
	return FALSE;
}

void WriteData()
{
	unsigned int width = bp->GetWidth();
	unsigned int height = bp->GetHeight();
	{
		Gdiplus::BitmapData bmdata;
		Gdiplus::Rect rect(0, 0, width, height);
		bp->LockBits(&rect, Gdiplus::ImageLockModeWrite, PixelFormat32bppRGB, &bmdata);
		for (unsigned int y = 0; y < height; ++y)
		{
			unsigned int* line = (unsigned int*)((char*)bmdata.Scan0 + (size_t)bmdata.Stride * y);
			auto source = renderer.GetLineData(y);
			std::copy(source, source + width, line);
		}
		bp->UnlockBits(&bmdata);
	}
}

void DrawScreen(HDC hdc, HWND hwnd)
{
	RECT rcClient;
	GetClientRect(hwnd, &rcClient);
	Gdiplus::Graphics graphics(hdc);

	renderer.DrawScreen();
	WriteData();

	INT windowWidth = rcClient.right - rcClient.left;
	INT windowHeight = rcClient.bottom - rcClient.top;
	float percentWidth = windowWidth / (float)(bp->GetWidth()-1);
	float percentHeight = windowHeight / (float)(bp->GetHeight()-1);
	float percent = windowWidth > windowHeight ? percentHeight : percentWidth;
	Gdiplus::Rect gdiRect;
	gdiRect.Width = (INT)(bp->GetWidth() * percent);
	gdiRect.Height = (INT)(bp->GetHeight() * percent);
	gdiRect.X = (windowWidth - (INT)gdiRect.Width) / 2;
	gdiRect.Y = (windowHeight - (INT)gdiRect.Height) / 2;
	//FillRect(hdc, &rcClient, (HBRUSH)GetStockObject(BLACK_BRUSH));
	graphics.SetInterpolationMode(Gdiplus::InterpolationModeNearestNeighbor);
	graphics.DrawImage(reinterpret_cast<Gdiplus::Image*>(bp), gdiRect);
}

Gdiplus::GdiplusStartupInput gdiplusStartupInput;
ULONG_PTR gdiplusToken;

// handle screen saver window messages; most are already handled by "DefScreenSaverProc"
LRESULT WINAPI ScreenSaverProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_TIMER:
	{
		RECT rect;
		GetClientRect(hwnd, &rect);
		InvalidateRect(hwnd, &rect, FALSE);
	}
	return 0;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		DrawScreen(hdc, hwnd);
		EndPaint(hwnd, &ps);
	}
	return 0;

	case WM_CREATE:
	{
		// Retrieve the application name from the .rc file. 
		LoadString(hMainInstance, idsAppName, szAppName, APPNAMEBUFFERLEN);

		// Retrieve the .ini (or registry) file name. 
		LoadString(hMainInstance, idsIniFile, szIniFile, MAXFILELEN);

		GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

		bp = new Gdiplus::Bitmap(256, 256, PixelFormat32bppRGB);

		SetTimer(hwnd, 1, 20, NULL); // 2000ms
	}
	return 0;

	case WM_DESTROY:
		KillTimer(hwnd, 1);

		Gdiplus::GdiplusShutdown(gdiplusToken);

		return 0;
	}
	return DefScreenSaverProc(hwnd, msg, wParam, lParam);
}
