#include "group3d.h"
#include <QMatrix4x4>

Group3D::Group3D()
{
    m_scale = 1;
}

void Group3D::draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions)
{
    QMatrix4x4 localMatrix;
    localMatrix.setToIdentity();
    localMatrix.translate(m_translate);
    localMatrix.rotate(m_rotate);
    localMatrix.scale(m_scale);
    localMatrix = m_globalTransform * localMatrix;

    for(int i = 0; i < m_objects.size(); ++i){
        m_objects[i]->setGlobalTransform(localMatrix);
        m_objects[i]->draw(program, functions);
    }
}

void Group3D::rotate(const QQuaternion &r)
{
    m_rotate = r * m_rotate;
}

void Group3D::translate(const QVector3D &t)
{
   m_translate += t;
}

void Group3D::scale(const float &s)
{
    m_scale *= s;
}

void Group3D::setGlobalTransform(const QMatrix4x4 &g)
{
    m_globalTransform = g;
}

void Group3D::addObject(Transformational *obj)
{
    m_objects.append(obj);
}
