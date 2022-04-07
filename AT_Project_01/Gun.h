#pragma once
#include "GameObject.h"
#include "Bullet.h"

class Graphics;
class Mouse;

class Gun : public GameObject
{
	public:
		Gun(Graphics* pGfx);
		~Gun() override = default;

		void reload();
		void fire(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 velocity);

		void Update(float dt) final;
		void Draw(Graphics* pGfx) final;

		std::unique_ptr<TransformComponent> pTransform;

		const std::vector<std::unique_ptr<Bullet>>& getBulletPool() const;

	private:
		std::unique_ptr<ModelComponent> model;
		std::vector<std::unique_ptr<Bullet>> bulletPool;

		size_t currentBullet = 0;
		bool empty = false;

		float distanceCull = 100.0f;
};
