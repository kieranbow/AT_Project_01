#pragma once

class CollisionComponent;

class CollisionHandler
{
public:
	CollisionHandler() = default;
	~CollisionHandler() = default;

	// Checks AABB collision with two collision component
	static bool DetectAABB(CollisionComponent* a, CollisionComponent* b);

};
