#include <engine.h>

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <window.h>
#include <graphics\d3d11_renderer.h>
#include <shader\point_3d.h>
#include <vertex_buffer\one_point.h>
#include <model\point3d.h>
//#include <graphics\d3d9_renderer.h>

int __stdcall WinMain(HINSTANCE, HINSTANCE, char *, int)
{
	{// メモリーリーク検出
#ifdef _DEBUG
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	}

	{// エンジン起動
		Engine<Window, D3D11Renderer> main;
		Model::Point3D model;

		main.Begin("ゲームタイトル", 1280U, 720U);

		{// 初期化
			main.Graphics()->RegistShader<Shader::Point3d>();
			main.Graphics()->RegistVertexBuffer<VertexBuffer::OnePoint>();
			main.Graphics()->AddModel(&model);
		}

		main.Run();

		{// 終了

		}

		main.End();
	}
}