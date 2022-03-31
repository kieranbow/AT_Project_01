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
