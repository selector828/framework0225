#pragma once

#include <scene.h>
#include <graphics\d3d11_model.h>
#include <model\point3d.h>
#include <component\camera.h>

namespace Scene
{
	class Opening : public IScene<ID3D11Model>
	{
	public:
		Opening(void)
		{
			this->Add<Component::Camera>();
			this->Create<Model::Point3D>();
		}
	};
}