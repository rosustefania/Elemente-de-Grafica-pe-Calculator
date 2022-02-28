#include "lab_m1/tema2/homework_camera.h"
#include <lab_m1/tema2/player.h>
#include "components/simple_scene.h"


#include <vector>
#include <string>
#include <iostream>


using namespace std;

Player::Player(float x, float z)
{
    
    posX = x;
    posZ = z;
    rotatingAngle = 0;
    offsetCameraX = 0;
    offsetCameraZ = 0;
   
}

float Player::getPosX()
{
    return posX;
}

float Player::getPosZ()
{
    return posZ;
}

float Player::getOffsetCameraX()
{
    return offsetCameraX;
}

float Player::getOffsetCameraZ()
{
    return offsetCameraZ;
}

float Player::getRotatingAngle()
{
    return rotatingAngle;
    
}

void Player::setPosX(float x)
{
    posX = x;
}

void Player::setPosZ(float z)
{
    posZ = z;
}

void Player::setOffsetCameraX(float x)
{
    offsetCameraX = x;
}

void Player::setOffsetCameraZ(float z)
{
    offsetCameraZ = z;
}


void Player::setRotatingAngle(float angle)
{
    rotatingAngle = angle;
}

Player::~Player()
{
}