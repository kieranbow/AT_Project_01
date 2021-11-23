#pragma once
#include "GameObject.h"

#include "ModelComponent.h"
#include "TransformComponent.h"

class Graphics;

class DefaultObject : public GameObject
{
public:
	DefaultObject();
	~DefaultObject() override = default;

	void Update(float dt) final;
	void Draw(Graphics* pGfx) final;

	std::unique_ptr<ModelComponent> model;
	std::unique_ptr<TransformComponent> transform;

};
