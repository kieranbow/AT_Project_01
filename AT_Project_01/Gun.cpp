#include "Gun.h"
#include "Graphics.h"
#include "Mouse.h"

Gun::Gun()
{
}

void Gun::fire(Graphics* pGfx, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 velocity)
{
	std::unique_ptr<Bullet> bullet = std::make_unique<Bullet>(pGfx, "Assets\\Model\\cube_proj.obj");
	bullet->pTransform->SetPosition(position.x, position.y, position.z);
	bullet->pTransform->SetScale(2.0f, 2.0f, 2.0f);
	bullet->pRigidBody->SetVelocity(velocity);
	bulletPool.push_back(std::move(bullet));
	bullet.release();
}

void Gun::Update(float dt)
{
	for (auto& bullet : bulletPool)
	{
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

const std::vector<std::unique_ptr<Bullet>>& Gun::getBulletPool() const
{
	return bulletPool;
}
