#pragma once
#include "GameObject.h"

#include "ModelComponent.h"
#include "TransformComponent.h"
#include "CollisionComponent.h"

class Graphics;

class DefaultObject : public GameObject
{
	public:
		DefaultObject();
		~DefaultObject() override = default;

		void Update(float dt) final;
		void Draw(Graphics* pGfx) final;

		std::unique_ptr<ModelComponent> pModel;
		std::unique_ptr<TransformComponent> pTransform;
		std::unique_ptr<CollisionComponent> pCollision;
};
