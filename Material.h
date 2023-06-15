#pragma once

class Material
{
private:
	float ambient[3];
	float diffuse[3];
	float specular[3];
	float shininess;

public:
	Material(void);
	~Material(void);

	void setAmbient(float r,float g,float b);
	void setDiffuse(float r,float g,float b);
	void setSpecular(float r,float g,float b);
        void setShininess(float shininess);

        void init();
	
	float *getAmbient();
	float *getDiffuse();
	float *getSpecular();
	float *getShininess();

};
