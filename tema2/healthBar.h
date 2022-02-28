#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

class HealthBar
{
public:
	HealthBar(float x1, float y1, float x2, float y2, float width, float height);
	~HealthBar();

	Mesh* getHealthBarBorder();
	Mesh* getHealthBar();

	float getHealthBarBorderPosX();
	float getHealthBarBorderPosY();

	void setHealthBarBorderPosX(float x);
	void setHealthBarBorderPosY(float y);

	float getHealthBarPosX();
	float getHealthBarPosY();

	void setHealthBarPosX(float x);
	void setHealthBarPosY(float y);

	float getWidth();
	float getHeight();

	void setWidth(float w);
	void setHeight(float h);



protected:
	float posX, posY;
	float borderPosX, borderPosY;
	Mesh* border;
	Mesh* bar;
	float width, height;

};