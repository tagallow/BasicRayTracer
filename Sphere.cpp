#include "Sphere.h"
#include <math.h>


Sphere::Sphere(string name)
    :Object(name)
{
	float theta,phi;
	float *vertices,*normals,*texcoords;
	int STACKS=50;
	int i,j;
	int SLICES = 50;
	float PI = 3.14159;
	float cosphi,sinphi,costheta,sintheta;

	display_list = glGenLists(1);

	vertices = new float[3*(STACKS+1)*(SLICES+1)];
	normals = new float[3*(STACKS+1)*(SLICES+1)];
	texcoords = new float[2*(STACKS+1)*(SLICES+1)];

	for (i=0;i<=STACKS;i++)
	{
		phi = -PI/2 + i*PI/STACKS;
		cosphi = cos(phi);
		sinphi = sin(phi);
		for (j=0;j<=SLICES;j++)
		{
			theta = 2*j*PI/SLICES;
			costheta = cos(theta);
			sintheta = sin(theta);

			vertices[3*(i*(SLICES+1)+j)] = cosphi*costheta;
			vertices[3*(i*(SLICES+1)+j)+1] = sinphi;
			vertices[3*(i*(SLICES+1)+j)+2] = -cosphi*sintheta;

			normals[3*(i*(SLICES+1)+j)] = cosphi*costheta;
			normals[3*(i*(SLICES+1)+j)+1] = sinphi;
			normals[3*(i*(SLICES+1)+j)+2] = -cosphi*sintheta;

			texcoords[2*(i*(SLICES+1)+j)] = theta/(2*PI);
			texcoords[2*(i*(SLICES+1)+j)+1] = (phi+0.5*PI)/PI;

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
	glEndList();

	printf("A sphere has been created\n");

	delete []vertices;
	delete []normals;
	delete []texcoords;
}

bool Sphere::intersect(Ray r,HitRecord& h)
{
        float temp[3];
        Ray rtemp;
        float time;
        Point3D p;
        float PI=3.14159;


        temp[0] = r.s.x;
        temp[1] = r.s.y;
        temp[2] = r.s.z;

        objToWorld.inverse().multPos(temp);
        rtemp.s.init(temp[0],temp[1],temp[2]);

        temp[0] = r.v.x;
        temp[1] = r.v.y;
        temp[2] = r.v.z;

        objToWorld.inverse().multVec(temp);
        rtemp.v.init(temp[0],temp[1],temp[2]);

        //intersect with a sphere of unit radius centered at the origin

        float A,B,C,D;

        A = rtemp.v.x*rtemp.v.x + rtemp.v.y*rtemp.v.y + rtemp.v.z*rtemp.v.z;
        B = 2*(rtemp.s.x*rtemp.v.x + rtemp.s.y*rtemp.v.y + rtemp.s.z*rtemp.v.z);
        C = rtemp.s.x*rtemp.s.x + rtemp.s.y*rtemp.s.y + rtemp.s.z*rtemp.s.z - 1;
        D = B*B-4*A*C;

        if (D>=0)
        {
                float t1,t2;
                t1 = (-B+sqrtf(D))/(2*A);
                t2 = (-B-sqrtf(D))/(2*A);

                if (t1<0)
                {
                        if (t2<0)
                        {
                                time = (-t1<-t2)?t1:t2;
                        }
                        else
                        {
                                time = t2;
                        }
                }
                else
                {
                        if (t2<0)
                                time = t1;
                        else
                                time = (t1<t2)?t1:t2;
                }
                h.t = time;
                h.p.init(r.s.x+time*r.v.x,r.s.y+time*r.v.y,r.s.z+time*r.v.z);
                temp[0] = rtemp.s.x + time*rtemp.v.x;
                temp[1] = rtemp.s.y + time*rtemp.v.y;
                temp[2] = rtemp.s.z + time*rtemp.v.z;

                objToWorld.inverse().transpose().multVec(temp);
                h.n.init(temp[0],temp[1],temp[2]);

                //texture coordinate
                float theta,phi;

                float y = rtemp.s.y + time*rtemp.v.y;
                y=((y>1)?1:((y<-1)?-1:y));
                phi = asin(fabs(y));
                if (y<0)
                        phi = -phi;

                float x = rtemp.s.x + time*rtemp.v.x;
                float z = rtemp.s.z + time*rtemp.v.z;
                x=((x>1)?1:((x<-1)?-1:x));
                theta = acos(fabs(x)/cos(phi));

                if (z<0)
                {
                        if (x<0)
                                theta = PI - theta;
                }
                else
                {
                        if (x<0)
                                theta = PI + theta;
                        else
                                theta = 2*PI - theta;
                }

                h.tex_s = theta/(2*PI);
                h.tex_t = (phi + 0.5*PI)/PI;

                h.n.normalize();
                h.material = material;
                 h.texture = tex;

                return true;
        }
        else
                return false;
}

Sphere::~Sphere(void)
{
}

