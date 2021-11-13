#pragma once
#include "GameObject.h"
#include "Model.h"

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

	Model model;

private:
	float speed = 1.0f;
	float rotationSpeed = 0.05f;

	int health = 0;
	int shield = 0;
};
