#include "lab_m1/tema1/scoreBar.h"
#include "lab_m1/tema1/objects.h"

ScoreBar::ScoreBar(float x1, float y1, float x2, float y2, float w, float h)
{
	posX = x1;
	posY = y1;
	borderPosX = x2;
	borderPosY = y2;
	width = w;
	height = h;
	bar = Objects::CreateRectangle("scoreBar", glm::vec3(0.0f, 0.0f, 0.0f), w, h, glm::vec3(0.25f, 0.87f, 0.81f), true);
	border = Objects::CreateRectangle("scoreBarBorder", glm::vec3(0.01f, 0.01f, 0.0f), w - 0.01, h - 0.01, glm::vec3(0.25f, 0.87f, 0.81f));

}

float ScoreBar::getScoreBarBorderPosX()
{
	return borderPosX;
}

void ScoreBar::setScoreBarBorderPosX(float x)
{
	borderPosX = x;
}

float ScoreBar::getScoreBarBorderPosY()
{
	return borderPosY;
}

void ScoreBar::setScoreBarBorderPosY(float y)
{
	borderPosY = y;
}

float ScoreBar::getScoreBarPosX()
{
	return posX;
}

void ScoreBar::setScoreBarPosX(float x)
{
	posX = x;
}

float ScoreBar::getScoreBarPosY()
{
	return posY;
}

void ScoreBar::setScoreBarPosY(float y)
{
	posY = y;
}

float ScoreBar::getWidth()
{
	return width;
}

void ScoreBar::setWidth(float w)
{
	width = w;
}

float ScoreBar::getHeight()
{
	return height;
}

void ScoreBar::setHeight(float h)
{
	height = h;
}


Mesh* ScoreBar::getScoreBarBorder()
{
	return border;
}

Mesh* ScoreBar::getScoreBar()
{
	return bar;
}