#ifndef SIMPLEOBJECT3D_H
#define SIMPLEOBJECT3D_H

#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QVector2D>
#include <QVector>

//#include <QOpenGLFunctions>
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

class SimpleObject3D
{
public:
    SimpleObject3D();
      //  m_indexBuffer(QOpenGLBuffer::IndexBuffer), m_texture(0)
 //   {}
    SimpleObject3D(const QVector<VertexData> &vertData, const QVector<GLuint> &indexes, const QImage &texture);
    ~SimpleObject3D();

    void init(const QVector<VertexData> &vertData, const QVector<GLuint> &indexes, const QImage &texture);
    void draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions);

private:
    QOpenGLBuffer m_vertexBuffer;
    QOpenGLBuffer m_indexBuffer;

    QMatrix4x4 m_modelMatrix;
    QOpenGLTexture *m_texture;
};

#endif // SIMPLEOBJECT3D_H
