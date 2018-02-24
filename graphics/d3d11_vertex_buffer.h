#pragma once

#include <utils\memory.h>

#include <d3dx11.h>
#include <string>
#include <vector>

class D3D11VertexBuffer
{
	friend class D3D11Renderer;

public:
	virtual ~D3D11VertexBuffer(void)
	{
		utils::SafeRelease(this->buffer_);
	}

protected:
	template<class _VertexType>
	void SetVertices(D3D11_PRIMITIVE_TOPOLOGY topology, std::vector<_VertexType> * vertices)
	{
		this->topology_ = topology;
		this->stride_ = sizeof(_VertexType);
		this->vertices_ = vertices->data();
		this->vertice_num_ = vertices->size();
	}

private:
	D3D11_PRIMITIVE_TOPOLOGY topology_ = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
	ID3D11Buffer * buffer_ = nullptr;
	unsigned int stride_ = 0;
	unsigned int offset_ = 0;
	unsigned int vertice_num_ = 0;
	void * vertices_ = nullptr;
};