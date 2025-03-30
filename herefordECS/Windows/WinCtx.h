#pragma once

#include "stdafx.h"

class HerefordECS;

class WinCtx
{
public:
	static int Run(HerefordECS* pEngine, std::wstring titleName, HINSTANCE hInstance, int nCmdShow);
	static HWND GetHwnd() { return mHWnd; };

protected:
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static void ParseCommandLineArgs(_In_reads_(argc) WCHAR* argv[], int argc, HerefordECS* pEngine);


private:
	static HWND mHWnd;

};

