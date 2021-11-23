#include "Enemy.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"

Enemy::Enemy(Graphics* pGfx)
{
	pTransform = std::make_unique<TransformComponent>();
	collision = std::make_unique<CollisionComponent>(pTransform->GetPosition(), pTransform->GetScale());

	pModel = std::make_unique<ModelComponent>(pTransform.get());

	pModel->LoadMeshFromSource(pGfx, "Assets\\Model\\billboard.obj");
	pModel->LoadShaders(pGfx, L"..\\x64\\Debug\\VS_Default.cso", L"..\\x64\\Debug\\PS_BlinnPhong.cso", pGfx->inputElemDesc, pGfx->GetSizeOfInputElemDesc());
	pModel->LoadTextures(pGfx, "Assets\\Texture\\icon.png", DXGI_FORMAT_R8G8B8A8_UNORM);
}

void Enemy::LookAt(DirectX::XMFLOAT3 position)
{
	if (position.x == pModel->GetPosition().x
		&& position.y == pModel->GetPosition().y
		&& position.z == pModel->GetPosition().z)
	{
		return;
	}

	position.x = pModel->GetPosition().x - position.x;
	position.y = pModel->GetPosition().y - position.y;
	position.z = pModel->GetPosition().z - position.z;

	float pitch = 0.0f;
	if (position.y != 0.0f)
	{
		const float distance = sqrtf(position.x * position.x + position.z * position.z);
		pitch = atanf(position.y / distance);
	}

	float yaw = 0.0f;
	if (position.x != 0.0f)
	{
		yaw = atanf(position.x / position.z);
	}

	if (position.z > 0)
	{
		yaw += DirectX::XM_PI;
	}

	pModel->SetRotation({ 0.0f, yaw, 0.0f });

}

void Enemy::MoveTo(DirectX::XMFLOAT3 position, float dt)
{
	float xPos = pModel->GetPosition().x;
	float zPos = pModel->GetPosition().z;

	//DirectX::XMVECTOR direction = DirectX::XMVectorSet(position.x - xPos, 0.0f, position.z - zPos, 0.0f);
	//DirectX::XMVector3Normalize(direction);

	//DirectX::XMVECTOR velocity = direction * speed * dt;

	//model.transform.SetPosition(position.x + velocity.m128_f32[0], 0.0f, position.z + velocity.m128_f32[2]);


	float distance_x = position.x - xPos;
	float distance_z = position.z - zPos;

	// normalize
	float hypotenuse = sqrtf((distance_x * distance_x) + (distance_z * distance_z));

	float velocity = hypotenuse * speed * dt;

	if (hypotenuse < 10)
	{
		pModel->SetPosition({ position.x + velocity, 0.0f, position.z + velocity });
	}
}

void Enemy::Update(float dt)
{
	collision->Update(pTransform->GetPosition(), pTransform->GetScale());
	pModel->Update(dt);
}

void Enemy::Draw(Graphics* pGfx)
{
	pModel->Draw(pGfx);
}
