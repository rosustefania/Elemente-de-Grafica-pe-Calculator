#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

class Player
{
public:
	Player(float x, float z);
	~Player();

	float getPosX();
	float getPosZ();
	float getOffsetCameraX();
	float getOffsetCameraZ();
	float getRotatingAngle();

	void setPosX(float x);
	void setPosZ(float z);
	void setOffsetCameraX(float x);
	void setOffsetCameraZ(float z);
	void setRotatingAngle(float angle);


protected:
	float posX, posZ;
	float offsetCameraX, offsetCameraZ;
	float rotatingAngle;
	
};