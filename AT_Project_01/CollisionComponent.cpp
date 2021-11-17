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
}

void CollisionComponent::Update(XMFLOAT3 position, XMFLOAT3 scale)
{
	aabb.min_x = position.x - scale.x;
	aabb.min_y = position.y - scale.y;
	aabb.min_z = position.z - scale.z;

	aabb.max_x = position.x + scale.x;
	aabb.max_y = position.y + scale.y;
	aabb.max_z = position.z + scale.z;
}
