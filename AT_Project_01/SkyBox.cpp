#include "SkyBox.h"
#include "Graphics.h"

SkyBox::SkyBox(Graphics* pGfx, std::string texture_filePath)
{
	transform = std::make_unique<TransformComponent>();
	
	model = std::make_unique<Model>(transform.get());
	model->LoadMeshFromSource(pGfx, "Assets\\Model\\inner_sphere.obj");
	model->LoadShaders(pGfx, L"..\\x64\\Debug\\VS_Default.cso", L"..\\x64\\Debug\\PS_unlit.cso", pGfx->inputElemDesc, pGfx->GetSizeOfInputElemDesc());
	model->LoadTextures(pGfx, texture_filePath);
	model->SetScale({ 5000.0f, 5000.f, 5000.0f });
}

void SkyBox::Update(float dt)
{
	transform->Update();
	model->Update(dt);
}

void SkyBox::Draw(Graphics* pGfx)
{
	model->Draw(pGfx);
}
