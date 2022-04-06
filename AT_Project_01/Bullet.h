#pragma once
#include "GameObject.h"

#include "TransformComponent.h"
#include "RigidBodyComponent.h"
#include "CollisionComponent.h"

#include "ModelComponent.h"

class Bullet : public GameObject
{
	public:
		Bullet(Graphics* pGfx, std::string model_filePath);
		~Bullet() override = default;

		bool DistanceFromPlayer(float distanceFromPlayer);
		void Update(float dt) final;
		void Draw(Graphics* pGfx);

		std::unique_ptr<TransformComponent> pTransform;
		std::unique_ptr<RigidBodyComponent> pRigidBody;
		std::unique_ptr<CollisionComponent> pCollision;
		
	private:
		std::unique_ptr<ModelComponent> pModel;
		DirectX::XMFLOAT3 bulletVelocity = { 0.0f, 0.0f, 0.1f };
};
