#include <engine.h>

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <graphics\d3d11_renderer.h>
//#include <graphics\d3d9_renderer.h>

int __stdcall WinMain(HINSTANCE, HINSTANCE, char *, int)
{
	{// メモリーリーク検出
#ifdef _DEBUG
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	}

	{// エンジン起動
		Engine<D3D11Renderer>("ゲームタイトル", 1280U, 720U);
	}
}