#pragma once

#include <graphics\d3d11_vertex_buffer.h>

#include <d3dx11.h>
#include <d3dx10.h>
#include <string>
#include <vector>


namespace VertexBuffer
{
	class OnePoint : public D3D11VertexBuffer
	{
	public:
		OnePoint(void)
		{
			this->vert_.emplace_back(D3DXVECTOR3(0.f, 0.f, 0.f));
			this->SetVertices(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &this->vert_);
		}

	private:
		std::vector<D3DXVECTOR3> vert_;
	};
}