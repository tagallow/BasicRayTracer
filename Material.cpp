#include ".\material.h"

Material::Material(void)
{
    init();
}

void Material::init()
{
    int i;

    for (i=0;i<3;i++)
    {
         ambient[i] = diffuse[i] = specular[i] = 0.0f;
    }
    shininess = 0.0f;
}

Material::~Material(void)
{

}

void Material::setAmbient(float r,float g,float b)
{
	ambient[0] = r;
	ambient[1] = g;
	ambient[2] = b;
}

void Material::setDiffuse(float r,float g,float b)
{
	diffuse[0] = r;
	diffuse[1] = g;
	diffuse[2] = b;
}

void Material::setSpecular(float r,float g,float b)
{
	specular[0] = r;
	specular[1] = g;
	specular[2] = b;
}

void Material::setShininess(float r)
{
	shininess = r;
}


float *Material::getAmbient()
{
	return ambient;
}

float *Material::getDiffuse()
{
	return diffuse;
}

float *Material::getSpecular()
{
	return specular;
}

float *Material::getShininess()
{
	return &shininess;
}

