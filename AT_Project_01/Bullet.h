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

		bool distanceCull(float distanceFromPlayer, XMFLOAT3 playerPosition);
		void Update(float dt) final;
		void Draw(Graphics* pGfx);

		std::unique_ptr<TransformComponent> pTransform;
		std::unique_ptr<RigidBodyComponent> pRigidBody;
		std::unique_ptr<CollisionComponent> pCollision;
		std::unique_ptr<ModelComponent> pModel;

		void isFired(bool fired);
		bool hasFired();
	private:
		DirectX::XMFLOAT3 m_velocity = { 0.01f, 0.01f, 0.01f };
		bool m_fired = false;
};
