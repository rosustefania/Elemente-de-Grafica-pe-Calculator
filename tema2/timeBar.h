#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

class TimeBar
{
public:
	TimeBar(float x1, float y1, float x2, float y2, float width, float height);
	~TimeBar();

	Mesh* getTimeBarBorder();
	Mesh* getTimeBar();

	float getTimeBarBorderPosX();
	float getTimeBarBorderPosY();

	void setTimeBarBorderPosX(float x);
	void setTimeBarBorderPosY(float y);

	float getTimeBarPosX();
	float getTimeBarPosY();

	void setTimeBarPosX(float x);
	void setTimeBarPosY(float y);

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