/*
 * Cone.cpp
 *
 *  Created on: Sep 13, 2009
 *      Author: ashesh
 */

#include "Cone.h"

Cone::Cone(string name)
    :Object(name)
{
	// TODO Auto-generated constructor stub
	float theta;
	float *vertices,*normals,*texcoords;
	int STACKS=50;
	int i,j;
	int SLICES = 50;
	float PI = 3.14159,y;
	float costheta,sintheta;
	float radius;

	display_list = glGenLists(1);

	vertices = new float[3*(STACKS+1)*(SLICES+1)];
	normals = new float[3*(STACKS+1)*(SLICES+1)];
	texcoords = new float[2*(STACKS+1)*(SLICES+1)];

	for (i=0;i<=STACKS;i++)
	{
		y = (float)i/STACKS;
		radius = 1.0 - y;
		for (j=0;j<=SLICES;j++)
		{
			theta = 2*j*PI/SLICES;
			costheta = cos(theta);
			sintheta = sin(theta);

			vertices[3*(i*(SLICES+1)+j)] = radius*costheta;
			vertices[3*(i*(SLICES+1)+j)+1] = y;
			vertices[3*(i*(SLICES+1)+j)+2] = radius*sintheta;

			normals[3*(i*(SLICES+1)+j)] = costheta;
			normals[3*(i*(SLICES+1)+j)+1] = 0;
			normals[3*(i*(SLICES+1)+j)+2] = sintheta;

			texcoords[2*(i*(SLICES+1)+j)] = theta/(2*PI);
			texcoords[2*(i*(SLICES+1)+j)+1] = y;

		}
	}


	glNewList(display_list,GL_COMPILE);
	glBegin(GL_QUADS);
	for (i=0;i<STACKS;i++)
	{
		for (j=0;j<SLICES;j++)
		{
			glNormal3fv(normals + 3*(i*(SLICES+1)+j));
			glTexCoord2fv(texcoords + 2*(i*(SLICES+1)+j));
			glVertex3fv(vertices + 3*(i*(SLICES+1)+j));

			glNormal3fv(normals + 3*(i*(SLICES+1)+(j+1)));
			glTexCoord2fv(texcoords + 2*(i*(SLICES+1)+(j+1)));
			glVertex3fv(vertices + 3*(i*(SLICES+1)+(j+1)));

			glNormal3fv(normals + 3*(((i+1))*(SLICES+1)+(j+1)));
			glTexCoord2fv(texcoords + 2*((i+1)*(SLICES+1)+(j+1)));
			glVertex3fv(vertices + 3*(((i+1))*(SLICES+1)+(j+1)));

			glNormal3fv(normals + 3*(((i+1))*(SLICES+1)+j));
			glTexCoord2fv(texcoords + 2*((i+1)*(SLICES+1)+j));
			glVertex3fv(vertices + 3*((i+1)*(SLICES+1)+j));
		}
	}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0,0,0);
	for (i=0;i<=SLICES;i++)
	{
		theta = i*2*PI/SLICES;
		costheta = cos(theta);
		sintheta = sin(theta);
		glVertex3f(costheta,0,-sintheta);
	}
	glEnd();

	glEndList();

	delete []vertices;
	delete []normals;
	delete []texcoords;


}

Cone::~Cone() {
	// TODO Auto-generated destructor stub
}
