
#include "Windows/stdafx.h"
#include "Windows/WinCtx.h"
#include "Core/HerefordECS.h"

_Use_decl_annotations_
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    HerefordECS hereford(1920, 1080);
    return WinCtx::Run(&hereford, L"HerefordECS", hInstance, nCmdShow);
}
