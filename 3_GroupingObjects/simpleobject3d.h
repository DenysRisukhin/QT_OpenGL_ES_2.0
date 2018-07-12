#ifndef SIMPLEOBJECT3D_H
#define SIMPLEOBJECT3D_H

#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QVector2D>
#include <QVector>
#include <transformation.h>
//#include <QOpenGLShaderProgram>

class QOpenGLTexture;
class QOpenGLFunctions;
class QOpenGLShaderProgram;

struct VertexData{
    VertexData(QVector3D p, QVector2D t, QVector3D n) :
        position(p), texCoord(t), normal(n) {}

    VertexData() {}

    QVector3D position;
    QVector2D texCoord;
    QVector3D normal; // нормаль данной точки
};

class SimpleObject3D: public Transformational
{
public:
    SimpleObject3D();
    SimpleObject3D(const QVector<VertexData> &vertData, const QVector<GLuint> &indexes, const QImage &texture);
    ~SimpleObject3D();

    void init(const QVector<VertexData> &vertData, const QVector<GLuint> &indexes, const QImage &texture);
    void draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions);
    void rotate(const QQuaternion &r);
    void translate(const QVector3D &t);
    void scale(const float &s);
    void setGlobalTransform(const QMatrix4x4 &g);

private:
    QOpenGLBuffer m_vertexBuffer;
    QOpenGLBuffer m_indexBuffer;

    QOpenGLTexture *m_texture;

    QQuaternion m_rotate;
    QVector3D m_translate;
    float m_scale;

    // Хранит глобальное преобразование - трансформацию всей лок системы координат
    // в пределах глобальной или родительской для нее
    QMatrix4x4 m_globalTransform;
};

#endif // SIMPLEOBJECT3D_H
