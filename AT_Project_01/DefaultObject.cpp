#include "DefaultObject.h"
#include "Graphics.h"

DefaultObject::DefaultObject()
{
	transform = std::make_unique<TransformComponent>();
	model = std::make_unique<ModelComponent>(transform.get());
	collision = std::make_unique<CollisionComponent>(transform->GetPosition(), transform->GetScale());
}

void DefaultObject::Update(float dt)
{
	model->Update(dt);
}

void DefaultObject::Draw(Graphics* pGfx)
{
	model->Draw(pGfx);
}
