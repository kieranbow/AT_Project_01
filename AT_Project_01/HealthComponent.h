#pragma once
#include <algorithm>

class HealthComponent
{
	private:
		enum class State
		{
			ALIVE,
			DEAD
		};

	public:
		HealthComponent() = default;
		~HealthComponent() = default;

		HealthComponent(const HealthComponent&) = delete;
		HealthComponent& operator=(const HealthComponent&) = delete;

		// Sets the inital health
		void initHealth(float health);

		// Add health to the current health
		void addHealth(float health);

		// Substract to the current health
		void subtractHealth(float health);

		// Reset health to max health
		void resetHealth();

		const float& getCurrentHealth() const;

		// Returns a state which describes where something is dead or alive
		// based on the current health
		int getStatus();

	private:
		float m_currentHealth = 100.0f;
		static constexpr float m_maxHealth = 100.0f;
		static constexpr float m_minHealth = 0.0f;
};
