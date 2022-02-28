#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

class ScoreBar
{
public:
	ScoreBar(float x1, float y1, float x2, float y2, float width, float height);
	~ScoreBar();

	Mesh* getScoreBarBorder();
	Mesh* getScoreBar();

	float getScoreBarBorderPosX();
	float getScoreBarBorderPosY();

	void setScoreBarBorderPosX(float x);
	void setScoreBarBorderPosY(float y);

	float getScoreBarPosX();
	float getScoreBarPosY();

	void setScoreBarPosX(float x);
	void setScoreBarPosY(float y);

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