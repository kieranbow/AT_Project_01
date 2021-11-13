#include "Enemy.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"

Enemy::Enemy(Graphics* pGfx)
{
	model.LoadMeshFromSource(pGfx, "Assets\\Model\\cube_proj.obj");
	model.LoadShaders(pGfx, L"..\\x64\\Debug\\VS_Default.cso", L"..\\x64\\Debug\\PS_BlinnPhong.cso", pGfx->inputElemDesc, pGfx->GetSizeOfInputElemDesc());
	model.LoadTextures(pGfx, "Assets\\Texture\\icon.png");
}

void Enemy::LookAt(DirectX::XMFLOAT3 position)
{
	if (position.x == model.transform.GetPosition().x
		&& position.y == model.transform.GetPosition().y
		&& position.z == model.transform.GetPosition().z)
	{
		return;
	}

	position.x = model.transform.GetPosition().x - position.x;
	position.y = model.transform.GetPosition().y - position.y;
	position.z = model.transform.GetPosition().z - position.z;

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

	model.transform.SetRotation(0.0f, yaw, 0.0f);

}

void Enemy::MoveTo(DirectX::XMFLOAT3 position, float dt)
{
	float xPos = model.transform.GetPosition().x;
	float zPos = model.transform.GetPosition().z;

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
		model.transform.SetPosition(position.x + velocity, 0.0f, position.z + velocity);
	}
}

void Enemy::Update(float dt)
{


	model.Update(dt);
}

void Enemy::Draw(Graphics* pGfx)
{
	model.Draw(pGfx);
}