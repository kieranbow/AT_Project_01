#include "CollisionComponent.h"
#include "RigidBodyComponent.h"
#include "TransformComponent.h"

CollisionComponent::CollisionComponent(XMFLOAT3 position, XMFLOAT3 scale)
{
	aabb.min_x = position.x - scale.x;
	aabb.min_y = position.y - scale.y;
	aabb.min_z = position.z - scale.z;

	aabb.max_x = position.x + scale.x;
	aabb.max_y = position.y + scale.y;
	aabb.max_z = position.z + scale.z;

	min = DirectX::XMVectorSet(position.x - scale.x, position.y - scale.y, position.z - scale.z, 0.0f);
	max = DirectX::XMVectorSet(position.x + scale.x, position.y + scale.y, position.z + scale.z, 0.0f);

	faces[0] = XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);	// 'left' face normal (-x direction)
	faces[1] = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);		// 'right' face normal (+x direction)
	faces[2] = XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f);	// 'bottom' face normal (-y direction)
	faces[3] = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);		// 'top' face normal (+y direction)
	faces[4] = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);	// 'far' face normal (-z direction)
	faces[5] = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);		// 'near' face normal (+x direction)
}

void CollisionComponent::Update(XMFLOAT3 position, XMFLOAT3 scale)
{
	aabb.min_x = position.x - scale.x;
	aabb.min_y = position.y - scale.y;
	aabb.min_z = position.z - scale.z;

	aabb.max_x = position.x + scale.x;
	aabb.max_y = position.y + scale.y;
	aabb.max_z = position.z + scale.z;

	min = DirectX::XMVectorSet(position.x - scale.x, position.y - scale.y, position.z - scale.z, 0.0f);
	max = DirectX::XMVectorSet(position.x + scale.x, position.y + scale.y, position.z + scale.z, 0.0f);
}
