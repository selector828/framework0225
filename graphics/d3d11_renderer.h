#pragma once

#include <graphics.h>

#include <utils\memory.h>

#include <d3dx11.h>
#include <d3dx10.h>

#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3dx10.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib,"d3dCompiler.lib")

class D3D11Renderer final : public Graphics
{
public:
	D3D11Renderer(HWND hwnd, unsigned int width = 1280, unsigned int height = 720) : Graphics(hwnd, width, height)
	{
		{// デバイスとスワップチェーンの作成
			DXGI_SWAP_CHAIN_DESC sd;
			memset(&sd, 0, sizeof(sd));
			sd.BufferCount = 1;
			sd.BufferDesc.Width = this->width_;
			sd.BufferDesc.Height = this->height_;
			sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			sd.BufferDesc.RefreshRate.Numerator = 60;
			sd.BufferDesc.RefreshRate.Denominator = 1;
			sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			sd.OutputWindow = hwnd;
			sd.SampleDesc.Count = 1;
			sd.SampleDesc.Quality = 0;
			sd.Windowed = true;

			D3D_FEATURE_LEVEL feature_levels = D3D_FEATURE_LEVEL_11_0;
			D3D_FEATURE_LEVEL * feature_level = NULL;

			D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL,
				0, &feature_levels, 1, D3D11_SDK_VERSION, &sd, &this->swap_chain_, &this->device_,
				feature_level, &this->context_);
		}

		{// 各種テクスチャーとビューを作成

			{// バックバッファーからテクスチャーを取得し、テクスチャーに対しRTVを作成（その後テクスチャーを放棄）
				ID3D11Texture2D *back_buffer_tex;
				this->swap_chain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&back_buffer_tex);

				this->device_->CreateRenderTargetView(back_buffer_tex, NULL, &this->rtv_);
				utils::SafeRelease(back_buffer_tex);
			}

			{// DSV用のテクスチャーを作成し、テクスチャーに対しDSVを作成
				D3D11_TEXTURE2D_DESC descDepth;
				descDepth.Width = this->width_;
				descDepth.Height = this->height_;
				descDepth.MipLevels = 1;
				descDepth.ArraySize = 1;
				descDepth.Format = DXGI_FORMAT_D32_FLOAT;
				descDepth.SampleDesc.Count = 1;
				descDepth.SampleDesc.Quality = 0;
				descDepth.Usage = D3D11_USAGE_DEFAULT;
				descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
				descDepth.CPUAccessFlags = 0;
				descDepth.MiscFlags = 0;
				this->device_->CreateTexture2D(&descDepth, NULL, &this->dsv_tex_);

				this->device_->CreateDepthStencilView(this->dsv_tex_, NULL, &this->dsv_);
			}
		}

		{// レンダーターゲットビューと深度ステンシルビューをパイプラインにバインド
			this->context_->OMSetRenderTargets(1, &this->rtv_, this->dsv_);
		}
			
		{// ビューポートの設定
			D3D11_VIEWPORT vp;
			vp.Width = static_cast<float>(this->width_);
			vp.Height = static_cast<float>(this->height_);
			vp.MinDepth = 0.0f;
			vp.MaxDepth = 1.0f;
			vp.TopLeftX = 0;
			vp.TopLeftY = 0;
			this->context_->RSSetViewports(1, &vp);
		}

		{// ラスタライズ設定
			D3D11_RASTERIZER_DESC rdc;
			ZeroMemory(&rdc, sizeof(rdc));
			rdc.CullMode = D3D11_CULL_NONE;
			rdc.FillMode = D3D11_FILL_SOLID;
			ID3D11RasterizerState* pIr = NULL;
			this->device_->CreateRasterizerState(&rdc, &pIr);
			this->context_->RSSetState(pIr);
		}
	}

public:
	void Render(void)
	{
		D3DXCOLOR clear_color(0.f, 0.f, .5f, 1.f);
		this->context_->ClearRenderTargetView(this->rtv_, clear_color);
		this->context_->ClearDepthStencilView(this->dsv_, D3D11_CLEAR_DEPTH, 1.0f, 0);

		this->swap_chain_->Present(0, 0);
	}

public:
	~D3D11Renderer(void)
	{
		{ //DirectX解放
			utils::SafeRelease(this->rtv_);
			utils::SafeRelease(this->dsv_);
			utils::SafeRelease(this->dsv_tex_);
			utils::SafeRelease(this->context_);
			utils::SafeRelease(this->swap_chain_);
			utils::SafeRelease(this->device_);
		}
	}

private:
	IDXGISwapChain * swap_chain_ = nullptr;
	ID3D11Device * device_ = nullptr;
	ID3D11DeviceContext * context_ = nullptr;
	ID3D11DepthStencilView * dsv_;
	ID3D11Texture2D * dsv_tex_ = nullptr;
	ID3D11RenderTargetView * rtv_;
};