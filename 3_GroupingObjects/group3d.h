#ifndef GROUP3D_H
#define GROUP3D_H

#include <transformation.h>
#include <QVector>
#include <QQuaternion>
#include <QVector3D>
#include <QMatrix4x4>

class Group3D: public Transformational
{
public:
    Group3D();
    void draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions);
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

    QVector<Transformational *> m_objects;
};

#endif // GROUP3D_H
