#pragma once
#include "Object.h"
#include "Plane.h"
#include "Matrix4.h"

class Box : public Object
{
public:

    Box(string name="")
        :Object(name)
    {
        float m[16];
        Matrix4 M;
        int i;

        for (i=0;i<6;i++)
                side[i] = new Plane();

        glMatrixMode(GL_MODELVIEW);
        //left side
        glPushMatrix();
        glLoadIdentity();
        glTranslatef(-0.5,0,0);
        glRotatef(90,1,0,0);
        glRotatef(90,0,0,1);
        glGetFloatv(GL_MODELVIEW_MATRIX,m);
        glPopMatrix();
        M.initFromOpenGL(m);
        side[0]->setTransform(M);

        //right side
        glPushMatrix();
        glLoadIdentity();
        glTranslatef(0.5,0,0);
        glRotatef(90,1,0,0);
        glRotatef(-90,0,0,1);
        glGetFloatv(GL_MODELVIEW_MATRIX,m);
        glPopMatrix();
        M.initFromOpenGL(m);
        side[1]->setTransform(M);

        //back side
        glPushMatrix();
        glLoadIdentity();
        glTranslatef(0,0,-0.5);
        glRotatef(-90,1,0,0);
        glGetFloatv(GL_MODELVIEW_MATRIX,m);
        glPopMatrix();
        M.initFromOpenGL(m);
        side[2]->setTransform(M);

        //front side
        glPushMatrix();
        glLoadIdentity();
        glTranslatef(0,0,0.5);
        glRotatef(90,1,0,0);
        glGetFloatv(GL_MODELVIEW_MATRIX,m);
        glPopMatrix();
        M.initFromOpenGL(m);
        side[3]->setTransform(M);

        //bottom side
        glPushMatrix();
        glLoadIdentity();
        glTranslatef(0,-0.5,0);
        glRotatef(180,0,0,1);
        glGetFloatv(GL_MODELVIEW_MATRIX,m);
        glPopMatrix();
        M.initFromOpenGL(m);
        side[4]->setTransform(M);

        //top side
        glPushMatrix();
        glLoadIdentity();
        glTranslatef(0,0.5,0);
        glGetFloatv(GL_MODELVIEW_MATRIX,m);
        glPopMatrix();
        M.initFromOpenGL(m);
        side[5]->setTransform(M);

        display_list = glGenLists(1);
        makeDisplayList();
    }

    ~Box(void)
    {
        int i;

        for (i=0;i<6;i++)
            delete side[i];
    }

    void makeDisplayList()
    {
        glNewList(display_list,GL_COMPILE);
        for (unsigned int i=0;i<6;i++)
                side[i]->draw();
        glEndList();
    }

    void setMaterial(const Material& mat)
    {
        for (unsigned int i=0;i<6;i++)
            side[i]->setMaterial(mat);
        makeDisplayList();
    }


private:
	Plane *side[6];
};
