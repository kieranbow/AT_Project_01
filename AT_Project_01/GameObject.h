#pragma once
class Graphics;

class GameObject
{
	public:
		GameObject() = default;
		virtual ~GameObject() = default;

		//GameObject(const GameObject&) = delete;
		//GameObject& operator=(const GameObject&) = delete;

		virtual void Update(float dt) {}
		virtual void Draw(Graphics* pGfx) {}

	private:
};
