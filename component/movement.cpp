#include "movement.h"
#include <model\point_3d.h>

Component::Movement::Movement(ID3D11Model * parent) : IComponent(parent)
{

}

void Component::Movement::Update(void)
{
	this->Parent()->Get<Component::Transform>()->position_ += D3DXVECTOR3(0.0001f, 0.f, 0.f);
}