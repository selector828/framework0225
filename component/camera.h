#pragma once

#include <component.h>
#include <graphics\d3d11_model.h>
#include <scene.h>

#include <d3dx10.h>

namespace Component
{
	class Camera : public IComponent<IScene<ID3D11Model>>
	{
	public:
		D3DXVECTOR3 eye_;
		D3DXVECTOR3 at_;
		D3DXVECTOR3 up_;

	public:
		float fov_;
		float aspect_;
		float near_;
		float far_;

	public:
		Camera(IScene<ID3D11Model> * parent) : IComponent(parent)
		{
			this->eye_ = { 0.0f, 0.0f, -2.0f };
			this->at_ = { 0.0f, 0.0f, 0.0f };
			this->up_ = { 0.0f, 1.0f, 0.0f };

			this->fov_ = static_cast<float>(D3DX_PI) / 4.f;
			this->aspect_ = 16.f / 9.f;
			this->near_ = 0.1f;
			this->far_ = 100.f;
		}
		D3DXMATRIX View(void)
		{
			D3DXMATRIX ret;
			D3DXMatrixLookAtLH(&ret, &this->eye_, &this->at_, &this->up_);
			return ret;
		}
		D3DXMATRIX Projection(void)
		{
			D3DXMATRIX ret;
			D3DXMatrixPerspectiveFovLH(&ret, this->fov_, this->aspect_, this->near_, this->far_);
			return ret;
		}

	private:
		void Update(void) override {}
		void Pause(void) override {};
		void Always(void) override {};
	};
}