#include "DefaultObject.h"
#include "Graphics.h"

DefaultObject::DefaultObject()
{
	pTransform	= std::make_unique<TransformComponent>();
	pModel		= std::make_unique<ModelComponent>(pTransform.get());
	pCollision	= std::make_unique<CollisionComponent>(pTransform->GetPosition(), pTransform->GetScale());
}

void DefaultObject::Update(float dt)
{
	pModel->Update(dt);
	pCollision->Update(pTransform->GetPosition(), pTransform->GetScale());
}

void DefaultObject::Draw(Graphics* pGfx)
{
	pModel->Draw(pGfx);
}
