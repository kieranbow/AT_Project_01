#include "CollisionComponent.h"
#include "RigidBodyComponent.h"
#include "TransformComponent.h"

CollisionComponent::CollisionComponent(RigidBodyComponent* pRigidBody, XMFLOAT3 scale)
{
	aabb.min_x = pRigidBody->GetPosition().x - scale.x;
	aabb.min_y = pRigidBody->GetPosition().y - scale.y;
	aabb.min_z = pRigidBody->GetPosition().z - scale.z;

	aabb.max_x = pRigidBody->GetPosition().x + scale.x;
	aabb.max_y = pRigidBody->GetPosition().y + scale.y;
	aabb.max_z = pRigidBody->GetPosition().z + scale.z;
}

CollisionComponent::CollisionComponent(TransformComponent* pTransform, XMFLOAT3 scale)
{
	aabb.min_x = pTransform->GetPosition().x - scale.x;
	aabb.min_y = pTransform->GetPosition().y - scale.y;
	aabb.min_z = pTransform->GetPosition().z - scale.z;

	aabb.max_x = pTransform->GetPosition().x + scale.x;
	aabb.max_y = pTransform->GetPosition().y + scale.y;
	aabb.max_z = pTransform->GetPosition().z + scale.z;
}

void CollisionComponent::Update(TransformComponent* pTransform, XMFLOAT3 scale)
{
	aabb.min_x = pTransform->GetPosition().x - scale.x;
	aabb.min_y = pTransform->GetPosition().y - scale.y;
	aabb.min_z = pTransform->GetPosition().z - scale.z;

	aabb.max_x = pTransform->GetPosition().x + scale.x;
	aabb.max_y = pTransform->GetPosition().y + scale.y;
	aabb.max_z = pTransform->GetPosition().z + scale.z;
}

bool CollisionComponent::AABBIntersect(CollisionComponent* collision)
{
	return(
		collision->aabb.min_x <= aabb.max_x && collision->aabb.max_x >= aabb.min_x &&
		collision->aabb.min_y <= aabb.max_y && collision->aabb.max_y >= aabb.min_y &&
		collision->aabb.min_z <= aabb.max_z && collision->aabb.max_z >= aabb.min_z);
}
