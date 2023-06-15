#include "Light.h"

Light::Light(void)
{
	init();
}

Light::~Light(void)
{
}

void Light::init()
{
	int i;

	for (i=0;i<3;i++)
	{
		ambient[i] = diffuse[i] = specular[i] = position[i] = spot_direction[i] = 0.0f;
	}
        position[3] = 0.0f;
        spot_cutoff = 180; //by default not a spotlight
}

void Light::setID(unsigned int id)
{
	this->light_id = id;
}

unsigned int Light::getID()
{
	return light_id;
}

void Light::setAmbient(float r,float g,float b)
{
	ambient[0] = r;
	ambient[1] = g;
	ambient[2] = b;
}

void Light::setDiffuse(float r,float g,float b)
{
	diffuse[0] = r;
	diffuse[1] = g;
	diffuse[2] = b;
}

void Light::setSpecular(float r,float g,float b)
{
	specular[0] = r;
	specular[1] = g;
	specular[2] = b;
}

void Light::setPosition(float x,float y,float z)
{
	position[0] = x;
	position[1] = y;
	position[2] = z;
        position[3] = 1.0f;
}

void Light::setDirection(float x,float y,float z)
{
	position[0] = x;
	position[1] = y;
	position[2] = z;
        position[3] = 0.0f;
}

void Light::setSpotDirection(float x,float y,float z)
{
	spot_direction[0] = x;
	spot_direction[1] = y;
	spot_direction[2] = z;
}

void Light::setSpotAngle(float angle)
{
	spot_cutoff = angle;
}


float *Light::getAmbient()
{
	return ambient;
}

float *Light::getDiffuse()
{
	return diffuse;
}

float *Light::getSpecular()
{
	return specular;
}

float *Light::getPosition()
{
	return position;
}

float *Light::getSpotDirection()
{
	return spot_direction;
}

float Light::getSpotAngle()
{
	return spot_cutoff;
}

