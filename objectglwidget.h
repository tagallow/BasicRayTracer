#ifndef OBJECTGLWIDGET_H
#define OBJECTGLWIDGET_H

#include <QGLWidget>
#include "Object.h"
#include "Matrix4.h"
#include "Point3D.h"
#include "Texture.h"
#include "Ray.h"
#include <map>
using namespace std;

class ObjectGLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit ObjectGLWidget(QWidget *parent = 0);
    ~ObjectGLWidget();

protected:
    void initializeGL();
    void resizeGL(int w,int h);
    void paintGL();
    void keyPressEvent(QKeyEvent *event);
    void raytrace();
    bool raycast(Ray R,float& r,float& g,float& b,bool full,float refractive_index,int bounce);
    void shade(vector<Light *>& lights,Point3D& P,Vector3D& N,Material& material,Texture *& texture,float tex_s,float tex_t,Point3D& eye,float& r,float& g,float& b);

private:
    int WINDOW_WIDTH,WINDOW_HEIGHT;
    Object *root;
    vector<Texture *> textures;
    map<string,Object *> nodemap;
    Matrix4 worldToView;
    float t;
    int track_stage;
    float start_t;
    bool RAYTRACE;
    float *buffer;
    float FOV;
    float background_r,background_g,background_b;
    Point3D cameraInWorld;
    int MAX_BOUNCES;

    void yes();
    void no();
    void circle();
    void ontrack();


signals:

public slots:
    void openfile();
    void animate();

};

#endif // OBJECTGLWIDGET_H
