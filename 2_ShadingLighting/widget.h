#ifndef WIDGET_H
#define WIDGET_H

#include <QOpenGLWidget>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>
#include <simpleobject3d.h>

class SimpleObject3D;
//struct VertexData{
//    VertexData(QVector3D p, QVector2D t, QVector3D n) :
//        position(p), texCoord(t), normal(n) {}

//    VertexData() {}

//    QVector3D position;
//    QVector2D texCoord;
//    QVector3D normal; // нормаль данной точки
//};

class Widget : public QOpenGLWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

protected:
    void initializeGL(); // вызыв 1 раз при создании окна

     // вызывается каждый раз при изменении размеров виджета
    // в нем настраивают матрицу проекций используемую для отбражения
    void resizeGL(int w, int h);

    // выз каждый раз при перерисовке содержимого окна(при изменении размеров окна, если дать явную команду
    // перерисовке с помощью update())
    void paintGL();

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    void initShaders();
    void initCube(float);



private:
    QMatrix4x4 m_projectionMatrix;
    QOpenGLShaderProgram m_program;
   // QOpenGLTexture *m_texture;

    // для работы куба два буфера
   // QOpenGLBuffer m_arrayBuffer; // по умолчанию вершинный(поэтому явно инициализировать ненужно)
   // QOpenGLBuffer m_indexBuffer; // сразу требует инициализации в конструкторе(указываем что он индексный)

    QVector2D m_mousePosition;
    QQuaternion m_rotation;

   // SimpleObject3D m_objects;
    QVector<SimpleObject3D *> m_objects;
};

#endif // WIDGET_H
