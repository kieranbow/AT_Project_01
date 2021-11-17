#include "CollisionHandler.h"
#include "CollisionComponent.h"

bool CollisionHandler::DetectAABB(CollisionComponent* a, CollisionComponent* b)
{
    return
		a->aabb.min_x <= b->aabb.max_x && a->aabb.max_x >= b->aabb.min_x &&
		a->aabb.min_y <= b->aabb.max_y && a->aabb.max_y >= b->aabb.min_y &&
		a->aabb.min_z <= b->aabb.max_z && a->aabb.max_z >= b->aabb.min_z;
}
