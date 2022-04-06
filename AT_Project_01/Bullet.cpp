#include "Bullet.h"
#include "Graphics.h"

#include <cmath>

Bullet::Bullet(Graphics* pGfx, std::string model_filePath)
{
	pTransform = std::make_unique<TransformComponent>();
	pRigidBody = std::make_unique<RigidBodyComponent>(pTransform->GetPosition(), m_velocity);
	pCollision = std::make_unique<CollisionComponent>(pRigidBody->GetPosition(), pTransform->GetScale());
	
	pModel = std::make_unique<ModelComponent>(pTransform.get());
	pModel->LoadMeshFromSource(pGfx, model_filePath);
	pModel->LoadShaders(pGfx, L"..\\x64\\Debug\\VS_Default.cso", L"..\\x64\\Debug\\PS_BlinnPhong.cso", pGfx->inputElemDesc, pGfx->GetSizeOfInputElemDesc());
}

bool Bullet::distanceCull(float distanceFromPlayer, XMFLOAT3 playerPosition)
{
	auto distance = [=](XMFLOAT3 lhs, XMFLOAT3 rhs)
	{
		float x = (lhs.x - rhs.x) * (lhs.x - rhs.x);
		float y = (lhs.y - rhs.y) * (lhs.y - rhs.y);
		float z = (lhs.z - rhs.z) * (lhs.z - rhs.z);
		return std::sqrtf(x + y + z);
	};

	return distance(pTransform->GetPosition(), playerPosition) >= distanceFromPlayer;
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

void Bullet::setBulletVelocity(float velX, float velY, float velZ)
{
	m_velocity.x = velX;
	m_velocity.y = velY;
	m_velocity.z = velZ;
}

void Bullet::setBulletVelocity(XMFLOAT3 velocity)
{
	m_velocity = velocity;
}

const XMFLOAT3& Bullet::getBulletVelocity() const
{
	return m_velocity;
}
