#pragma once
#include "GameObject.h"
#include "Model.h"
#include "Camera.h"

// Forward delcares
class Graphics;
class Keyboard;
class Mouse;

class Player : public GameObject
{
public:
	Player(Graphics* pGfx);
	~Player() override = default;

	void Input(Keyboard* keyboard, Mouse* mouse);
	void Update(float dt) final;
	void Draw(Graphics* pGfx) final;

	Model model;
	Model gun;
	std::shared_ptr<Camera> camera;

private:
	float speed = 0.1f;
	float rotationSpeed = 0.05f;

	int health = 0;
	int shield = 0;
};
