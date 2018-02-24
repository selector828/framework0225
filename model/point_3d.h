#pragma once

#include <scene.h>
#include <graphics\d3d11_model.h>

#include <vertex_buffer\one_point.h>
#include <shader\point_3d.h>

#include <component\movement.h>
#include <component\transform.h>
#include <component\camera.h>

namespace Model
{
	class Point3D : public D3D11Model<VertexBuffer::OnePoint, Shader::Point3D>
	{
	public:
		Point3D(IScene<ID3D11Model> * parent) : D3D11Model(parent)
		{
			this->Add<Component::Transform>();
			this->Add<Component::Movement>();
		}

	public:
		void Adjust(void) override
		{
			this->constant_buffer_.w_ = this->Get<Component::Transform>()->GetMatrix();
			this->constant_buffer_.v_ = this->Parent()->Get<Component::Camera>()->View();
			this->constant_buffer_.p_ = this->Parent()->Get<Component::Camera>()->Projection();
		}
		
	};
}