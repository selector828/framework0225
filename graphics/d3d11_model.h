#pragma once

#pragma warning(disable: 4346)

#include <d3dx11.h>
#include <string>
#include <typeinfo>

class ID3D11Model
{
	friend class D3D11Renderer;

public:
	ID3D11Model(std::string vertex_buffer, std::string shader) : vertex_buffer_(vertex_buffer), shader_(shader) {}

private:
	virtual void UpdateConstantBuffer(D3D11_MAPPED_SUBRESOURCE & data) = 0;

private:
	std::string vertex_buffer_;
	std::string shader_;
};

template<class _VertexBuffer, class _Shader>
class D3D11Model : public ID3D11Model
{
public:
	typename _Shader::CB constant_buffer_;

public:
	D3D11Model(void) : ID3D11Model(typeid(_VertexBuffer).name(), typeid(_Shader).name()) {}
	void UpdateConstantBuffer(D3D11_MAPPED_SUBRESOURCE & data) override
	{
		memcpy_s(data.pData, data.RowPitch, (void*)(&this->constant_buffer_), sizeof(this->constant_buffer_));
	}
};