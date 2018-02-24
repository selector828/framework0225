#pragma once

#include <component.h>
#include <graphics\d3d11_model.h>

#include <d3dx10.h>

namespace Component
{
	class Transform : public IComponent<ID3D11Model>
	{
	public:
		Transform(ID3D11Model * parent) : IComponent(parent)
		{
			this->position_ = { 0.f, 0.f, 0.f };
			this->rotation_ = { 0.f, 0.f, 0.f };
			this->scale_ = { 1.f, 1.f, 1.f };
		}

	public:
		D3DXVECTOR3 position_;
		D3DXVECTOR3 rotation_;
		D3DXVECTOR3 scale_;

	public:
		D3DXVECTOR3 center_rotation_;
		D3DXVECTOR3 center_scale_;

	public:
		D3DXMATRIX GetMatrix(void)
		{
			D3DXMATRIX out;
			D3DXQUATERNION rot;
			D3DXQuaternionRotationYawPitchRoll(&rot, this->rotation_.y, this->rotation_.x, this->rotation_.z);
			D3DXMatrixTransformation(&out, &this->center_scale_, nullptr, &this->scale_, &this->center_rotation_, &rot, &this->position_);
			return out;
		}

	private:
		void Update(void) override {}
		void Pause(void) override {}
		void Always(void) override {}
	};
}