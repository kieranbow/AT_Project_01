#pragma once
#include "GameObject.h"

#include "TransformComponent.h"
#include "RigidBodyComponent.h"
#include "CollisionComponent.h"

#include "Model.h"

class Bullet : public GameObject
{
	public:
		Bullet(Graphics* pGfx, std::string model_filePath);
		~Bullet() override = default;

		void Update(float dt) final;
		void Draw(Graphics* pGfx);

		std::unique_ptr<TransformComponent> transform;
		std::unique_ptr<RigidBodyComponent> rigidBody;
		//std::unique_ptr<CollisionComponent> collision;
		
	private:
		std::unique_ptr<Model> model;
		DirectX::XMFLOAT3 bulletVelocity = { 0.0f, 0.0f, 0.0f };
};
