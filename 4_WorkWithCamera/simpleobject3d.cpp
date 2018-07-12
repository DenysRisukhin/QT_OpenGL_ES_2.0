#include "simpleobject3d.h"
#include <QOpenGLTexture>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

SimpleObject3D::SimpleObject3D():
    m_indexBuffer(QOpenGLBuffer::IndexBuffer), m_texture(0)
{
    m_scale = 1.0;
}

SimpleObject3D::SimpleObject3D(const QVector<VertexData> &vertData, const QVector<GLuint> &indexes, const QImage &texture):
     m_indexBuffer(QOpenGLBuffer::IndexBuffer), m_texture(0)
{
    m_scale = 1.0;
    init(vertData, indexes, texture);
}

SimpleObject3D::~SimpleObject3D()
 {
    if (m_vertexBuffer.isCreated())
        m_vertexBuffer.destroy();
    if (m_indexBuffer.isCreated())
        m_indexBuffer.destroy();
    if (m_texture != 0){
        if (m_texture->isCreated())
            m_texture->destroy();
    }
}

void SimpleObject3D::init(const QVector<VertexData> &vertData, const QVector<GLuint> &indexes, const QImage &texture)
{
    if (m_vertexBuffer.isCreated())
        m_vertexBuffer.destroy();
    if (m_indexBuffer.isCreated())
        m_indexBuffer.destroy();
    if (m_texture != 0){
        if (m_texture->isCreated()){
            delete m_texture;
            m_texture = 0;
        }
    }

    m_vertexBuffer.create();
    m_vertexBuffer.bind();
    m_vertexBuffer.allocate(vertData.constData(), vertData.size() * sizeof(VertexData));
    m_vertexBuffer.release();

    m_indexBuffer.create();
    m_indexBuffer.bind();
    m_indexBuffer.allocate(indexes.constData(), indexes.size() * sizeof(GLuint));
    m_indexBuffer.release();

    m_texture = new QOpenGLTexture(texture.mirrored());

    m_texture->setMinificationFilter(QOpenGLTexture::Nearest);

    m_texture->setMagnificationFilter(QOpenGLTexture::Linear);

    m_texture->setWrapMode(QOpenGLTexture::Repeat);
}

void SimpleObject3D::draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions)
{
    if (!m_vertexBuffer.isCreated() || !m_indexBuffer.isCreated()) return;

    m_texture->bind(0);
    program->setUniformValue("u_texture", 0);

    // опреции с матрицей идут в обратном порядке начиная со scale
    QMatrix4x4 modelMatrix;
    modelMatrix.setToIdentity();
    modelMatrix.translate(m_translate);
    modelMatrix.rotate(m_rotate);
    modelMatrix.scale(m_scale);
    modelMatrix = m_globalTransform * modelMatrix;

    program->setUniformValue("u_modelMatrix", modelMatrix);

    m_vertexBuffer.bind();
    int offset = 0;

    // получаем располжение вершинного атрибута: qt_Vertex и qt_MultiTexCoord0 координаты
    int vertLoc = program->attributeLocation("a_position");

    // вкл использование этого атрибута
    program->enableAttributeArray(vertLoc);
    program->setAttributeBuffer(vertLoc, GL_FLOAT, offset, 3, sizeof(VertexData));

    offset += sizeof(QVector3D);

    int texLoc = program->attributeLocation("a_texcoord");
    program->enableAttributeArray(texLoc);
    program->setAttributeBuffer(texLoc, GL_FLOAT, offset, 2, sizeof(VertexData));

    offset += sizeof(QVector2D);

    int normLoc = program->attributeLocation("a_normal");
    program->enableAttributeArray(normLoc);
    program->setAttributeBuffer(normLoc, GL_FLOAT, offset, 3, sizeof(VertexData));


    // биндим индексный буфер
    m_indexBuffer.bind();

    // вызываем отрисовку
    // GL_TRIANGLES - примитивы которыми будем рисовать
    // m_indexBuffer.size() кол-во эл-ов для отрисовки
    functions->glDrawElements(GL_TRIANGLES, m_indexBuffer.size(), GL_UNSIGNED_INT, 0);

    m_vertexBuffer.release();
    m_indexBuffer.release();
    m_texture->release();
}

void SimpleObject3D::rotate(const QQuaternion &r)
{
    m_rotate = r * m_rotate;
}

void SimpleObject3D::translate(const QVector3D &t)
{
   m_translate += t;
}

void SimpleObject3D::scale(const float &s)
{
    m_scale *= s;
}

void SimpleObject3D::setGlobalTransform(const QMatrix4x4 &g)
{
    m_globalTransform = g;
}
