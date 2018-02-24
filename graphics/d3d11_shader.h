#pragma once

#include <utils\memory.h>

#include <d3dx11.h>
#include <string>
#include <vector>

std::string operator""_hlsl(const char * str, size_t size)
{
	return "resource\\shader\\" + std::string(str, size) + ".hlsl";
}

class D3D11Shader
{
	friend class D3D11Renderer;

public:
	D3D11Shader(std::string hlsl) : resource_(hlsl) {}
	virtual ~D3D11Shader(void)
	{
		utils::SafeRelease(this->vs_);
		utils::SafeRelease(this->gs_);
		utils::SafeRelease(this->ps_);
		utils::SafeRelease(this->input_layout_);
	}
	
protected:
	std::vector<D3D11_INPUT_ELEMENT_DESC> element_desc_;

private:
	std::string resource_ = "";
	ID3D11VertexShader * vs_ = nullptr;
	ID3D11GeometryShader * gs_ = nullptr;
	ID3D11PixelShader * ps_ = nullptr;
	ID3D11InputLayout * input_layout_ = nullptr;
	ID3D11Buffer * constant_buffer_ = nullptr;
};