#include "Bullet.h"
#include "Graphics.h"

#include <cmath>

Bullet::Bullet(Graphics* pGfx, std::string model_filePath)
{
	// Init the components for the bullet
	pTransform = std::make_unique<TransformComponent>();
	pRigidBody = std::make_unique<RigidBodyComponent>(pTransform->GetPosition(), m_velocity);
	pCollision = std::make_unique<CollisionComponent>(pRigidBody->GetPosition(), pTransform->GetScale());

	// Create the model and assign it a material and shader
	pModel = std::make_unique<ModelComponent>(pTransform.get());

	// Gold material
	Material_PBR material;
	material.baseColor = { 1.0f, 0.84f, 0.0f, 1.0f };
	material.ambientOcculsion = 1.0f;
	material.metallic = 1.0f;
	material.roughness = 0.5f;

	pModel->SetPBRMaterial(material);
	pModel->LoadMeshFromSource(pGfx, model_filePath);
	pModel->LoadShaders(pGfx, L"VS_Default.cso", L"PS_PBRMaterial.cso", pGfx->inputElemDesc, pGfx->GetSizeOfInputElemDesc());
}

bool Bullet::distanceCull(float distanceFromPlayer, XMFLOAT3 playerPosition)
{
	// Lambda distance function
	auto distance = [=](XMFLOAT3 lhs, XMFLOAT3 rhs)
	{
		float x = (lhs.x - rhs.x) * (lhs.x - rhs.x);
		float y = (lhs.y - rhs.y) * (lhs.y - rhs.y);
		float z = (lhs.z - rhs.z) * (lhs.z - rhs.z);
		return std::sqrtf(x + y + z);
	};

	// Return if the distance between the bullets transform is greater than a point
	return distance(pTransform->GetPosition(), playerPosition) >= distanceFromPlayer;
}

void Bullet::Update(float dt)
{
	// Update all the components
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

void Bullet::isFired(bool fired)
{
	m_fired = fired;
}

bool Bullet::hasFired()
{
	return m_fired;
}
