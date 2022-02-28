#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

using std::pair;

typedef struct Position {
	float posX;
	float posY;
} position;

class Player
{
public:
	Player(float posX, float posY, float headBodyRadius, float armRadius);
	~Player();

	float getPositionX();
	void setPositionX(float x);
	float getPositionY();
	void setPositionY(float y);
	Mesh* getPlayerHead();
	Mesh* getPlayerFirstArm();
	Mesh* getPlayerSecondArm();
	pair<Position,Position> ArmsAfterRotation(float nextStepX, float nextStepY, float angle, float distanceArmX, float distanceArmY);

protected:
	float posX, posY;
	Mesh* head;
	Mesh* firstArm;
	Mesh* secondArm;
};