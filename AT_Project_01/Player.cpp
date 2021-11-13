#include "Player.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"

Player::Player(Graphics* pGfx)
{
	camera = std::make_shared<Camera>(pGfx->GetWindowSize().first, pGfx->GetWindowSize().second, 90.0f, 0.01f, 10000.0f, false);
	model.LoadMeshFromSource(pGfx, "Assets\\Model\\Dir_light.obj");
	model.LoadShaders(pGfx, L"..\\x64\\Debug\\VS_Default.cso", L"..\\x64\\Debug\\PS_PBR.cso", pGfx->inputElemDesc, pGfx->GetSizeOfInputElemDesc());
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
	float cam_x = camera->GetPosition().m128_f32[0];
	float cam_y = camera->GetPosition().m128_f32[1];
	float cam_z = camera->GetPosition().m128_f32[2];
	float cam_rot_x = camera->GetRotation().m128_f32[0];
	float cam_rot_y = camera->GetRotation().m128_f32[1];
	float cam_rot_z = camera->GetRotation().m128_f32[2];


	model.transform.SetPosition(cam_x, cam_y, cam_z);
	model.transform.SetRotation(0.0f, cam_rot_y + 1.55f, 0.0f);
	model.Update(dt);
}

void Player::Draw(Graphics* pGfx)
{
	//model.Draw(pGfx);
}
