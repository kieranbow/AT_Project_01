#pragma once
class Graphics;

class GameObject
{
	public:
		GameObject() = default;
		virtual ~GameObject() = default;

		virtual void Update(float dt) {}
		virtual void Draw(Graphics* pGfx) {}

	private:
};
