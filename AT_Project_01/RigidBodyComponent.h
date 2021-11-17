#pragma once
#include <DirectXMath.h>
using namespace DirectX;

class RigidBodyComponent
{
	public:
		RigidBodyComponent() = default;
		RigidBodyComponent(float x_pos, float y_pos, float z_pos, float x_vel, float y_vel, float z_vel);
		RigidBodyComponent(XMFLOAT3 position, XMFLOAT3 velocity);
		~RigidBodyComponent() = default;

		RigidBodyComponent(const RigidBodyComponent&) = delete;
		RigidBodyComponent& operator=(const RigidBodyComponent&) = delete;

		void Update(float dt);

		void SetPosition(XMFLOAT3 position);
		XMFLOAT3 GetPosition() const;

		void SetVelocity(XMFLOAT3 newVelocity);
		XMFLOAT3 GetVelocity() const;

	private:
		XMFLOAT3 f_position;
		XMFLOAT3 f_velocity;
		float const gravity = 9.8f;
};
