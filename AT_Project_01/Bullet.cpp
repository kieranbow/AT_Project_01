#include "Bullet.h"
#include "Graphics.h"

Bullet::Bullet(Graphics* pGfx, std::string model_filePath)
{
	transform = std::make_unique<TransformComponent>();
	rigidBody = std::make_unique<RigidBodyComponent>(transform->GetPosition(), bulletVelocity);
	collision = std::make_unique<CollisionComponent>(rigidBody->GetPosition(), transform->GetScale());
	
	model = std::make_unique<ModelComponent>(transform.get());
	model->LoadMeshFromSource(pGfx, model_filePath);
	model->LoadShaders(pGfx, L"..\\x64\\Debug\\VS_Default.cso", L"..\\x64\\Debug\\PS_BlinnPhong.cso", pGfx->inputElemDesc, pGfx->GetSizeOfInputElemDesc());
}

bool Bullet::DistanceFromPlayer(float distanceFromPlayer)
{
	return transform->GetPosition().z >= distanceFromPlayer;
}

void Bullet::Update(float dt)
{
	transform->Update();

	rigidBody->SetPosition(transform->GetPosition());
	rigidBody->Update(dt);

	model->SetPosition(rigidBody->GetPosition());
	model->Update(dt);
}

void Bullet::Draw(Graphics* pGfx)
{
	model->Draw(pGfx);
}
