#pragma once
#include "GameObject.h"
#include "TransformComponent.h"
#include "Model.h"

class SkyBox : GameObject
{
public:
	SkyBox(Graphics* pGfx, std::string texture_filePath);
	~SkyBox() override = default;

	SkyBox(const SkyBox&) = delete;
	SkyBox& operator=(const SkyBox&) = delete;

	void Update(float dt) final;
	void Draw(Graphics* pGfx) final;

	std::unique_ptr<TransformComponent> transform;
	std::unique_ptr<Model> model;

private:
};

