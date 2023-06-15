#ifndef OBJECTXMLREADER_H
#define OBJECTXMLREADER_H

#include <QString>
#include <QtGui>
#include <qxml.h>
#include <QtOpenGL>
#include <qDebug>
#include <stack>
using namespace std;
#include "Object.h"
#include "Plane.h"
#include "Cone.h"
#include "Box.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "SemiCircle.h"
#include "Light.h"
#include "Material.h"
#include "Texture.h"





class ObjectXMLReader
{
public:
	ObjectXMLReader(){}
	~ObjectXMLReader(){}
        static Object * readObjectXMLFile(QString filename,vector<Texture *>& textures) //should instead return the list of objects
	{
            ObjectXMLFileHandler handler;
            QXmlSimpleReader reader;
            QFile xmlFile(filename);
            QXmlInputSource source(&xmlFile);


	    reader.setContentHandler(&handler);

	    bool answer = reader.parse(source);

            Object *root;

	    if (answer)
	    {
                root = handler.getRoot();
                textures = handler.getTextures();
	    }
	    else
	    	printf("Parsing unsuccessful because %s\n",handler.errorString().toAscii().constData());

            return root;

	}

	class ObjectXMLFileHandler: public QXmlDefaultHandler
	{
            public:
                bool startDocument()
                {
                    obj = NULL;
                    root = NULL;
                    stackObjects.push(NULL);
                    texture = NULL;
                    light = NULL;
                    inTexture = false;
                    light_id = GL_LIGHT0;
                    return true;
                }

                bool characters(const QString& text)
                {
                    int c;
                    float f;
                        QString info;

                    info = text;
                    info.remove(QRegExp("[\n\t\r]*")); //remove tabs and newlines

                            QStringList list = info.split(' ',QString::SkipEmptyParts);


                            for (int i=0;i<list.length();i++,index++)
                            {
                                    c=sscanf(list.at(i).toAscii().constData(),"%f",&f);
                                    printf("Read number: %f\n",f);
                                    data.push_back(f);
                                    if (c!=1)
                                            return FALSE;

                            }

                            //all the data numbers are in the data array

                    return TRUE;
                }

                bool startElement( const QString & namespaceURI,const QString & localName,const QString & qName,const QXmlAttributes & atts)
                {
                    printf("Start:%s\n",qName.toAscii().constData());

                    if (qName.compare("object")==0)
                    {
                        string name;

                        int i=0;
                        bool found = false;

                        while ((i<atts.count()) && (!found))
                        {
                            if (atts.qName(i).compare("name")==0)
                            {
                                name = atts.value(i).toAscii().constData();
                                found = true;
                            }
                            i++;
                        }

                        stackObjects.push(new Object(name));
                    }
                    else if (qName.compare("plane")==0)
                    {
                        string name;

                        int i=0;
                        bool found = false;

                        while ((i<atts.count()) && (!found))
                        {
                            if (atts.qName(i).compare("name")==0)
                            {
                                name = atts.value(i).toAscii().constData();
                                found = true;
                            }
                            i++;
                        }
                        stackObjects.push(new Plane(name));
                    }
                    else if (qName.compare("sphere")==0)
                    {
                        string name;

                        int i=0;
                        bool found = false;

                        while ((i<atts.count()) && (!found))
                        {
                            if (atts.qName(i).compare("name")==0)
                            {
                                name = atts.value(i).toAscii().constData();
                                found = true;
                            }
                            i++;
                        }stackObjects.push(new Sphere(name));
                    }
                    else if (qName.compare("box")==0)
                    {
                        string name;

                        int i=0;
                        bool found = false;

                        while ((i<atts.count()) && (!found))
                        {
                            if (atts.qName(i).compare("name")==0)
                            {
                                name = atts.value(i).toAscii().constData();
                                found = true;
                            }
                            i++;
                        }
                         stackObjects.push(new Box(name));
                    }
                    else if (qName.compare("cylinder")==0)
                    {

                        QString inner;
                        float inner_radius;
                        int i=0;
                        bool found = false;

                        while ((i<atts.count()) && (!found))
                        {
                            if (atts.qName(i).compare("inner")==0)
                            {
                                    inner = atts.value(i);
                                    found = true;
                            }
                            i++;
                        }
                        if (found)
                        {
                            inner_radius = inner.toFloat();
                        }
                        else
                            inner_radius = 0.0f;

                        string name;

                        i=0;
                        found = false;

                        while ((i<atts.count()) && (!found))
                        {
                            if (atts.qName(i).compare("name")==0)
                            {
                                name = atts.value(i).toAscii().constData();
                                found = true;
                            }
                            i++;
                        }

                         stackObjects.push(new Cylinder(name,inner_radius));
                    }
                    else if (qName.compare("cone")==0)
                    {
                        string name;

                        int i=0;
                        bool found = false;

                        while ((i<atts.count()) && (!found))
                        {
                            if (atts.qName(i).compare("name")==0)
                            {
                                name = atts.value(i).toAscii().constData();
                                found = true;
                            }
                            i++;
                        }
                         stackObjects.push(new Cone(name));
                    }
                    else if (qName.compare("semicircle")==0)
                    {

                        QString inner;
                        float inner_radius;
                        int i=0;
                        bool found = false;

                        while ((i<atts.count()) && (!found))
                        {
                            if (atts.qName(i).compare("inner")==0)
                            {
                                    inner = atts.value(i);
                                    found = true;
                            }
                            i++;
                        }
                        if (found)
                        {
                            inner_radius = inner.toFloat();
                        }
                        else
                            inner_radius = 0.0f;

                        string name;

                        i=0;
                        found = false;

                        while ((i<atts.count()) && (!found))
                        {
                            if (atts.qName(i).compare("name")==0)
                            {
                                name = atts.value(i).toAscii().constData();
                                found = true;
                            }
                            i++;
                        }

                         stackObjects.push(new SemiCircle(name,inner_radius));
                    }

                    else if (qName.compare("light")==0)
                    {
                        light = new Light();
                        light->setID(light_id);
                        light_id++;
                    }
                    else if (qName.compare("mesh")==0)
                    {
                    }
                    else if (qName.compare("image")==0)
                    {
                        QString path,name;
                        unsigned int i=0;
                        bool found = false;

                        while ((i<atts.count()) && (!found))
                        {
                            if (atts.qName(i).compare("path")==0)
                            {
                                path = atts.value(i);
                                found = true;
                            }
                            i++;
                        }
                        if (!found)
                                return false;

                        i = 0;
                        found = false;
                        while ((i<atts.count()) && (!found))
                        {
                            if (atts.qName(i).compare("name")==0)
                            {
                                name = atts.value(i);
                                found = true;
                            }
                            i++;
                        }

                        if (!found)
                            return false;

                        texture = new Texture();
                        if (!texture->createImage(path.toAscii().data()))
                                return false;
                        texture->setName(name);
                        printf("Read in an image %s for texture purposes\n",path.toAscii().constData());
                    }
                    else if (qName.compare("texture")==0)
                    {
                        QString name;
                        unsigned int i;
                        bool found = false;

                        i = 0;
                        while ((i<atts.count()) && (!found))
                        {
                            if (atts.qName(i).compare("name")==0)
                            {
                                name = atts.value(i);
                                found = true;
                            }
                            i++;
                        }
                        if (!found)
                                return false;

                        i = 0;

                        while ((i<textures.size()) && (name.compare(textures[i]->getName())!=0))
                            i++;

                        if (i<textures.size())
                        {
                            obj = stackObjects.top();
                            obj->setTexture(textures[i]);
                            inTexture = true;
                        }
                        else
                            return false;
                    }

                    return true;
                }


                bool endElement( const QString&, const QString&, const QString& qName)
                {
                    if (qName.compare("object")==0)
                    {
                        obj = stackObjects.top();
                        stackObjects.pop();
                        if (stackObjects.top()!=NULL)
                        {
                            Object *temp = stackObjects.top();
                            temp->addChild(obj);
                        }
                        else
                        {
                            root = obj;
                        }
                        obj = NULL;
                    }
                    else if (qName.compare("plane")==0)
                    {
                        obj = stackObjects.top();
                        stackObjects.pop();
                        if (stackObjects.top()!=NULL)
                        {
                            Object *temp = stackObjects.top();
                            temp->addChild(obj);
                        }
                        obj = NULL;
                    }
                    else if (qName.compare("sphere")==0)
                    {
                        obj = stackObjects.top();
                        stackObjects.pop();
                        if (stackObjects.top()!=NULL)
                        {
                            Object *temp = stackObjects.top();
                            temp->addChild(obj);
                        }
                        obj = NULL;
                    }
                    else if (qName.compare("cylinder")==0)
                    {
                        obj = stackObjects.top();
                        stackObjects.pop();
                        if (stackObjects.top()!=NULL)
                        {
                            Object *temp = stackObjects.top();
                            temp->addChild(obj);
                        }
                        obj = NULL;
                    }
                    else if (qName.compare("cone")==0)
                    {
                        obj = stackObjects.top();
                        stackObjects.pop();
                        if (stackObjects.top()!=NULL)
                        {
                            Object *temp = stackObjects.top();
                            temp->addChild(obj);
                        }
                        obj = NULL;
                    }
                    else if (qName.compare("box")==0)
                    {
                        obj = stackObjects.top();
                        stackObjects.pop();
                        if (stackObjects.top()!=NULL)
                        {
                            Object *temp = stackObjects.top();
                            temp->addChild(obj);
                        }
                        obj = NULL;
                    }
                    else if (qName.compare("semicircle")==0)
                    {
                        obj = stackObjects.top();
                        stackObjects.pop();
                        if (stackObjects.top()!=NULL)
                        {
                            Object *temp = stackObjects.top();
                            temp->addChild(obj);
                        }
                        obj = NULL;
                    }
                    else if (qName.compare("mesh")==0)
                    {
                    }
                    else if (qName.compare("image")==0)
                    {
                        textures.push_back(texture);
                        texture = NULL;
                    }
                    else if (qName.compare("texture")==0)
                    {
                        inTexture = false;
                    }
                    else if (qName.compare("transform")==0)
                    {
                        obj = stackObjects.top();
                        if (obj!=NULL)
                        {
                            if (inTexture)
                                obj->setTextureTransform(M);
                            else
                                obj->setTransform(M);
                        }
                        M.setIdentity();
                    }
                    else if (qName.compare("translate")==0)
                    {
                        if (data.size()!=3)
                            return false;

                        glMatrixMode(GL_MODELVIEW);
                        glPushMatrix();
                        glLoadIdentity();
                        glTranslatef(data[0],data[1],data[2]);
                        float m[16];
                        glGetFloatv(GL_MODELVIEW_MATRIX,m);
                        glPopMatrix();
                        Matrix4 temp;

                        temp.initFromOpenGL(m);
                        M = M.preMul(temp);
                        data.clear();
                    }
                    else if (qName.compare("rotate")==0)
                    {
                        if (data.size()!=4)
                        return false;

                        glMatrixMode(GL_MODELVIEW);
                        glPushMatrix();
                        glLoadIdentity();
                        glRotatef(data[0],data[1],data[2],data[3]);
                        float m[16];
                        glGetFloatv(GL_MODELVIEW_MATRIX,m);
                        glPopMatrix();
                        Matrix4 temp;

                        temp.initFromOpenGL(m);
                        M = M.preMul(temp);
                        data.clear();
                    }
                    else if (qName.compare("scale")==0)
                    {
                        if (data.size()!=3)
                            return false;

                        glMatrixMode(GL_MODELVIEW);
                        glPushMatrix();
                        glLoadIdentity();
                        glScalef(data[0],data[1],data[2]);
                        float m[16];
                        glGetFloatv(GL_MODELVIEW_MATRIX,m);
                        glPopMatrix();
                        Matrix4 temp;

                        temp.initFromOpenGL(m);
                        M = M.preMul(temp);
                        data.clear();
                    }
                   /* else if (qName.compare("color")==0)
                    {
                        if (data.size()!=3)
                            return false;

                        obj = stackObjects.top();
                        if (obj!=NULL)
                            obj->setColor(data[0],data[1],data[2]); //set the object's color
                        data.clear();
                    }*/
                    else if (qName.compare("ambient")==0)
                    {
                        if (data.size()!=3)
                            return false;
                        if (light!=NULL)
                            light->setAmbient(data[0],data[1],data[2]);
                        else
                            mat.setAmbient(data[0],data[1],data[2]);
                        data.clear();
                    }
                    else if (qName.compare("diffuse")==0)
                    {
                        if (data.size()!=3)
                            return false;
                        if (light!=NULL)
                            light->setDiffuse(data[0],data[1],data[2]);
                        else
                            mat.setDiffuse(data[0],data[1],data[2]);
                        data.clear();
                    }
                    else if (qName.compare("specular")==0)
                    {
                        if (data.size()!=3)
                            return false;
                        if (light!=NULL)
                            light->setSpecular(data[0],data[1],data[2]);
                        else
                            mat.setSpecular(data[0],data[1],data[2]);
                        data.clear();
                    }
                    else if (qName.compare("shininess")==0)
                    {
                        if (data.size()!=1)
                            return false;
                        mat.setShininess(data[0]);
                        data.clear();
                    }
                    else if (qName.compare("position")==0)
                    {
                        if (data.size()!=3)
                            return false;
                        if (light!=NULL)
                            light->setPosition(data[0],data[1],data[2]);
                        data.clear();
                    }
                    else if (qName.compare("direction")==0)
                    {
                        if (data.size()!=3)
                            return false;
                        if (light!=NULL)
                            light->setDirection(data[0],data[1],data[2]);
                        data.clear();
                    }
                    else if (qName.compare("spotdirection")==0)
                    {
                        if (data.size()!=3)
                            return false;
                        if (light!=NULL)
                            light->setSpotDirection(data[0],data[1],data[2]);
                        data.clear();
                    }
                    else if (qName.compare("spotangle")==0)
                    {
                        if (data.size()!=1)
                            return false;
                        if (light!=NULL)
                            light->setSpotAngle(data[0]);
                        data.clear();
                    }
                    else if (qName.compare("light")==0)
                    {
                        obj = stackObjects.top();
                        obj->addLight(light);
                        light = NULL;
                    }
                    else if (qName.compare("material")==0)
                    {
                        obj = stackObjects.top();
                        obj->setMaterial(mat);
                        mat.init();
                    }
                    else if (qName.compare("absorption")==0)
                    {
                        data.clear();
                    }
                    else if (qName.compare("reflection")==0)
                    {
                        data.clear();
                    }
                    else if (qName.compare("transparency")==0)
                    {
                        data.clear();
                    }
                    else if (qName.compare("refractive")==0)
                    {
                        data.clear();
                    }
                    else if (qName.compare("camera")==0)
                    {
                    }
                    else if (qName.compare("eye")==0)
                    {
                    }
                    else if (qName.compare("look")==0)
                    {
                    }
                    //printf("End:%s \n",qName.toAscii().constData());
                    qDebug() << "End:" << qName.toAscii().constData() <<endl;

                    return true;
                }

                Object *getRoot()
                {
                    return root;
                }

                vector<Texture *> getTextures()
                {
                    return textures;
                }

            private:

                vector<float> data;
                int index;
                Object *obj,*root;
                Material mat;
                Texture *texture;
                bool inTexture;
                vector<Texture *> textures;
                Light *light;
                Matrix4 M;
                unsigned int light_id;
                stack<Object*> stackObjects;
            };



};



#endif /* OBJECTXMLREADER_H_ */
