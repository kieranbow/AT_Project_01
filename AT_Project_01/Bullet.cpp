#include "Bullet.h"
#include "Graphics.h"

Bullet::Bullet(Graphics* pGfx, std::string model_filePath)
{
	transform = std::make_unique<TransformComponent>();
	//rigidBody = std::make_unique<RigidBodyComponent>(transform->GetPosition(), bulletVelocity);
	//collision = std::make_unique<CollisionComponent>(rigidBody.get(), transform->GetScale());
	
	model = std::make_unique<Model>(transform.get());
	model->LoadMeshFromSource(pGfx, model_filePath);
	model->LoadShaders(pGfx, L"..\\x64\\Debug\\VS_Default.cso", L"..\\x64\\Debug\\PS_BlinnPhong.cso", pGfx->inputElemDesc, pGfx->GetSizeOfInputElemDesc());
}

void Bullet::Update(float dt)
{
	transform->Update();
	//rigidBody->Update(dt);
	//collision->Update(transform.get(), transform->GetScale());
	model->Update(dt);
}

void Bullet::Draw(Graphics* pGfx)
{
	model->Draw(pGfx);
}
