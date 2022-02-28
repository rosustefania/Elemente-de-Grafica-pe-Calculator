#include "lab_m1/tema1/player.h"
#include "lab_m1/tema1/objects.h"



Player::Player(float x, float y, float headBodyRadius, float armRadius)
{	
	posX = x;
	posY = y;
	head = Objects::CreateCircle("head", glm::vec3(0.f, 0.f, 0.f), headBodyRadius, 2000, glm::vec3(1.0f, 0.46f, 0.5f));
	firstArm = Objects::CreateCircle("firstArm", glm::vec3(0.f, 0.f, 0.f), armRadius, 2000, glm::vec3(0.81f, 0.0f, 0.39f));
	secondArm = Objects::CreateCircle("secondArm", glm::vec3(0.f, 0.f, 0.f), armRadius, 2000, glm::vec3(0.81f, 0.0f, 0.39f));
}

float Player::getPositionX()
{
	return posX;
}

void Player::setPositionX(float x)
{
	 posX = x;
}

float Player::getPositionY()
{
	return posY;
}

void Player::setPositionY(float y)
{
	posY = y;
}

Mesh* Player::getPlayerHead()
{
	return head;
}

Mesh* Player::getPlayerFirstArm()
{
	return firstArm;
}

Mesh* Player::getPlayerSecondArm()
{
	return secondArm;
}

/*
* gets arms' coordinates after player rotates with a given angle
*/
pair<Position, Position> Player::ArmsAfterRotation(float nextStepX, float nextStepY, float angle, float distanceArmX, float distanceArmY)
{
	Position firstArm, secondArm;
	float firstX, firstY, secondX, secondY;

	if (angle < 0) {
		firstX = (-distanceArmX) * cos(angle) - (-distanceArmY) * sin(angle) + (posX + nextStepX);
		firstY = (-distanceArmX) * cos(angle) + (-distanceArmY) * sin(angle) + (posY + nextStepY);

    	secondX = distanceArmX * cos(angle) - (-distanceArmY) * sin(angle) + (posX + nextStepX);
		secondY = distanceArmX * cos(angle) + (-distanceArmY) * sin(angle) + (posY + nextStepY);
	}
	else {
		firstX = (-distanceArmX) * cos(angle) + (-distanceArmY) * sin(angle) + (posX + nextStepX);
		firstY = (-distanceArmX) * cos(angle) - (-distanceArmY) * sin(angle) + (posY + nextStepY);

		secondX = distanceArmX * cos(angle) + (-distanceArmY) * sin(angle) + (posX + nextStepX);
		secondY = distanceArmX * cos(angle) - (-distanceArmY) * sin(angle) + (posY + nextStepY);
	}

	firstArm.posX = firstX;
	firstArm.posY = firstY;

	secondArm.posX = secondX;
	secondArm.posY = secondY;

	pair<Position, Position> pair = std::make_pair(firstArm, secondArm);
	return pair;
}


Player::~Player()
{
	delete head;
	delete firstArm;
	delete secondArm;
}