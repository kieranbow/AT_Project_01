#include "RigidBodyComponent.h"

RigidBodyComponent::RigidBodyComponent(float x_pos, float y_pos, float z_pos, float x_vel, float y_vel, float z_vel) : f_position(x_pos, y_pos, z_pos), f_velocity(x_vel, y_vel, z_vel)
{
}

RigidBodyComponent::RigidBodyComponent(XMFLOAT3 position, XMFLOAT3 velocity) : f_position(position), f_velocity(velocity)
{
}

void RigidBodyComponent::Update(float dt)
{
	f_position.x += f_velocity.x * dt;
	f_position.y += f_velocity.y * dt;
	f_position.z += f_velocity.z * dt;
}

XMFLOAT3 RigidBodyComponent::GetPosition() const
{
	return f_position;
}

void RigidBodyComponent::SetVelocity(XMFLOAT3 newVelocity)
{
	f_velocity = newVelocity;
}

XMFLOAT3 RigidBodyComponent::GetVelocity() const
{
	return f_velocity;
}
