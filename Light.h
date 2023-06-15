#pragma once

class Light
{
private:
	float ambient[3];
	float diffuse[3];
	float specular[3];
	float position[4];
        float spot_direction[3];
	float spot_cutoff;
	unsigned int light_id;
public:
	Light(void);
	~Light(void);

	void setAmbient(float r,float g,float b);
	void setDiffuse(float r,float g,float b);
	void setSpecular(float r,float g,float b);
	void setPosition(float x,float y,float z);
	void setDirection(float x,float y,float z);
	void setID(unsigned int id);
	void init();
	void setSpotDirection(float x,float y,float z);
	void setSpotAngle(float angle);
	float *getAmbient();
	float *getDiffuse();
	float *getSpecular();
	float *getPosition();
	unsigned int getID();
	float *getSpotDirection();
	float getSpotAngle();
};
