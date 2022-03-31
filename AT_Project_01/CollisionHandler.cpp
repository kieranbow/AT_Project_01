#include "CollisionHandler.h"


bool CollisionHandler::DetectAABB(CollisionComponent* a, CollisionComponent* b)
{
	if (a->aabb.min_x <= b->aabb.max_x && a->aabb.max_x >= b->aabb.min_x) return true;
	if (a->aabb.min_y <= b->aabb.max_y && a->aabb.max_y >= b->aabb.min_y) return true;
	if (a->aabb.min_z <= b->aabb.max_z && a->aabb.max_z >= b->aabb.min_z) return true;
}


bool CollisionHandler::AABBIntersect(const DirectX::XMVECTOR& mina,  // min vector of boxa                   
	const XMVECTOR& maxa,  // max vector of boxa                    
	const XMVECTOR& minb,  // min vector of boxb                   
	const XMVECTOR& maxb,  // max vector of boxb                   
	XMVECTOR& ncoll,       // normal of collision.                   
	float& dcoll,        // depth of intersection.                   
	int& fcoll)          // face intersected.
{
	// https://gamedev.net/forums/topic/567310-platform-game-collision-detection/
	// the normal of each face.    
	static const DirectX::XMVECTOR faces[6] =
	{ 
		XMVectorSet(-1.0f,  0.0f,  0.0f, 0.0f), // 'left' face normal (-x direction)        
		XMVectorSet( 1.0f,  0.0f,  0.0f, 0.0f), // 'right' face normal (+x direction)        
		XMVectorSet( 0.0f, -1.0f,  0.0f, 0.0f), // 'bottom' face normal (-y direction)        
		XMVectorSet( 0.0f,  1.0f,  0.0f, 0.0f), // 'top' face normal (+y direction)        
		XMVectorSet( 0.0f,  0.0f, -1.0f, 0.0f), // 'far' face normal (-z direction)        
		XMVectorSet( 0.0f,  0.0f,  1.0f, 0.0f), // 'near' face normal (+x direction)    
	};        

	XMFLOAT3 maxbf;
	XMStoreFloat3(&maxbf, maxb);

	XMFLOAT3 maxaf;
	XMStoreFloat3(&maxaf, maxa);

	XMFLOAT3 minbf;
	XMStoreFloat3(&minbf, minb);

	XMFLOAT3 minaf;
	XMStoreFloat3(&minaf, mina);

	// distance of collided box to the face.    
	float distances[6] = 
	{         
		(maxbf.x - minaf.x), // distance of box 'b' to face on 'left' side of 'a'.        
		(maxaf.x - minbf.x), // distance of box 'b' to face on 'right' side of 'a'.        
		(maxbf.y - minaf.y), // distance of box 'b' to face on 'bottom' side of 'a'.        
		(maxaf.y - minbf.y), // distance of box 'b' to face on 'top' side of 'a'.
		(maxbf.z - minaf.z), // distance of box 'b' to face on 'far' side of 'a'.    
		(maxaf.z - minbf.z), // distance of box 'b' to face on 'near' side of 'a'.     
	};  

	// scan each face, make sure the box intersects,    
	// and take the face with least amount of intersection    
	// as the collided face.    
	for(int i = 0; i < 6; i ++)    
	{        
		// box does not intersect face. So boxes don't intersect at all.        
		if(distances[i] < 0.0f) return false;        
		// face of least intersection depth. That's our candidate.        
		if((i == 0) || (distances[i] < dcoll))   
		{            
			fcoll = i;
			ncoll = faces[i];            
			dcoll = distances[i];        
		}            
	}
	return true;
}