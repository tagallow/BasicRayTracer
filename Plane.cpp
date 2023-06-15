#include "plane.h"
#include <QtOpenGL>
#include <math.h>

#define max(A,B) A>B?A:B

Plane::Plane(string name)
    :Object(name)
{
	float *vertices,*texcoords;
	int i,j;
	int DIM;

        DIM = 10;
	vertices = new float[3*(DIM+1)*(DIM+1)];
	texcoords = new float[2*(DIM+1)*(DIM+1)];

	for (i=0;i<DIM+1;i++)
	{
		for (j=0;j<DIM+1;j++)
		{
			vertices[3*(i*(DIM+1)+j)] = -0.5+(1.0f*j/DIM);
			vertices[3*(i*(DIM+1)+j)+1] = 0;
			vertices[3*(i*(DIM+1)+j)+2] = -0.5 + (1.0f*i/DIM);

                        texcoords[2*(i*(DIM+1)+j)] = (float)j/DIM;
                        texcoords[2*(i*(DIM+1)+j)+1] = (float)i/DIM;
		}
	}

	display_list = glGenLists(1);
	glNewList(display_list,GL_COMPILE);
	glBegin(GL_QUADS);
	for (i=0;i<DIM;i++)
	{
		for (j=0;j<DIM;j++)
		{
                        glNormal3f(0,1,0);
                        glTexCoord2fv(texcoords + 2*(i*(DIM+1)+j));
			glVertex3fv(vertices + 3*(i*(DIM+1)+j));

			glNormal3f(0,1,0);
                        glTexCoord2fv(texcoords + 2*(i*(DIM+1)+(j+1)));
			glVertex3fv(vertices + 3*(i*(DIM+1)+j+1));

			glNormal3f(0,1,0);
                        glTexCoord2fv(texcoords + 2*((i+1)*(DIM+1)+(j+1)));
			glVertex3fv(vertices + 3*((i+1)*(DIM+1)+j+1));

			glNormal3f(0,1,0);
                        glTexCoord2fv(texcoords + 2*((i+1)*(DIM+1)+j));
                        glVertex3fv(vertices + 3*((i+1)*(DIM+1)+j));

                    /*    glNormal3f(0,1,0);
                        glTexCoord2f((float)i/DIM,(float)j/DIM);
                        glVertex3f(-0.5+1*(float)i/DIM,0,-0.5+1*(float)j/DIM);

                        glNormal3f(0,1,0);
                        glTexCoord2f((float)(i+1)/DIM,(float)j/DIM);
                        glVertex3f(-0.5+1*(float)(i+1)/DIM,0,-0.5+1*(float)j/DIM);

                        glNormal3f(0,1,0);
                        glTexCoord2f((float)(i+1)/DIM,(float)(j+1)/DIM);
                        glVertex3f(-0.5+1*(float)(i+1)/DIM,0,-0.5+1*(float)(j+1)/DIM);

                        glNormal3f(0,1,0);
                        glTexCoord2f((float)i/DIM,(float)(j+1)/DIM);
                        glVertex3f(-0.5+1*(float)i/DIM,0,-0.5+1*(float)(j+1)/DIM);
*/
		}
	}
	glEnd();
	glEndList();

	delete []vertices;
	delete []texcoords;

}

bool Plane::intersect(Ray r,HitRecord& h)
{
        float temp[3];
        Ray rtemp;
        float time;
        Point3D p;


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

        //intersect with the X-Z plane

        time = -rtemp.s.y/rtemp.v.y;

        p.init(rtemp.s.x+time*rtemp.v.x,rtemp.s.y+time*rtemp.v.y,rtemp.s.z+time*rtemp.v.z);
        if ((p.x>=-0.5) && (p.x<=0.5) && (p.y>=-0.5) && (p.y<=0.5) && (p.z>=-0.5) && (p.z<=0.5))
        {
                h.t = time;
                h.p.init(r.s.x+time*r.v.x,r.s.y+time*r.v.y,r.s.z+time*r.v.z);
                temp[0] = 0;
                temp[1] = 1;
                temp[2] = 0;
                objToWorld.inverse().transpose().multVec(temp);
                h.n.init(temp[0],temp[1],temp[2]);
                h.n.normalize();
                h.material = material;
                h.texture = tex;

                h.tex_s = (0.5+rtemp.s.x + time*rtemp.v.x);
                h.tex_t = (0.5+rtemp.s.z + time*rtemp.v.z);

                return true;
        }
        else
                return false;
}



Plane::~Plane(void)
{
}

