#pragma once

#include <d3dx9.h>
#include <d3d9.h>

#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"d3dCompiler.lib")

class D3D9Renderer final
{
public:
	D3D9Renderer(HWND hwnd, unsigned int width, unsigned int height)
	{
		D3DDISPLAYMODE d3ddm;

		{// Direct3D9�I�u�W�F�N�g�̍쐬
			this->d3d_ = ::Direct3DCreate9(D3D_SDK_VERSION);
		}

		{// ���݂̃f�B�X�v���C���[�h���擾
			this->d3d_->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);
		}
		
		{// �f�o�C�X�̃v���[���e�[�V�����p�����[�^��������
			memset(&this->d3d_present_param_, 0, sizeof(D3DPRESENT_PARAMETERS));
			this->d3d_present_param_.BackBufferCount = 1;
			this->d3d_present_param_.Windowed = true;
			this->d3d_present_param_.BackBufferFormat = d3ddm.Format;
			this->d3d_present_param_.SwapEffect = D3DSWAPEFFECT_DISCARD;
			this->d3d_present_param_.EnableAutoDepthStencil = TRUE;
			this->d3d_present_param_.AutoDepthStencilFormat = D3DFMT_D16;
		}

		{// �f�o�C�X�̐���
			this->d3d_->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_HAL,
				hwnd,
				D3DCREATE_HARDWARE_VERTEXPROCESSING,
				&this->d3d_present_param_, &this->device_);
		}
	}
	~D3D9Renderer(void)
	{
		{// DirectX���
			utils::SafeRelease(this->device_);
			utils::SafeRelease(this->d3d_);
		}
	}

public:
	void Render(void)
	{
		// VP�Ɛ[�x�o�b�t�@�̃N���A�ƃX�e���V���o�b�t�@�̍폜
		this->device_->Clear(0, NULL, D3DCLEAR_TARGET |	D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

		{// �`��
			if (SUCCEEDED(this->device_->BeginScene())) {

				this->device_->EndScene();
			}

			if (FAILED(this->device_->Present(0, 0, 0, 0))) {
				// �f�o�C�X�������畜�A
				this->device_->Reset(&this->d3d_present_param_);
			}
		}	
	}

private:
	IDirect3D9 * d3d_;
	IDirect3DDevice9 * device_;
	D3DPRESENT_PARAMETERS d3d_present_param_;

private:
	unsigned int width_ = 0;
	unsigned int height_ = 0;
};