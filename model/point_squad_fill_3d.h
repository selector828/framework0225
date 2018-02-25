#pragma once

#include <scene.h>
#include <graphics\d3d11_model.h>

#include <vertex_buffer\one_point.h>
#include <shader\point_squad_fill_3d.h>

#include <component\movement.h>
#include <component\transform.h>
#include <component\camera.h>

namespace Model
{
	namespace Point
	{
		namespace Squad
		{
			class Fill3D : public D3D11Model<VertexBuffer::OnePoint, Shader::Point::Squad::Fill3D>
			{
			public:
				Fill3D(IScene<ID3D11Model> * parent) : D3D11Model(parent)
				{
					this->Add<Component::Transform>();
				}

			public:
				virtual void Adjust(void) override
				{
					this->constant_buffer_.w_ = this->Get<Component::Transform>()->GetMatrix();
					this->constant_buffer_.v_ = this->Parent()->Get<Component::Camera>()->View();
					this->constant_buffer_.p_ = this->Parent()->Get<Component::Camera>()->Projection();

					this->constant_buffer_.size_ = { 16.f, 9.f };
				}
			};
		}
	}
}