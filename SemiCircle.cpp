#include "SemiCircle.h"

SemiCircle::SemiCircle(string name,float inner_radius)
    :Object(name)
{
    // TODO Auto-generated constructor stub
    float theta,nexttheta;
    int SLICES=50;
    int i;
    float PI = 3.14159;

    display_list = glGenLists(1);

    glNewList(display_list,GL_COMPILE);
    glBegin(GL_QUADS);
    glNormal3f(0,1,0);
    for (i=0;i<SLICES;i++)
    {
        theta = (float)i*PI/SLICES;
        nexttheta = (float)((i+1))*PI/SLICES;

        glTexCoord2f(0,theta/(PI));
        glVertex3f(inner_radius * cos(theta),0,inner_radius*sin(theta));

        glTexCoord2f(1,theta/(PI));
        glVertex3f(1 * cos(theta),0,1*sin(theta));

        glTexCoord2f(1,nexttheta/(PI));
        glVertex3f(1 * cos(nexttheta),0,1*sin(nexttheta));

        glTexCoord2f(0,nexttheta/(PI));
        glVertex3f(inner_radius * cos(nexttheta),0,inner_radius*sin(nexttheta));

    }
    glEnd();

    glEndList();
}
