#pragma once

#include <scene.h>
#include <graphics\d3d11_model.h>

#include <vertex_buffer\one_point.h>
#include <shader\point_2d.h>

#include <component\movement.h>
#include <component\transform.h>
#include <component\camera.h>

namespace Model
{
	class Point2D : public D3D11Model<VertexBuffer::OnePoint, Shader::Point2D>
	{
	public:
		Point2D(IScene<ID3D11Model> * parent) : D3D11Model(parent)
		{
			this->Add<Component::Transform>();
		}

	public:
		virtual void Adjust(void) override
		{
			this->constant_buffer_.w_ = this->Get<Component::Transform>()->GetMatrix();
			this->constant_buffer_.view_port_.x = 1280.f;
			this->constant_buffer_.view_port_.y = 720.f;
		}

	};
}