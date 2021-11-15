#pragma once
#include "GameObject.h"
#include "Model.h"
#include "Camera.h"

#include "RigidBodyComponent.h"
#include "TransformComponent.h"

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

private:
	std::unique_ptr<Model> pModel;


	DirectX::XMFLOAT3 velocity = {0.2f, 0.2f, 0.2f};

	float rotationSpeed = 0.05f;

	int health = 0;
	int shield = 0;
};
