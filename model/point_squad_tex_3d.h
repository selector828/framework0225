#pragma once

#include <scene.h>
#include <graphics\d3d11_model.h>

#include <vertex_buffer\one_point.h>
#include <shader\point_squad_tex_3d.h>

#include <component\movement.h>
#include <component\transform.h>
#include <component\camera.h>

namespace Model
{
	namespace Point
	{
		namespace Squad
		{
			class Tex3D : public D3D11Model<VertexBuffer::OnePoint, Shader::Point::Squad::Tex3D>
			{
			public:
				Tex3D(IScene<ID3D11Model> * parent) : D3D11Model(parent)
				{
					{// コンポーネント追加
						this->Add<Component::Transform>();
					}

					{// テクスチャ設定
						this->SetTexture(0, "dragon_stay");
					}

					{//ステート設定
						this->SetBlendState(BLEND_STATE::ALPHA);
					}

					{// コンスタントバッファー
						this->constant_buffer_.split_ = this->Texture(0)->split_;
						this->constant_buffer_.size_ = this->Texture(0)->size_;
						this->constant_buffer_.pattern_ = 0;
					}
				}

			public:
				virtual void Adjust(void) override
				{
					this->constant_buffer_.w_ = this->Get<Component::Transform>()->GetMatrix();
					this->constant_buffer_.v_ = this->Parent()->Get<Component::Camera>()->View();
					this->constant_buffer_.p_ = this->Parent()->Get<Component::Camera>()->Projection();

					this->constant_buffer_.pattern_ += .02f;
				}
			};
		}
	}
}