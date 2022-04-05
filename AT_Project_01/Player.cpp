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
	pTransform	= std::make_unique<TransformComponent>();
	pRigidBody	= std::make_unique<RigidBodyComponent>(pTransform->GetPosition(), velocity);
	pCollision	= std::make_unique<CollisionComponent>(pTransform->GetPosition(), pTransform->GetScale());
	pHealth		= std::make_unique<HealthComponent>();

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

		XMFLOAT4X4 matrix;
		XMStoreFloat4x4(&matrix, camera->GetViewMatrix());

		float cam_x = matrix._13 * 0.05f; // 0.05 is to slow down the bullets by a certain amount
		float cam_y = matrix._23 * 0.05f;
		float cam_z = matrix._33 * 0.05f;

		XMFLOAT3 camPosition;
		XMStoreFloat3(&camPosition, camera->GetPosition());

		gun->fire(pGraphics, camPosition, {cam_x, cam_y, cam_z});
	}
	if (keyboard->IsKeyPressed('Z'))
	{
		//camera->UpdatePosition({ 0.0f, -1.0f * pRigidBody->GetVelocity().y, 0.0f, 0.0f });
	}

	// Camera rotation
	if (keyboard->IsKeyPressed(37)) // Left arrow
	{
		camera->UpdateRotation({ 0.0f, -rotationSpeed, 0.0f, 0.0f });
	}
	if (keyboard->IsKeyPressed(39)) // Right arrow
	{
		camera->UpdateRotation({ 0.0f, rotationSpeed, 0.0f, 0.0f });
	}

	if (keyboard->IsKeyPressed(38)) // Up arrow
	{
		camera->UpdateRotation({ -rotationSpeed, 0.0f, 0.0f, 0.0f });
	}
	if (keyboard->IsKeyPressed(40)) // Down arrow
	{
		camera->UpdateRotation({ rotationSpeed, 0.0f, 0.0f, 0.0f });
	}
}

void Player::Update(float dt)
{
	gun->Update(dt);

	pTransform->Update();

	//pRigidBody->Update(dt);

	XMFLOAT3 position;
	XMStoreFloat3(&position, camera->GetPosition());

	pCollision->Update(position, pTransform->GetScale());

	pModel->SetPosition(position);
	pModel->SetRotation({ camera->GetRotation().m128_f32[0], camera->GetRotation().m128_f32[1], camera->GetRotation().m128_f32[2] });
	pModel->Update(dt);

}

void Player::Draw(Graphics* pGfx)
{
	pModel->Draw(pGfx);
	gun->Draw(pGfx);
}
