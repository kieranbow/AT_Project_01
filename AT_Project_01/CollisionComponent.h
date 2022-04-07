#pragma once
#include <DirectXMath.h>
#include <array>
using namespace DirectX;

// Forward declares
class RigidBodyComponent;
class TransformComponent;

// A simple collision component that creates bounding boxes around objects given their position and size.
// Currently, the class can only detect AABB collision between two objects.
class CollisionComponent
{
	public:
		CollisionComponent(XMFLOAT3 position, XMFLOAT3 scale);
		~CollisionComponent() = default;

		CollisionComponent(const CollisionComponent&) = delete;
		CollisionComponent& operator=(const CollisionComponent&) = delete;

		// Updates the aabb struct using the transform and scale.
		void Update(XMFLOAT3 position, XMFLOAT3 scale);

		// Defines the bounding box for collision
		struct AABB
		{
			float min_x;
			float min_y;
			float min_z;

			float max_x;
			float max_y;
			float max_z;
		} aabb;

		XMVECTOR min;
		XMVECTOR max;

		std::array<XMVECTOR, 6> faces;

	private:

};
