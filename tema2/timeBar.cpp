#include "lab_m1/tema2/timeBar.h"
#include "lab_m1/tema2/objects2D.h"

TimeBar::TimeBar(float x1, float y1, float x2, float y2, float w, float h)
{
	posX = x1;
	posY = y1;
	borderPosX = x2;
	borderPosY = y2;
	width = w;
	height = h;
	bar = Objects::CreateRectangle("TimeBar", glm::vec3(0.0f, 0.0f, 0.0f), w, h, glm::vec3(0.25f, 0.87f, 0.81f), true);
	border = Objects::CreateRectangle("TimeBarBorder", glm::vec3(0.01f, 0.01f, 0.0f), w - 0.01, h - 0.01, glm::vec3(0.25f, 0.87f, 0.81f));

}

float TimeBar::getTimeBarBorderPosX()
{
	return borderPosX;
}

void TimeBar::setTimeBarBorderPosX(float x)
{
	borderPosX = x;
}

float TimeBar::getTimeBarBorderPosY()
{
	return borderPosY;
}

void TimeBar::setTimeBarBorderPosY(float y)
{
	borderPosY = y;
}

float TimeBar::getTimeBarPosX()
{
	return posX;
}

void TimeBar::setTimeBarPosX(float x)
{
	posX = x;
}

float TimeBar::getTimeBarPosY()
{
	return posY;
}

void TimeBar::setTimeBarPosY(float y)
{
	posY = y;
}

float TimeBar::getWidth()
{
	return width;
}

void TimeBar::setWidth(float w)
{
	width = w;
}

float TimeBar::getHeight()
{
	return height;
}

void TimeBar::setHeight(float h)
{
	height = h;
}


Mesh* TimeBar::getTimeBarBorder()
{
	return border;
}

Mesh* TimeBar::getTimeBar()
{
	return bar;
}