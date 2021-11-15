#include "Player.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"

Player::Player(Graphics* pGfx)
{
	camera = std::make_shared<Camera>(pGfx->GetWindowSize().first, pGfx->GetWindowSize().second, 90.0f, 0.01f, 10000.0f, false);


	pTransform = std::make_unique<TransformComponent>();
	pRigidBody = std::make_unique<RigidBodyComponent>(pTransform->GetPosition(), velocity);
	pCollision = std::make_unique<CollisionComponent>(pTransform.get(), pTransform->GetScale());

	pModel = std::make_unique<Model>(pTransform.get());
	pModel->LoadMeshFromSource(pGfx, "Assets\\Model\\cube_proj.obj");
	pModel->LoadShaders(pGfx, L"..\\x64\\Debug\\VS_Default.cso", L"..\\x64\\Debug\\PS_PBR.cso", pGfx->inputElemDesc, pGfx->GetSizeOfInputElemDesc());
}

void Player::Input(Keyboard* keyboard, Mouse* mouse)
{
	// When key is pressed update the transform based on position of camera
	if (keyboard->IsKeyPressed('W'))
	{
		camera->UpdatePosition(camera->GetDirection().v_forward * pRigidBody->GetVelocity().z);
	}
	if (keyboard->IsKeyPressed('S'))
	{
		camera->UpdatePosition(camera->GetDirection().v_backward * pRigidBody->GetVelocity().z);
	}
	if (keyboard->IsKeyPressed('A'))
	{
		camera->UpdatePosition(camera->GetDirection().v_left * pRigidBody->GetVelocity().x);
	}
	if (keyboard->IsKeyPressed('D'))
	{
		camera->UpdatePosition(camera->GetDirection().v_right * pRigidBody->GetVelocity().x);
	}
	if (keyboard->IsKeyPressed(VK_SPACE))
	{
		camera->UpdatePosition({ 0.0f, 1.0f * pRigidBody->GetVelocity().y, 0.0f, 0.0f });
	}
	if (keyboard->IsKeyPressed('Z'))
	{
		camera->UpdatePosition({ 0.0f, -1.0f * pRigidBody->GetVelocity().y, 0.0f, 0.0f });
	}


	// Camera rotation
	if (keyboard->IsKeyPressed(37)) // Left arrow
	{
		camera->UpdateRotation({ 0.0f, -0.05f, 0.0f, 0.0f });
	}
	if (keyboard->IsKeyPressed(39)) // Right arrow
	{
		camera->UpdateRotation({ 0.0f, 0.05f, 0.0f, 0.0f });
	}
	if (keyboard->IsKeyPressed(38)) // Up arrow
	{
		camera->UpdateRotation({ -0.05f, 0.0f, 0.0f, 0.0f });
	}
	if (keyboard->IsKeyPressed(40)) // Down arrow
	{
		camera->UpdateRotation({ 0.05f, 0.0f, 0.0f, 0.0f });
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

	pTransform->Update();
	pRigidBody->Update(dt);
	pCollision->Update(pTransform.get(), pTransform->GetScale());

	pModel->SetPosition(camera->GetPositionFloat());
	pModel->SetRotation({ 0.0f, cam_rot_y + 1.55f, 0.0f });
	pModel->Update(dt);

	//pModel->transform.SetPosition(cam_x, cam_y, cam_z);
	//pModel->transform.SetRotation(0.0f, cam_rot_y + 1.55f, 0.0f);
	//pModel->Update(dt);
}

void Player::Draw(Graphics* pGfx)
{
	pModel->Draw(pGfx);
}
