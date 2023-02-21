// *********************************************************************
// *                                                                   *
// *                   EXAMPLE SCREEN SAVER APP                        *
// *                                                                   *
// * ROTATE THROUGH THE FIRST 25 IMAGES IN THE USER'S PICTURES FOLDER. *
// *                                                                   *
// *            SAVE THE EXECUTABLE WITH .SCR EXTENSION.               *
// *             RUN THE EXECUTABLE WITH /s PARAMETER.                 *
// *                                                                   *
// *********************************************************************

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
#include <objidl.h>
#include <gdiplus.h>
#include <shlobj.h>
#include <strsafe.h>

// include the special screen saver header
#include <scrnsave.h>

using namespace Gdiplus;

#define MAX_IMAGES 25
WCHAR images[MAX_IMAGES + 1][MAX_PATH + 1];
int imageIndex;

#define IM_COVER   1
#define IM_CONTAIN 2
#define IM_STRETCH 3

int imageMode = IM_COVER;


BOOL WINAPI RegisterDialogClasses(HANDLE hInst)
{
	return TRUE;
}

BOOL WINAPI ScreenSaverConfigureDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	
	return FALSE;
}

void DrawScreen(HDC hdc, HWND hwnd)
{
	RECT rcClient;
	GetClientRect(hwnd, &rcClient);
	Graphics graphics(hdc);
	if (images[imageIndex][0])
	{
		// Create an Image (Bitmap) object
		Image bitmap(images[imageIndex]);
		// Draw the original source image
		Rect gdiRect;
		if (IM_COVER == imageMode)
		{
			INT windowWidth = rcClient.right - rcClient.left;
			INT windowHeight = rcClient.bottom - rcClient.top;
			float percentWidth = windowWidth / (float)bitmap.GetWidth();
			float percentHeight = windowHeight / (float)bitmap.GetHeight();
			float percent = percentHeight > percentWidth ? percentHeight : percentWidth;
			gdiRect.Width = bitmap.GetWidth() * percent;
			gdiRect.Height = bitmap.GetHeight() * percent;
			gdiRect.X = (windowWidth - (INT)gdiRect.Width) / 2;
			gdiRect.Y = (windowHeight - (INT)gdiRect.Height) / 2;
		}
		else if (IM_CONTAIN == imageMode)
		{
			INT windowWidth = rcClient.right - rcClient.left;
			INT windowHeight = rcClient.bottom - rcClient.top;
			float percentWidth = windowWidth / (float)bitmap.GetWidth();
			float percentHeight = windowHeight / (float)bitmap.GetHeight();
			float percent = windowWidth > windowHeight ? percentHeight : percentWidth;
			gdiRect.Width = bitmap.GetWidth() * percent;
			gdiRect.Height = bitmap.GetHeight() * percent;
			gdiRect.X = (windowWidth - (INT)gdiRect.Width) / 2;
			gdiRect.Y = (windowHeight - (INT)gdiRect.Height) / 2;

			FillRect(hdc, &rcClient, (HBRUSH)GetStockObject(BLACK_BRUSH));
		}
		else if (IM_STRETCH == imageMode)
		{
			gdiRect.Width = rcClient.right - rcClient.left;
			gdiRect.Height = rcClient.bottom - rcClient.top;
			gdiRect.X = gdiRect.Y = 0;
		}
		graphics.DrawImage(&bitmap, gdiRect);
	}
	else
	{
		static float offset = 0.0;
		float width = (float)rcClient.right - rcClient.left;
		if (offset > width)
			offset = 0.0;
		FillRect(hdc, &rcClient, (HBRUSH)GetStockObject(BLACK_BRUSH));
		float fontSize = (rcClient.bottom - rcClient.top) / 11.0; // 11 lines

		FontFamily fontFamily(L"Times New Roman");
		Font       font(&fontFamily, fontSize, FontStyleBold, UnitPixel);
		PointF     pointF(offset, 0.0f);
		SolidBrush solidBrush(Color(255, 192, 32, 32));

		offset += width / 25;

		// draw text telling user there are no pics in the pics folder
		graphics.DrawString(L"Add pictures", -1, &font, pointF, &solidBrush);
		pointF.Y += fontSize;
		graphics.DrawString(L"to the folder", -1, &font, pointF, &solidBrush);
		WCHAR myPics[MAX_PATH + 1];
		if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_MYPICTURES, NULL, SHGFP_TYPE_CURRENT, myPics)))
		{
			SolidBrush solidBrush(Color(255, 64, 64, 225));
			pointF.Y += fontSize;
			graphics.DrawString(myPics, -1, &font, pointF, &solidBrush);
		}
	}
}

GdiplusStartupInput gdiplusStartupInput;
ULONG_PTR gdiplusToken;

// handle screen saver window messages; most are already handled by "DefScreenSaverProc"
LRESULT WINAPI ScreenSaverProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int x, y;
	static int X, Y, called = 0;
	switch (msg)
	{
	case WM_TIMER:
	{
		imageIndex++;
		if (!images[imageIndex][0])
			imageIndex = 0;
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
		// get images to display from the user's pictures folder
		ZeroMemory(images, sizeof(images));
		imageIndex = 0;
		WCHAR myPics[MAX_PATH + 1];
		if (FAILED(SHGetFolderPath(NULL, CSIDL_MYPICTURES, NULL, SHGFP_TYPE_CURRENT, myPics)))
			return 0;
		StringCchCat(myPics, MAX_PATH, L"\\Saved Pictures\\*");
		WIN32_FIND_DATA fd;
		HANDLE hFind;
		if ((hFind = FindFirstFile(myPics, &fd)) != INVALID_HANDLE_VALUE)
		{
			int i = 0;
			do
			{
				if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					// TODO: search this directory for more images
				}
				else if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN || fd.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM))
				{
					if (FAILED(SHGetFolderPath(NULL, CSIDL_MYPICTURES, NULL, SHGFP_TYPE_CURRENT, images[i])))
						return 0;
					StringCchCat(images[i], MAX_PATH, L"\\Saved Pictures\\");
					StringCchCat(images[i], MAX_PATH, fd.cFileName);
					i++;
				}
			} while (i < MAX_IMAGES && FindNextFile(hFind, &fd) != 0);
		}
		GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

		SetTimer(hwnd, 1, 2000, NULL); // 2000ms
	}
	return 0;

	case WM_DESTROY:
		KillTimer(hwnd, 1);

		GdiplusShutdown(gdiplusToken);

		return 0;
	}
	return DefScreenSaverProc(hwnd, msg, wParam, lParam);
}
