#pragma once
#include "GameObject.h"
#include "ModelComponent.h"

#include "TransformComponent.h"
#include "CollisionComponent.h"

// Forward delcares
class Graphics;
class Keyboard;
class Mouse;

class Enemy : public GameObject
{
public:
	Enemy(Graphics* pGfx);
	~Enemy() override = default;

	void LookAt(DirectX::XMFLOAT3 position);
	void MoveTo(DirectX::XMFLOAT3 position, float dt);

	void Update(float dt) final;
	void Draw(Graphics* pGfx) final;

	std::unique_ptr<TransformComponent> pTransform;
	std::unique_ptr<CollisionComponent> collision;
	std::unique_ptr<ModelComponent> pModel;

private:
	float speed = 1.0f;
	float rotationSpeed = 0.05f;

	int health = 0;
	int shield = 0;
};
