#include "Gun.h"
#include "Graphics.h"
#include "Mouse.h"

Gun::Gun(Graphics* pGfx)
{
	pTransform = std::make_unique<TransformComponent>();

	for (size_t i = 0; i < 30; i++)
	{
		std::unique_ptr<Bullet> bullet = std::make_unique<Bullet>(pGfx, "Assets\\Model\\sphere.obj");
		bullet->pTransform->SetPosition(0.0f, -10.0f, 0.0f);
		bullet->pTransform->SetScale(0.5f, 0.5f, 0.5f);
		bullet->pRigidBody->SetVelocity({0.0f, 0.0f, 0.0f});
		bulletPool.push_back(std::move(bullet));
		bullet.release();
	}
}

void Gun::reload()
{
	empty = false;
	currentBullet = 0;
}

void Gun::fire(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 velocity)
{
	if (currentBullet == bulletPool.size())
	{
		empty = true;
	}
	if (!empty && !bulletPool[currentBullet]->hasFired())
	{
		bulletPool[currentBullet]->pModel->enableRendering();
		bulletPool[currentBullet]->pTransform->SetPosition(position.x, position.y, position.z);
		bulletPool[currentBullet]->pRigidBody->SetVelocity(velocity);
		bulletPool[currentBullet]->isFired(true);
		currentBullet++;
	}
}

const std::vector<std::unique_ptr<Bullet>>& Gun::getBulletPool() const
{
	return bulletPool;
}

void Gun::Update(float dt)
{
	for (auto& bullet : bulletPool)
	{
		bullet->Update(dt);

		if (bullet->distanceCull(distanceCull, pTransform->GetPosition()))
		{
			bullet->pTransform->SetPosition(0.0f, -10.0f, 0.0f);
			bullet->pRigidBody->SetVelocity({ 0.0f, 0.0f, 0.0f });
			bullet->isFired(false);
			bullet->pModel->disableRendering();
		}
	}
}

void Gun::Draw(Graphics* pGfx)
{
	for (auto& bullet : bulletPool)
	{
		bullet->Draw(pGfx);
	}
}
