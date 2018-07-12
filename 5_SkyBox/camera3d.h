#ifndef CAMERA3D_H
#define CAMERA3D_H

#include <transformation.h>
#include <QQuaternion>
#include <QVector3D>
#include <QMatrix4x4>

class Camera3D: public Transformational
{
public:
    Camera3D();

    void draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions = 0);
    void rotate(const QQuaternion &r);
    void translate(const QVector3D &t);
    void scale(const float &s);
    void setGlobalTransform(const QMatrix4x4 &g);

    void addObject(Transformational *obj);

private:
    QQuaternion m_rotate;
    QVector3D m_translate;
    float m_scale;

    // Хранит глобальное преобразование - трансформацию всей лок системы координат
    // в пределах глобальной или родительской для нее
    QMatrix4x4 m_globalTransform;

    QMatrix4x4 m_viewMatrix;
};

#endif // CAMERA3D_H
