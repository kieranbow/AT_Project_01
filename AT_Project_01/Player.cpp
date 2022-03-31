#include "Player.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"

Player::Player(Graphics* pGfx)
{
	pGraphics = pGfx;

	// Camera
	camera = std::make_shared<Camera>(pGraphics->GetWindowSize().first, pGraphics->GetWindowSize().second, 90.0f, 0.01f, 10000.0f, false);
	camera->SetPosition({ 0.0f, 2.0f, 0.0f, 0.0f });

	// Components
	pTransform = std::make_unique<TransformComponent>();
	pRigidBody = std::make_unique<RigidBodyComponent>(pTransform->GetPosition(), velocity);
	pCollision = std::make_unique<CollisionComponent>(pTransform->GetPosition(), pTransform->GetScale());

	// Model
	pModel = std::make_unique<ModelComponent>(pTransform.get());
	pModel->LoadMeshFromSource(pGraphics, "Assets\\Model\\cube_proj.obj");
	pModel->LoadShaders(pGraphics, L"..\\x64\\Debug\\VS_Default.cso", L"..\\x64\\Debug\\PS_PBR.cso", pGraphics->inputElemDesc, pGraphics->GetSizeOfInputElemDesc());

	// Gun
	gun = std::make_unique<Gun>();
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
		//camera->UpdatePosition({ 0.0f, 1.0f * pRigidBody->GetVelocity().y, 0.0f, 0.0f });

		gun->fire(pGraphics, camera->GetPositionFloat());
	}
	//if (keyboard->IsKeyPressed('Z'))
	//{
	//	camera->UpdatePosition({ 0.0f, -1.0f * pRigidBody->GetVelocity().y, 0.0f, 0.0f });
	//}

	// https://stackoverflow.com/questions/28026562/sfml-cant-get-the-projectile-shooting-in-the-right-direction
	// https://pybullet.org/Bullet/phpBB3/viewtopic.php?t=11584
	// https://learnopengl.com/Getting-started/Camera

	//if (mouse->IsLeftBtnDown())
	//{
	//	gun->fire(pGraphics, camera->GetPositionFloat());
	//}

	// Camera rotation
	if (keyboard->IsKeyPressed('Q')) // '37' Left arrow
	{
		camera->UpdateRotation({ 0.0f, -0.05f, 0.0f, 0.0f });
	}
	if (keyboard->IsKeyPressed('E')) // '39' Right arrow
	{
		camera->UpdateRotation({ 0.0f, 0.05f, 0.0f, 0.0f });
	}

	//if (keyboard->IsKeyPressed(38)) // Up arrow
	//{
	//	camera->UpdateRotation({ -0.05f, 0.0f, 0.0f, 0.0f });
	//}
	//if (keyboard->IsKeyPressed(40)) // Down arrow
	//{
	//	camera->UpdateRotation({ 0.05f, 0.0f, 0.0f, 0.0f });
	//}

}

void Player::Update(float dt)
{
	float cam_x = camera->GetPosition().m128_f32[0];
	float cam_y = camera->GetPosition().m128_f32[1];
	float cam_z = camera->GetPosition().m128_f32[2];
	float cam_rot_x = camera->GetRotation().m128_f32[0];
	float cam_rot_y = camera->GetRotation().m128_f32[1];
	float cam_rot_z = camera->GetRotation().m128_f32[2];

	gun->Update(dt);

	pTransform->Update();

	//pRigidBody->Update(dt);

	XMFLOAT3 position;
	XMStoreFloat3(&position, camera->GetPosition());

	pCollision->Update(position, pTransform->GetScale());

	//pModel->SetPosition(camera->GetPositionFloat());
	//pModel->SetRotation({ 0.0f, cam_rot_y + 1.55f, 0.0f });
	//pModel->Update(dt);

	//pModel->transform.SetPosition(cam_x, cam_y, cam_z);
	//pModel->transform.SetRotation(0.0f, cam_rot_y + 1.55f, 0.0f);
	//pModel->Update(dt);
}

void Player::Draw(Graphics* pGfx)
{
	//pModel->Draw(pGfx);
	gun->Draw(pGfx);
}
