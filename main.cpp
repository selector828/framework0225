#include <engine.h>

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <window.h>
//graphics
#include <graphics\d3d11_renderer.h>

//shader
//#include <shader\point_2d.h>
//#include <shader\point_3d.h>
//#include <shader\point_squad_tex_3d.h>

//vertex_buffer
#include <vertex_buffer\one_point.h>

//scene
#include <scene\opening.h>

//#include <graphics\d3d9_renderer.h>

int __stdcall WinMain(HINSTANCE, HINSTANCE, char *, int)
{
	{// �������[���[�N���o
#ifdef _DEBUG
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	}

	{// �G���W���N��
		Engine<Window, D3D11Renderer> main;

		main.Begin("�Q�[���^�C�g��", 1280U, 720U);

		{// ������
			{// ���[�h
				main.Graphics()->RegistShader<Shader::Point3D>();
				main.Graphics()->RegistShader<Shader::Point::Squad::Tex3D>();
				main.Graphics()->RegistShader<Shader::Point2D>();
				main.Graphics()->RegistVertexBuffer<VertexBuffer::OnePoint>();
			}

			{// �ݒ�
				main.SetCurrentScene<Scene::Opening>();
			}
		}

		main.Run();

		{// �I��
			
		}

		main.End();
	}
}