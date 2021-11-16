#pragma once
#include "GameObject.h"
#include "Bullet.h"

class Graphics;
class Mouse;

class Gun : public GameObject
{
	public:
		Gun();
		~Gun() override = default;

		void fire(Graphics* pGfx, Mouse* mouse);

		void Update(float dt) final;
		void Draw(Graphics* pGfx) final;

		std::unique_ptr<TransformComponent> transform;


	private:
		std::unique_ptr<Model> model;
		std::vector<std::unique_ptr<Bullet>> bulletPool;

		int rate_of_fire = 0;
		float distanceCull = 400.0f;

		float offset = 0.0f;
};
