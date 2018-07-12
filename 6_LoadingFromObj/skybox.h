#ifndef SKYBOX_H
#define SKYBOX_H

#include "transformation.h"

class SimpleObject3D;
class QImage;

class SkyBox: public Transformational
{
public:
    SkyBox(float width, const QImage &texture);
    ~SkyBox();
    void draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions);
    void rotate(const QQuaternion &r);
    void translate(const QVector3D &t);
    void scale(const float &s);
    void setGlobalTransform(const QMatrix4x4 &g);
  private:
    SimpleObject3D *m_cube;
};

#endif // SKYBOX_H
