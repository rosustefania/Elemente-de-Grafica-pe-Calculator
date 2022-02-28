#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

typedef struct EnemyPosition {
	float posX;
	float posY;
	float initialPosX;
	float initialPosY;
} enemyPosition;

class Enemy
{
public:
	Enemy(float bodyLength, float armLength);
	~Enemy();

	Mesh* getEnemyBody();
	Mesh* getEnemyFirstArm();
	Mesh* getEnemySecondArm();
	

protected:
	Mesh* enemyBody;
	Mesh* enemyFirstArm;
	Mesh* enemySecondArm;
};