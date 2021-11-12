#include "Player.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"

Player::Player(Graphics* pGfx)
{
	camera = std::make_shared<Camera>(pGfx->GetWindowSize().first, pGfx->GetWindowSize().second, 90.0f, 0.01f, 10000.0f, false);
	model.LoadMeshFromSource(pGfx, "Assets\\Model\\cube_proj.obj");
}

void Player::Input(Keyboard* keyboard, Mouse* mouse)
{
	if (keyboard->IsKeyPressed('W'))
	{
		camera->UpdatePosition(camera->GetDirection().v_forward * speed);
	}
	if (keyboard->IsKeyPressed('S'))
	{
		camera->UpdatePosition(camera->GetDirection().v_backward * speed);
	}
	if (keyboard->IsKeyPressed('A'))
	{
		camera->UpdatePosition(camera->GetDirection().v_left * speed);
	}
	if (keyboard->IsKeyPressed('D'))
	{
		camera->UpdatePosition(camera->GetDirection().v_right * speed);
	}
	if (keyboard->IsKeyPressed(VK_SPACE))
	{
		camera->UpdatePosition({ 0.0f, 1.0f * speed, 0.0f, 0.0f });
	}
	if (keyboard->IsKeyPressed('Z'))
	{
		camera->UpdatePosition({ 0.0f, -1.0f * speed, 0.0f, 0.0f });
	}
	if (keyboard->IsKeyPressed('Q'))
	{
		camera->UpdateRotation({ 0.0f, -0.05f, 0.0f, 0.0f });
	}
	if (keyboard->IsKeyPressed('E'))
	{
		camera->UpdateRotation({ 0.0f, 0.05f, 0.0f, 0.0f });
	}
}

void Player::Update(float dt)
{
	model.Update(dt);
}

void Player::Draw(Graphics* pGfx)
{
}
