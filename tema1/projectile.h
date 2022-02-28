#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

typedef struct ProjectilePosition {
    float currentPosX;
    float currentPosY;
    float intialPosX;
    float intialPosY;
} projectilePosition;

typedef struct TraveledDistance {
    float distX;
    float distY;
    float angle;
    float destX;
    float destY;

} traveledDistance;

class Projectile
{
public:
	Projectile(float length, float dist);
	~Projectile();

    Mesh* getProjectile();
    float getProjectileLength();
    float getProjectileMaxDist();

protected:
    float projectileLength;
    float projectileMaxDist;
    Mesh* projectile;

};