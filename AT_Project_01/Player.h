#pragma once
#include "GameObject.h"
#include "ModelComponent.h"
#include "Camera.h"

#include "RigidBodyComponent.h"
#include "TransformComponent.h"
#include "CollisionComponent.h"
#include "HealthComponent.h"

#include "Gun.h"

// Forward delcares
class Graphics;
class Keyboard;
class Mouse;

class Player : public GameObject
{
	public:
		Player(Graphics* pGfx);
		~Player() override = default;

		// Main functions
		void Input(Keyboard* keyboard, Mouse* mouse);
		void Update(float dt) final;
		void Draw(Graphics* pGfx) final;

		std::shared_ptr<Camera> camera;

		// Components
		std::unique_ptr<RigidBodyComponent> pRigidBody;
		std::unique_ptr<TransformComponent> pTransform;
		std::unique_ptr<CollisionComponent> pCollision;
		std::unique_ptr<HealthComponent> pHealth;

	private:
		Graphics* pGraphics;

		std::unique_ptr<ModelComponent> pModel;
		std::unique_ptr<Gun> gun;

		DirectX::XMFLOAT3 velocity = {0.2f, 0.2f, 0.2f};
	
		static constexpr float rotationSpeed = 0.05f;
};
