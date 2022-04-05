#include "HealthComponent.h"

void HealthComponent::initHealth(float health)
{
	m_currentHealth = health;
}

void HealthComponent::addHealth(float health)
{
	m_currentHealth = std::clamp(m_currentHealth + health, m_minHealth, m_maxHealth);
}

void HealthComponent::subtractHealth(float health)
{
	m_currentHealth = std::clamp(m_currentHealth - health, m_minHealth, m_maxHealth);
}

void HealthComponent::resetHealth()
{
	m_currentHealth = m_maxHealth;
}

const float& HealthComponent::getCurrentHealth() const
{
	return m_currentHealth;
}

int HealthComponent::getStatus()
{
	if (m_currentHealth <= m_minHealth) return 1;
	else return 0;
}
