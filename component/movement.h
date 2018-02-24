#pragma once

#include <component.h>
#include <graphics\d3d11_model.h>

namespace Component
{
	class Movement : public IComponent<ID3D11Model>
	{
	public:
		Movement(ID3D11Model * parent);

	private:
		void Update(void) override;
		void Pause(void) override {}
		void Always(void) override {}
	};
}