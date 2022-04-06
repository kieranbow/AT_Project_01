#include "Bullet.h"
#include "Graphics.h"

Bullet::Bullet(Graphics* pGfx, std::string model_filePath)
{
	pTransform = std::make_unique<TransformComponent>();
	pRigidBody = std::make_unique<RigidBodyComponent>(pTransform->GetPosition(), bulletVelocity);
	pCollision = std::make_unique<CollisionComponent>(pRigidBody->GetPosition(), pTransform->GetScale());
	
	pModel = std::make_unique<ModelComponent>(pTransform.get());
	pModel->LoadMeshFromSource(pGfx, model_filePath);
	pModel->LoadShaders(pGfx, L"..\\x64\\Debug\\VS_Default.cso", L"..\\x64\\Debug\\PS_BlinnPhong.cso", pGfx->inputElemDesc, pGfx->GetSizeOfInputElemDesc());
}

bool Bullet::DistanceFromPlayer(float distanceFromPlayer)
{
	return pTransform->GetPosition().z >= distanceFromPlayer;
}

void Bullet::Update(float dt)
{
	pTransform->Update();

	pRigidBody->SetPosition(pTransform->GetPosition());
	pRigidBody->Update(dt);

	pCollision->Update(pTransform->GetPosition(), pTransform->GetScale());

	pModel->SetPosition(pRigidBody->GetPosition());
	pModel->Update(dt);
}

void Bullet::Draw(Graphics* pGfx)
{
	pModel->Draw(pGfx);
}
