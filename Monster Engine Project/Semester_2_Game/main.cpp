#include <Windows.h>

#include "MainGame.h"

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

int WINAPI wWinMain(HINSTANCE i_hInstance, HINSTANCE i_hPrevInstance, LPWSTR i_lpCmdLine, int i_nCmdShow)
{
	//_CrtSetBreakAlloc(506);

	MainGame mainGame;

	mainGame.Initialize(i_hInstance, i_nCmdShow);
	mainGame.Run();

#ifdef _DEBUG
	_CrtDumpMemoryLeaks();
#endif
}

