#pragma once

#include <graphics\d3d11_shader.h>
#include <graphics\d3d11_vertex_buffer.h>
#include <graphics\d3d11_model.h>
#include <scene.h>

#include <utils\memory.h>

#include <d3dx11.h>
#include <d3dx10.h>

#include <unordered_map>

#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3dx10.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib,"d3dCompiler.lib")

class D3D11Renderer final
{
public:
	using model_type = ID3D11Model;

public:
	D3D11Renderer(HWND hwnd, unsigned int width = 1280, unsigned int height = 720)
	{
		{// デバイスとスワップチェーンの作成
			DXGI_SWAP_CHAIN_DESC sd;
			memset(&sd, 0, sizeof(sd));
			sd.BufferCount = 1;
			sd.BufferDesc.Width = width;
			sd.BufferDesc.Height = height;
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
				descDepth.Width = width;
				descDepth.Height = height;
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
			vp.Width = static_cast<float>(width);
			vp.Height = static_cast<float>(height);
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
	void Render(Scene<model_type> * scene)
	{
		D3DXCOLOR clear_color(0.f, 0.f, .5f, 1.f);
		this->context_->ClearRenderTargetView(this->rtv_, clear_color);
		this->context_->ClearDepthStencilView(this->dsv_, D3D11_CLEAR_DEPTH, 1.0f, 0);

		for (auto model : scene->ModelList())
		{
			this->DrawModel(model);
		}

		this->swap_chain_->Present(0, 0);
	}

public:
	~D3D11Renderer(void)
	{
		{// シェーダー開放
			for (auto shader : this->shader_db_)
				utils::SafeDelete(shader.second);
		}

		{// DirectX解放
			utils::SafeRelease(this->rtv_);
			utils::SafeRelease(this->dsv_);
			utils::SafeRelease(this->dsv_tex_);
			utils::SafeRelease(this->context_);
			utils::SafeRelease(this->swap_chain_);
			utils::SafeRelease(this->device_);
		}
	}

public:
	template<class _VertexBuffer>
	void RegistVertexBuffer(void)
	{
		D3D11VertexBuffer * vertex_buffer = new _VertexBuffer;

		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = vertex_buffer->stride_ * vertex_buffer->vertice_num_;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA init_data;
		init_data.pSysMem = vertex_buffer->vertices_;
		
		if (FAILED(this->device_->CreateBuffer(&bd, &init_data, &vertex_buffer->buffer_)))
		{
			MessageBoxA(0, "頂点バッファーの生成に失敗しました。", 0, MB_OK);
			PostQuitMessage(0);
		}

		this->vertex_buffer_db_[typeid(_VertexBuffer).name()] = vertex_buffer;
	}

	template<class _Shader>
	void RegistShader(void)
	{
		D3D11Shader * shader = new _Shader;

		ID3DBlob * compiled_shader = nullptr;
		ID3DBlob * errors = nullptr;

		{// バーテックシェーダー作成
			if (FAILED(D3DX11CompileFromFileA(shader->resource_.c_str(), nullptr, nullptr, "VS", "vs_5_0",
				D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION, 0, nullptr, &compiled_shader, &errors, nullptr)))
			{
				char*p = (char*)errors->GetBufferPointer();
				MessageBoxA(0, p, 0, MB_OK);
				PostQuitMessage(0);
			}
			else
			{
				if (FAILED(this->device_->CreateVertexShader(compiled_shader->GetBufferPointer(), compiled_shader->GetBufferSize(),
					nullptr, &shader->vs_)))
				{
					PostQuitMessage(0);
				}
			}
			utils::SafeRelease(errors);
		}

		{// インプットレイアウト作成
			if (FAILED(this->device_->CreateInputLayout(shader->element_desc_.data(), shader->element_desc_.size(),
				compiled_shader->GetBufferPointer(), compiled_shader->GetBufferSize(), &shader->input_layout_)))
			{
				PostQuitMessage(0);
			}
			utils::SafeRelease(compiled_shader);
		}

		{// ジオメトリシェーダー作成
			if (FAILED(D3DX11CompileFromFileA(shader->resource_.c_str(), nullptr, nullptr, "GS", "gs_5_0",
				D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION, 0, nullptr, &compiled_shader, &errors, nullptr)))
			{
				char*p = (char*)errors->GetBufferPointer();
				MessageBoxA(0, p, 0, MB_OK);
			}
			else
			{
				if (FAILED(this->device_->CreateGeometryShader(compiled_shader->GetBufferPointer(), compiled_shader->GetBufferSize(),
					nullptr, &shader->gs_)))
				{
					PostQuitMessage(0);
				}
			}
			utils::SafeRelease(errors);
		}

		{// ピクセルシェーダー作成
			if (FAILED(D3DX11CompileFromFileA(shader->resource_.c_str(), nullptr, nullptr, "PS", "ps_5_0",
				D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION, 0, nullptr, &compiled_shader, &errors, nullptr)))
			{
				char*p = (char*)errors->GetBufferPointer();
				MessageBoxA(0, p, 0, MB_OK);
			}
			else
			{
				if (FAILED(this->device_->CreatePixelShader(compiled_shader->GetBufferPointer(), compiled_shader->GetBufferSize(),
					nullptr, &shader->ps_)))
				{
					PostQuitMessage(0);
				}
			}
			utils::SafeRelease(errors);
		}

		//コンスタントバッファー作成　ここでは変換行列渡し用
		D3D11_BUFFER_DESC cb;
		cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cb.ByteWidth = sizeof(_Shader::CB);
		cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cb.MiscFlags = 0;
		cb.StructureByteStride = 0;
		cb.Usage = D3D11_USAGE_DYNAMIC;

		this->device_->CreateBuffer(&cb, nullptr, &shader->constant_buffer_);

		this->shader_db_[typeid(_Shader).name()] = shader;
	}

public:
	void DrawModel(ID3D11Model * model)
	{
		auto shader = this->shader_db_[model->shader_];
		auto vertex_buffer = this->vertex_buffer_db_[model->vertex_buffer_];
		D3D11_MAPPED_SUBRESOURCE data;
		if (SUCCEEDED(this->context_->Map(shader->constant_buffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &data)))
		{
			model->UpdateConstantBuffer(data);
			this->context_->Unmap(shader->constant_buffer_, 0);
		}

		//使用するシェーダーのセット
		this->context_->VSSetShader(shader->vs_, nullptr, 0);
		this->context_->PSSetShader(shader->ps_, nullptr, 0);
		//このコンスタントバッファーをどのシェーダーで使うか
		this->context_->VSSetConstantBuffers(0, 1, &shader->constant_buffer_);
		this->context_->PSSetConstantBuffers(0, 1, &shader->constant_buffer_);
		//頂点インプットレイアウトをセット
		this->context_->IASetInputLayout(shader->input_layout_);
		//プリミティブ・トポロジーをセット
		this->context_->IASetPrimitiveTopology(vertex_buffer->topology_);
		this->context_->IASetVertexBuffers(0, 1, &vertex_buffer->buffer_, &vertex_buffer->stride_, &vertex_buffer->offset_);
		//プリミティブをレンダリング
		this->context_->Draw(vertex_buffer->vertice_num_, 0);
	}

private:
	std::unordered_map<std::string, D3D11Shader*> shader_db_;
	std::unordered_map<std::string, D3D11VertexBuffer*> vertex_buffer_db_;

private:
	IDXGISwapChain * swap_chain_ = nullptr;
	ID3D11Device * device_ = nullptr;
	ID3D11DeviceContext * context_ = nullptr;
	ID3D11DepthStencilView * dsv_;
	ID3D11Texture2D * dsv_tex_ = nullptr;
	ID3D11RenderTargetView * rtv_;
};