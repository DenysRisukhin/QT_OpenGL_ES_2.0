#include "skybox.h"
#include "simpleobject3d.h"

SkyBox::SkyBox(float width, const QImage &texture)
{
    float width_div_2 = width / 2.0f;

    QVector<VertexData> vertexes;

    // Позади нас
    vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, width_div_2), QVector2D(1.0, 2.0/3.0), QVector3D(0.0, 0.0, -1.0)));
    vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, width_div_2), QVector2D(1.0, 1.0/3.0), QVector3D(0.0, 0.0, -1.0)));
    vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, width_div_2), QVector2D(3.0/4.0, 2.0/3.0), QVector3D(0.0, 0.0, -1.0)));
    vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, width_div_2), QVector2D(3.0/4.0, 1.0/3.0), QVector3D(0.0, 0.0, -1.0)));

    // Справа от нас
    vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, width_div_2), QVector2D(3.0/4.0, 2.0/3.0), QVector3D(-1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, width_div_2), QVector2D(3.0/4.0, 1.0/3.0), QVector3D(-1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, -width_div_2), QVector2D(2.0/4.0, 2.0/3.0), QVector3D(-1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, -width_div_2), QVector2D(2.0/4.0, 1.0/3.0), QVector3D(-1.0, 0.0, 0.0)));

    // Над нами
     vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, width_div_2), QVector2D(2.0/4.0, 1.0), QVector3D(0.0, -1.0, 0.0)));
     vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, -width_div_2), QVector2D(2.0/4.0, 2.0/3.0), QVector3D(0.0, -1.0, 0.0)));
     vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, width_div_2), QVector2D(1.0/4.0, 1.0), QVector3D(0.0, -1.0, 0.0)));
     vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, -width_div_2), QVector2D(1.0/4.0, 2.0/3.0), QVector3D(0.0, -1.0, 0.0)));

     // Перед нами
     vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, -width_div_2), QVector2D(2.0/4.0, 2.0/3.0), QVector3D(0.0, 0.0, 1.0)));
     vertexes.append(VertexData(QVector3D(width_div_2,-width_div_2, -width_div_2), QVector2D(2.0/4.0, 1.0/3.0), QVector3D(0.0, 0.0, 1.0)));
     vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, -width_div_2), QVector2D(1.0/4.0, 2.0/3.0), QVector3D(0.0, 0.0, 1.0)));
     vertexes.append(VertexData(QVector3D(-width_div_2,- width_div_2, -width_div_2), QVector2D(1.0/4.0, 1.0/3.0), QVector3D(0.0, 0.0, 1.0)));

     // Слева от нас
     vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, width_div_2), QVector2D(0.0, 2.0/3.0), QVector3D(1.0, 0.0, 0.0)));
     vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, -width_div_2), QVector2D(1.0/4.0, 2.0/3.0), QVector3D(1.0, 0.0, 0.0)));
     vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, width_div_2), QVector2D(0.0, 1.0/3.0), QVector3D(1.0, 0.0, 0.0)));
     vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, -width_div_2), QVector2D(1.0/4.0, 1.0/3.0), QVector3D(1.0, 0.0, 0.0)));

     // Под нами
     vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, width_div_2), QVector2D(1.0/4.0, 0.0), QVector3D(0.0, 1.0, 0.0)));
     vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, -width_div_2), QVector2D(1.0/4.0, 1.0/3.0), QVector3D(0.0, 1.0, 0.0)));
     vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, width_div_2), QVector2D(2.0/4.0, 0.0), QVector3D(0.0, 1.0, 0.0)));
     vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, -width_div_2), QVector2D(2.0/4.0, 1.0/3.0), QVector3D(0.0, 1.0, 0.0)));

     QVector<GLuint> indexes;
     // каждая плоскость сформирована одинаково и добавление будет одинаково
     for(int i=0; i<24; i+=4){
         indexes.append(i + 0);
         indexes.append(i + 2);
         indexes.append(i + 1);

         indexes.append(i + 2);
         indexes.append(i + 3);
         indexes.append(i + 1);
     }

     m_cube = new SimpleObject3D(vertexes, indexes, texture);
}

SkyBox::~SkyBox()
{
    delete m_cube;
}

void SkyBox::draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions)
{
    m_cube->draw(program, functions);
}

void SkyBox::rotate(const QQuaternion &r)
{
    (void)r;
}

void SkyBox::translate(const QVector3D &t)
{
    (void)t;
}

void SkyBox::scale(const float &s)
{
    (void)s;
}

void SkyBox::setGlobalTransform(const QMatrix4x4 &g)
{
    (void)g;
}
