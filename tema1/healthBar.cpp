#include "lab_m1/tema1/healthBar.h"
#include "lab_m1/tema1/objects.h"

HealthBar::HealthBar(float x1, float y1, float x2, float y2, float w, float h)
{
	posX = x1;
	posY = y1;
	borderPosX = x2;
	borderPosY = y2;
	width = w;
	height = h;
	bar = Objects::CreateRectangle("healthBar", glm::vec3(0.0f, 0.0f, 0.0f), w, h, glm::vec3(0.98f, 0.16f, 0.33f), true);
	border = Objects::CreateRectangle("healthBarBorder", glm::vec3(0.01f, 0.01f, 0.0f), w - 0.01, h - 0.01, glm::vec3(0.98f, 0.16f, 0.33f));

}

float HealthBar::getHealthBarBorderPosX()
{
	return borderPosX;
}

void HealthBar::setHealthBarBorderPosX(float x)
{
	borderPosX = x;
}

float HealthBar::getHealthBarBorderPosY()
{
	return borderPosY;
}

void HealthBar::setHealthBarBorderPosY(float y)
{
	borderPosY = y;
}

float HealthBar::getHealthBarPosX()
{
	return posX;
}

void HealthBar::setHealthBarPosX(float x)
{
	posX = x;
}

float HealthBar::getHealthBarPosY()
{
	return posY;
}

void HealthBar::setHealthBarPosY(float y)
{
	posY = y;
}

float HealthBar::getWidth()
{
	return width;
}

void HealthBar::setWidth(float w)
{
	width = w;
}

float HealthBar::getHeight()
{
	return height;
}

void HealthBar::setHeight(float h)
{
	height = h;
}


Mesh* HealthBar::getHealthBarBorder()
{
	return border;
}

Mesh* HealthBar::getHealthBar()
{
	return bar;
}