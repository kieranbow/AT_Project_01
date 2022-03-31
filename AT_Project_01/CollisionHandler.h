#pragma once
#include "CollisionComponent.h"

class CollisionHandler
{
	public:
		CollisionHandler() = default;
		~CollisionHandler() = default;

		// Checks AABB collision with two collision component
		static bool DetectAABB(CollisionComponent* a, CollisionComponent* b);
		static bool AABBIntersect(const XMVECTOR& mina, const XMVECTOR& maxa, const XMVECTOR& minb, const XMVECTOR& maxb, XMVECTOR& ncoll, float& dcoll, int& fcoll);
};
