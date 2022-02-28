#include "lab_m1/tema1/enemy.h"
#include "lab_m1/tema1/objects.h"


Enemy::Enemy(float bodyLength, float armLength)
{	
	enemyBody = Objects::CreateSquare("enemyBody", glm::vec3(0.f, 0.f, 0.f), bodyLength, glm::vec3(0.62f, 0.76f, 0.90f), true);
	enemyFirstArm = Objects::CreateSquare("enemyFirstArm", glm::vec3(0.f, 0.f, 0.f), armLength, glm::vec3(0.5f, 0.90f, 0.70f), true);
	enemySecondArm = Objects::CreateSquare("enemySecondArm", glm::vec3(0.f, 0.f, 0.f), armLength, glm::vec3(0.5f, 0.90f, 0.70f), true);
}

Mesh* Enemy::getEnemyBody()
{
	return enemyBody;
}

Mesh* Enemy::getEnemyFirstArm()
{
	return enemyFirstArm;
}

Mesh* Enemy::getEnemySecondArm()
{
	return enemySecondArm;
}

Enemy::~Enemy()
{
	delete enemyBody;
	delete enemyFirstArm;
	delete enemySecondArm;

}

