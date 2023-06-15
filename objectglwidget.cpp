#include "objectglwidget.h"
#include <QFileDialog>
#include "ObjectXMLReader.h"
#include <queue>

using namespace std;
#include <qDebug>
#include <algorithm>

ObjectGLWidget::ObjectGLWidget(QWidget *parent) :
    QGLWidget(parent)
{
    root = NULL;
    track_stage = 0;
    start_t = 0;
    RAYTRACE=false;
    MAX_BOUNCES = 3;
    buffer = NULL;
}

ObjectGLWidget::~ObjectGLWidget()
{
 /*   for (unsigned int i=0;i<listObjects.size();i++)
        delete listObjects[i];
    listObjects.clear();*/
    if (root!=NULL)
        delete root;
    for (unsigned int i=0;i<textures.size();i++)
        delete textures[i];
}

void ObjectGLWidget::initializeGL()
{
    background_r = background_g = background_b = 0.0f;
    float m[16];
    glClearColor(background_r,background_g,background_b,1);
    glMatrixMode(GL_MODELVIEW);
   // glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    glLoadIdentity();
  //  cameraInWorld.init(100,100,100);
 //   cameraInWorld.init(-50,50,-50);
    cameraInWorld.init(-1.016,1.1,-0.4209);
 //   cameraInWorld.init(-100,50,100);
    // gluLookAt(000,1400,00,0,0,0,0,0,-1);
    gluLookAt(cameraInWorld.x,cameraInWorld.y,cameraInWorld.z,0,0,0,0,1,0);
    //gluLookAt(100,100,100,0,0,0,0,1,0);
    glGetFloatv(GL_MODELVIEW_MATRIX,m);
    glLoadIdentity();
    worldToView.initFromOpenGL(m);
}

void ObjectGLWidget::animate()
{
    t += 0.01;
    ontrack();
    //circle();
    //yes();
    updateGL();
}

void ObjectGLWidget::resizeGL(int w,int h)
{
    WINDOW_WIDTH = w;
    WINDOW_HEIGHT = h;
    FOV = 60;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
   // glOrtho(-100,100,-100.0f*WINDOW_HEIGHT/WINDOW_WIDTH,100.0f*WINDOW_HEIGHT/WINDOW_WIDTH,0.1,10000);
    gluPerspective(FOV,(float)WINDOW_WIDTH/WINDOW_HEIGHT,0.1,10000);
    glViewport(0,0,WINDOW_WIDTH,WINDOW_HEIGHT);
    if (buffer!=NULL)
        delete []buffer;
    buffer = new float[3*WINDOW_WIDTH * WINDOW_HEIGHT];
}

void ObjectGLWidget::paintGL()
{
    float m[16];
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (RAYTRACE)
    {
        if (root!=NULL)
        {
            glMatrixMode(GL_PROJECTION);
            glPushMatrix();
            glLoadIdentity();
            gluOrtho2D(0,WINDOW_WIDTH,0,WINDOW_HEIGHT);
            raytrace();
            glDrawPixels(WINDOW_WIDTH,WINDOW_HEIGHT,GL_RGB,GL_FLOAT,buffer);
            glMatrixMode(GL_PROJECTION);
            glPopMatrix();
        }
    }
    else
    {
        worldToView.getOpenGLMatrix(m);
        glMatrixMode(GL_MODELVIEW);
        glMultMatrixf(m);

        if (root!=NULL)
        {
            glEnable(GL_LIGHTING);
            glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_TRUE);
            glEnable(GL_TEXTURE_2D);
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
            glEnable(GL_NORMALIZE);
            glShadeModel(GL_SMOOTH);
            root->enableLights();
            root->draw();
            root->disableLights();
            glDisable(GL_LIGHTING);
            glDisable(GL_TEXTURE_2D);
        }
    }
}

void ObjectGLWidget::raytrace()
{
    int x,y;
    Ray r;
    float tanFOV = tan(0.5*FOV * 3.14159/180);
    Vector3D dir;
    float p[3];
    float red,green,blue;
    Matrix4 viewToWorld = worldToView.inverse();

    red = 1;
    green = 1;
    blue = 1;

    for (y=0;y<WINDOW_HEIGHT;y++)
    {
        for (x=0;x<WINDOW_WIDTH;x++)
        {
          //  qDebug() << "("<<x<<","<<y<<")\b\b\b\b\b\b\b\b\b\b\b";
            p[0] = x-0.5*WINDOW_WIDTH;
            p[1] = y-0.5*WINDOW_HEIGHT;
            p[2] = -(0.5*WINDOW_HEIGHT/tanFOV);

            viewToWorld.multVec(p);
            dir.init(p[0],p[1],p[2]);
            r.s = cameraInWorld;
            r.v = dir;

            if (!raycast(r,red,green,blue,true,1,0))
            {
                red = background_r;
                green = background_g;
                blue = background_b;
            }

            buffer[3*(y*WINDOW_WIDTH+x)] = red;
            buffer[3*(y*WINDOW_WIDTH+x)+1] = green;
            buffer[3*(y*WINDOW_WIDTH+x)+2] = blue;
        }
    }
}

bool ObjectGLWidget::raycast(Ray R,float& r,float& g,float& b,bool full,float refractive_index,int bounce)
{
        HitRecord hit;
        bool didhit;
        float ra,ga,ba,rr,gr,br,rt,gt,bt;

        r = background_r;
        g = background_g;
        b = background_b;

        didhit = root->intersect(R,hit);

        if (!didhit)
                return false;

        if (!full)
                return didhit;



      //  shade(root->getLights(),refractive_index,hit.p,hit.n,hit.material,hit.texture,hit.tex_s,hit.tex_t,cameraInWorld,ra,ga,ba);
        shade(root->getLights(),hit.p,hit.n,hit.material,hit.texture,hit.tex_s,hit.tex_t,cameraInWorld,ra,ga,ba);


        r = ra;
        g = ga;
        b = ba;

        return true;
}

void ObjectGLWidget::shade(vector<Light *>& lights,Point3D& P,Vector3D& N,Material& material,Texture *& texture,float tex_s,float tex_t,Point3D& eye,float& r,float& g,float& b)
{
        int i,j,k;
        Point3D lightPos;
        Vector3D L;
        Vector3D V,H;
        float DdotDirection;
        float color[3];
        float eta;
        float PI = 3.14159;
        float global_ambient[3];

        global_ambient[0] = global_ambient[1] = global_ambient[2] = 0.2;

        N.normalize();
        V.init(P,eye);
        V.normalize();

        for (i=0;i<3;i++)
                color[i] = 0.0f;
        //global ambient

        for (k=0;k<3;k++)
        {
                color[k] = color[k] + material.getAmbient()[k]*global_ambient[k];
        }

        for (j=0;j<lights.size();j++)
        {
                lightPos.init(lights[j]->getPosition()[0],lights[j]->getPosition()[1],lights[j]->getPosition()[2]);
                if (lights[j]->getPosition()[3]==1)
                {
                        L.init(P,lightPos);
                }
                else
                {
                        L.init(lightPos.x,lightPos.y,lightPos.z);
                }
                L.normalize();

                if (lights[j]->getSpotAngle()<180)
                {
                        Vector3D D(-L.x,-L.y,-L.z);
                        Vector3D LSpotDir(lights[j]->getSpotDirection()[0],lights[j]->getSpotDirection()[1],lights[j]->getSpotDirection()[2]);
                        LSpotDir.normalize();


                        //calculate D.direction of spotlight
                        DdotDirection = D.dotProduct(LSpotDir);

                        DdotDirection = (DdotDirection>0.0f)?DdotDirection:0.0f;

                        if (DdotDirection<cos(PI*lights[j]->getSpotAngle()/180))
                                DdotDirection = 0.0f;
                }
                else
                        DdotDirection = 1.0f;

                //calculate H=L+V
                H = L+V;
                //normalize the H vector
                H.normalize();

                float NdotL = N.dotProduct(L);

                NdotL = (NdotL>0.0f)?NdotL:0.0f;


                //calculate N.H
                float NdotH = N.dotProduct(H);

                NdotH = (NdotH>0.0f)?NdotH:0.0f;

                //finally we set in the color

                //ambient
                float *mat = material.getAmbient();
                float *light = lights[j]->getAmbient();
                for (k=0;k<3;k++)
                {
                        color[k] = color[k] + mat[k]*light[k];
                }
                mat = material.getDiffuse();
                light = lights[j]->getDiffuse();
                //diffuse
                for (k=0;k<3;k++)
                {
                        color[k] = color[k] + DdotDirection*mat[k]*light[k]*NdotL;
                }

                mat = material.getSpecular();
                light = lights[j]->getSpecular();
                //specular
                if (NdotL>0)
                {
                    eta = material.getShininess()[0];
                        for (k=0;k<3;k++)
                        {
                                color[k] = color[k] + DdotDirection*mat[k]*light[k]*pow(NdotH,eta);
                        }
                }
        }
        //clamp
        for (k=0;k<3;k++)
        {
                if (color[k]>1)
                        color[k] = 1;
        }

        r = color[0];
        g = color[1];
        b = color[2];

        if (texture!=NULL)
        {
                float rt,gt,bt;

                texture->lookup(tex_s,tex_t,rt,gt,bt);
               // qDebug() << "Tex coords: "<< r<<","<<g<<","<<b<<endl;
                r *=rt;
                g *=gt;
                b *=bt;
        }

}

void ObjectGLWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_R:
        case Qt::Key_Left:
   // case Qt::Key_r:
        RAYTRACE = !RAYTRACE;
        if (RAYTRACE)
            qDebug() << "Raytracing mode" << endl;
        else
            qDebug() << "OpenGL mode" << endl;
        updateGL();
        break;
    }
}

void ObjectGLWidget::openfile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "./",
                                                    tr("Scene XML files (*.xml *.txt )"));

   // listObjects = ObjectXMLReader::readObjectXMLFile(fileName);

    Object *  temp = ObjectXMLReader::readObjectXMLFile(fileName,textures);
    if (temp!=NULL)
    {
        if (root!=NULL)
            delete root;
        root = temp;
        nodemap.clear();
        root->makeNodemap(nodemap);
    }



 /*   QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(animate()));
    timer->start(5);*/
    updateGL();
}

void ObjectGLWidget::yes()
{
    Matrix4 mat;
    float omat[16];

    mat.setIdentity();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glRotatef(60*sin(5*t),1,0,0);
    glGetFloatv(GL_MODELVIEW_MATRIX,omat);
    glPopMatrix();
    mat.initFromOpenGL(omat);
    Object *facenode = nodemap["face"];
    if (facenode!=NULL)
        facenode->setAnimationTransform(mat);
}

void ObjectGLWidget::no()
{
    Matrix4 mat;
    float omat[16];

    mat.setIdentity();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glRotatef(60*sin(5*t),0,1,0);
    glGetFloatv(GL_MODELVIEW_MATRIX,omat);
    glPopMatrix();
    mat.initFromOpenGL(omat);
    Object *facenode = nodemap["face"];
    if (facenode!=NULL)
        facenode->setAnimationTransform(mat);
}

void ObjectGLWidget::ontrack()
{
    Matrix4 mat;
    float omat[16];
    float speed = 500;

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    switch (track_stage)
    {
    case 0: //right straight arm of the track
        glTranslatef(425,0,500 - speed*(t-start_t));
        if ((speed*(t-start_t)) >=1000)
        {
            track_stage =  (track_stage+1)%4;
            start_t = t;
        }
        break;
    case 1: //upper circle
        glTranslatef(0,0,-500);
        glRotatef(speed*(t-start_t)/425*180/3.14159,0,1,0);
        glTranslatef(425,0,0);
        if ((speed*(t-start_t)/425*180/3.14159)>=180)
        {
            track_stage =  (track_stage+1)%4;
            start_t = t;
        }
        break;
    case 2: //left straight arm of the track
        glTranslatef(-425,0,-500 + speed*(t-start_t));
        glRotatef(180,0,1,0);
        if ((speed*(t-start_t)) >=1000)
        {
            track_stage =  (track_stage+1)%4;
            start_t = t;
        }
        break;
    case 3: //lower circle
        glTranslatef(0,0,500);
        glRotatef(180+speed*(t-start_t)/425*180/3.14159,0,1,0);
        glTranslatef(425,0,0);
        if ((speed*(t-start_t)/425*180/3.14159)>=180)
        {
            track_stage =  (track_stage+1)%4;
            start_t = t;
        }
        break;
    }
//    glRotatef(50*t,0,1,0);
//    glTranslatef(0,0,400);
    glRotatef(90,0,1,0);
    glGetFloatv(GL_MODELVIEW_MATRIX,omat);
    glPopMatrix();
    mat.initFromOpenGL(omat);
    Object *wagonnode = nodemap["wagon"];
    if (wagonnode!=NULL)
        wagonnode->setAnimationTransform(mat);

    glPushMatrix();
    glLoadIdentity();
    glRotatef(1600*t,0,0,1);
    glGetFloatv(GL_MODELVIEW_MATRIX,omat);
    glPopMatrix();
    mat.initFromOpenGL(omat);
    Object *propellor = nodemap["propellor"];
    if (propellor!=NULL)
        propellor->setAnimationTransform(mat);


    glPushMatrix();
    glLoadIdentity();
    glRotatef(-10*speed*t/3,0,0,1);
    glGetFloatv(GL_MODELVIEW_MATRIX,omat);
    glPopMatrix();
    mat.initFromOpenGL(omat);
    Object *wheel = nodemap["front-left-wheel"];
    if (wheel!=NULL)
        wheel->setAnimationTransform(mat);

    wheel = nodemap["front-right-wheel"];
    if (wheel!=NULL)
        wheel->setAnimationTransform(mat);

    glPushMatrix();
    glLoadIdentity();
    //no rotation for rear wheels
    glRotatef(-10*speed*t/3,0,0,1);
    glGetFloatv(GL_MODELVIEW_MATRIX,omat);
    glPopMatrix();
    mat.initFromOpenGL(omat);
    wheel = nodemap["rear-left-wheel"];
    if (wheel!=NULL)
        wheel->setAnimationTransform(mat);

    wheel = nodemap["rear-right-wheel"];
    if (wheel!=NULL)
        wheel->setAnimationTransform(mat);

}

void ObjectGLWidget::circle()
{
    Matrix4 mat;
    float omat[16];

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glRotatef(50*t,0,1,0);
    glTranslatef(0,0,400);
    glGetFloatv(GL_MODELVIEW_MATRIX,omat);
    glPopMatrix();
    mat.initFromOpenGL(omat);
    Object *wagonnode = nodemap["wagon"];
    if (wagonnode!=NULL)
        wagonnode->setAnimationTransform(mat);

    glPushMatrix();
    glLoadIdentity();
    glRotatef(1600*t,0,0,1);
    glGetFloatv(GL_MODELVIEW_MATRIX,omat);
    glPopMatrix();
    mat.initFromOpenGL(omat);
    Object *propellor = nodemap["propellor"];
    if (propellor!=NULL)
        propellor->setAnimationTransform(mat);


    glPushMatrix();
    glLoadIdentity();
    glRotatef(50*0.01,0,1,0); //rotation of the axle to turn
    glRotatef(-2000*t/3,0,0,1);
    glGetFloatv(GL_MODELVIEW_MATRIX,omat);
    glPopMatrix();
    mat.initFromOpenGL(omat);
    Object *wheel = nodemap["front-left-wheel"];
    if (wheel!=NULL)
        wheel->setAnimationTransform(mat);

    wheel = nodemap["front-right-wheel"];
    if (wheel!=NULL)
        wheel->setAnimationTransform(mat);

    glPushMatrix();
    glLoadIdentity();
    //no rotation for rear wheels
    glRotatef(-2000*t/3,0,0,1);
    glGetFloatv(GL_MODELVIEW_MATRIX,omat);
    glPopMatrix();
    mat.initFromOpenGL(omat);
    wheel = nodemap["rear-left-wheel"];
    if (wheel!=NULL)
        wheel->setAnimationTransform(mat);

    wheel = nodemap["rear-right-wheel"];
    if (wheel!=NULL)
        wheel->setAnimationTransform(mat);
}

