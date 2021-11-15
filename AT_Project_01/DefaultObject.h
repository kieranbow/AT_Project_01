#pragma once
#include "GameObject.h"

#include "Model.h"
#include "TransformComponent.h"

class Graphics;

class DefaultObject : public GameObject
{
public:
	DefaultObject();
	~DefaultObject() override = default;

	void Update(float dt) final;
	void Draw(Graphics* pGfx) final;

	std::unique_ptr<Model> model;
	std::unique_ptr<TransformComponent> transform;

};
