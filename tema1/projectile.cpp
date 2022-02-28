#include "lab_m1/tema1/projectile.h"
#include "lab_m1/tema1/objects.h"


Projectile::Projectile(float length, float dist) {
    projectileLength = length;
    projectileMaxDist = dist;
    projectile = Objects::CreateSquare("projectile", glm::vec3(0.f, 0.f, 0.f), length, glm::vec3(0.0f, 0.0f, 0.0f), true);
}

Mesh* Projectile::getProjectile()
{
    return projectile;
}

float Projectile::getProjectileLength()
{
    return projectileLength;
}

float Projectile::getProjectileMaxDist()
{
    return projectileMaxDist;
}
