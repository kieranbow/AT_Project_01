#include "DefaultObject.h"
#include "Graphics.h"

DefaultObject::DefaultObject()
{
	transform = std::make_unique<TransformComponent>();
	model = std::make_unique<Model>(transform.get());
}

void DefaultObject::Update(float dt)
{
	transform->Update();
	model->Update(dt);
}

void DefaultObject::Draw(Graphics* pGfx)
{
	model->Draw(pGfx);
}
