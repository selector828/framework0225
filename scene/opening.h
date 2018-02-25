#pragma once

#include <scene.h>
#include <graphics\d3d11_model.h>
#include <model\point_2d.h>
#include <model\point_3d.h>
#include <model\point_squad_tex_3d.h>
#include <component\camera.h>

namespace Scene
{
	class Opening : public IScene<ID3D11Model>
	{
	public:
		Opening(void)
		{
			this->Add<Component::Camera>();
			this->Create<Model::Point::Squad::Tex3D>();
		}
	};
}