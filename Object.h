#pragma once

#include <QtOpenGL>
#include <vector>
#include <string>
using namespace std;
#include "Matrix4.h"
#include "Light.h"
#include "Material.h"
#include "Texture.h"
#include "Point3D.h"
#include "Vector3D.h"
#include "Ray.h"
#include "HitRecord.h"



class Object
{
protected:
	GLuint display_list;
        string name;
        Material material;
        Matrix4 objToWorld,animation,texture;
        vector<Object *> children;
        vector<Light *> lights;
        Texture *tex;

public:
        Object(string name="")
        {
            display_list = 0;
            setName(name);
            tex = NULL;
	}

	virtual ~Object(void)
	{
            if (display_list!=0)
                glDeleteLists(display_list,1);

            for (unsigned int i=0;i<lights.size();i++)
                delete lights[i];

            for (unsigned int i=0;i<children.size();i++)
                delete children[i];
	}

        void addChild(Object *child)
        {
            children.push_back(child);
        }

        void makeNodemap(map<string,Object *>& nodemap)
        {
            if (name != "")
            {
                nodemap[name] = this;
            }
            for (unsigned int i=0;i<children.size();i++)
                children[i]->makeNodemap(nodemap);
        }

        void addLight(Light *l)
        {
            lights.push_back(l);
        }

        vector<Light *>& getLights()
        {
            return lights;
        }

        void setTexture(Texture *tex)
        {
            this->tex = tex;
        }

        void enableLights()
        {
            float mat[16],animat[16];

            objToWorld.getOpenGLMatrix(mat);
            animation.getOpenGLMatrix(animat);
            glMatrixMode(GL_MODELVIEW);
            glPushMatrix();
            glMultMatrixf(animat);
            glMultMatrixf(mat);
            for (unsigned int i=0;i<lights.size();i++)
            {
                glEnable(lights[i]->getID());
                glLightfv(lights[i]->getID(),GL_AMBIENT,lights[i]->getAmbient());
                glLightfv(lights[i]->getID(),GL_DIFFUSE,lights[i]->getDiffuse());
                glLightfv(lights[i]->getID(),GL_SPECULAR,lights[i]->getSpecular());
                glLightfv(lights[i]->getID(),GL_POSITION,lights[i]->getPosition());
                if (lights[i]->getSpotAngle()<180)
                {
                    glLightfv(lights[i]->getID(),GL_SPOT_DIRECTION,lights[i]->getSpotDirection());
                    glLightf(lights[i]->getID(),GL_SPOT_CUTOFF,lights[i]->getSpotAngle());
                }
            }
            for (unsigned int i=0;i<children.size();i++)
                children[i]->enableLights();
            glMatrixMode(GL_MODELVIEW);
            glPopMatrix();
        }

        void disableLights()
        {
            for (unsigned int i=0;i<children.size();i++)
                children[i]->disableLights();
            for (unsigned int i=0;i<lights.size();i++)
            {
                glDisable(lights[i]->getID());
            }
        }

        virtual bool intersect(Ray r,HitRecord& h)
        {
            float temp[3];
            Ray rtemp;
            float time=-1;
            Point3D p;
            HitRecord htemp;


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

            for (unsigned int i=0;i<children.size();i++)
            {
                if (children[i]->intersect(rtemp,htemp))
                {
                    if ((htemp.t>0) && ((htemp.t<time) || (time==-1)))
                    {
                        time = htemp.t;
                        h = htemp;
                    }
                }
            }

            if (time!=-1)
            {
                //transform the point and normal in hitrecord to coordinate system of parent
                float temp[3];

                temp[0] = h.p.x;
                temp[1] = h.p.y;
                temp[2] = h.p.z;
                objToWorld.multPos(temp);
                h.p.init(temp[0],temp[1],temp[2]);

                temp[0] = h.n.x;
                temp[1] = h.n.y;
                temp[2] = h.n.z;
                objToWorld.inverse().transpose().multVec(temp);
                h.n.init(temp[0],temp[1],temp[2]);
                return true;
            }

            return false;
        }

	virtual void draw()
	{
            float mat[16],animat[16],texmat[16];
            objToWorld.getOpenGLMatrix(mat);
            animation.getOpenGLMatrix(animat);
            texture.getOpenGLMatrix(texmat);

            glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,material.getAmbient());
            glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,material.getDiffuse());
            glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,material.getSpecular());
            glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,material.getShininess());

            glMatrixMode(GL_MODELVIEW);
            glPushMatrix();
            glMultMatrixf(animat);
            glMultMatrixf(mat);
            if (tex!=NULL)
            {
                glBindTexture(GL_TEXTURE_2D,tex->getTextureID());
                glMatrixMode(GL_TEXTURE);
                glPushMatrix();
                glMultMatrixf(texmat);
            }
            else
                glBindTexture(GL_TEXTURE_2D,0);
            glCallList(display_list);
            if (tex!=NULL)
            {
                glMatrixMode(GL_TEXTURE);
                glPopMatrix(); //texture matrix does not apply to children
            }
            for (unsigned int i=0;i<children.size();i++)
                children[i]->draw();
            glBindTexture(GL_TEXTURE_2D,0); //unbind it
            glMatrixMode(GL_MODELVIEW);
            glPopMatrix();
	}


        virtual void setMaterial(const Material& mat)
        {
            material = mat;
        }

	void setTransform(Matrix4& obj)
	{
		objToWorld = obj;
	}

        void setAnimationTransform(Matrix4& mat)
        {
            animation = mat;
        }

        void setTextureTransform(Matrix4& mat)
        {
            texture = mat;
        }



        void setName(const string& name)
        {
            this->name = name;
        }

};
