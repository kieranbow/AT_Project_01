#include "Gun.h"
#include "Graphics.h"
#include "Mouse.h"

Gun::Gun()
{
}

void Gun::fire(Graphics* pGfx, DirectX::XMFLOAT3 position, Mouse* mouse)
{
	if (mouse->IsLeftBtnDown())
	{
		std::unique_ptr<Bullet> bullet = std::make_unique<Bullet>(pGfx, "Assets\\Model\\cube_proj.obj");
		bullet->transform->SetPosition(position.x, position.y, position.z); // spawn on barrel of gun
		bulletPool.push_back(std::move(bullet));
		
		bullet.release();

		OutputDebugStringA("Bullet spawned");
	}
}

void Gun::Update(float dt)
{
	for (auto& bullet : bulletPool)
	{
		bullet->transform->SetPosition(
			bullet->transform->GetPosition().x, 
			bullet->transform->GetPosition().y, 
			bullet->transform->GetPosition().z * (0.5f * dt));
		bullet->Update(dt);
	}
}

void Gun::Draw(Graphics* pGfx)
{
	for (auto& bullet : bulletPool)
	{
		bullet->Draw(pGfx);
	}
}
