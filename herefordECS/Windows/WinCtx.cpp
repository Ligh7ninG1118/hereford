#include "WinCtx.h"
#include "Core/HerefordECS.h"

HWND WinCtx::mHWnd = nullptr;

int WinCtx::Run(HerefordECS* pEngine, std::wstring titleName, HINSTANCE hInstance, int nCmdShow)
{
	// Command args handling
	{
		int argc;
		LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);
		ParseCommandLineArgs(argv, argc, pEngine);
		LocalFree(argv);
	}

	WNDCLASSEX wndClass = { 0 };
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WindowProc;
	wndClass.hInstance = hInstance;
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.lpszClassName = L"HerefordClass";
	RegisterClassEx(&wndClass);

	RECT wndRect = { 0, 0, pEngine->GetWidth(), pEngine->GetHeight() };
	AdjustWindowRect(&wndRect, WS_OVERLAPPEDWINDOW, FALSE);

	mHWnd = CreateWindow(
		wndClass.lpszClassName,
		titleName.c_str(),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		wndRect.right - wndRect.left,
		wndRect.bottom - wndRect.top,
		nullptr,
		nullptr,
		hInstance,
		pEngine
	);

	pEngine->OnInit();

	SetProcessDPIAware();
	ShowWindow(mHWnd, nCmdShow);

	MSG msg = {};
	bool shouldQuit = false;

	while (true)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				shouldQuit = true;
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (shouldQuit)
			break;

		pEngine->OnUpdate();
	}

	pEngine->OnDestroy();

	return static_cast<char>(msg.wParam);
}

LRESULT WinCtx::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HerefordECS* pEngine = reinterpret_cast<HerefordECS*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	switch (uMsg)
	{
		case WM_CREATE:
			{
				LPCREATESTRUCT pCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
				SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCreateStruct->lpCreateParams));
			}
			return 0;

		case WM_MOUSEMOVE:

			return 0;
		case WM_KEYUP:
			if (pEngine)
			{
				pEngine->OnKeyUp(static_cast<unsigned char>(wParam));
			}
			return 0;
		case WM_KEYDOWN:
			if (pEngine)
			{
				pEngine->OnKeyDown(static_cast<unsigned char>(wParam));
			}
			return 0;
		case WM_PAINT:
			if (pEngine)
			{
				//pEngine->OnUpdate();
			}
			return 0;
		case WM_SIZE:
			if (pEngine)
			{
				pEngine->SetWidth(static_cast<int>(LOWORD(lParam)));
				pEngine->SetHeight(static_cast<int>(HIWORD(lParam)));
			}
			return 0;
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

_Use_decl_annotations_
void WinCtx::ParseCommandLineArgs(WCHAR* argv[], int argc, HerefordECS* pEngine)
{
}
