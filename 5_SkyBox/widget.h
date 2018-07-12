#ifndef WIDGET_H
#define WIDGET_H

#include <QOpenGLWidget>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>
#include <simpleobject3d.h>
#include <QBasicTimer>

class SimpleObject3D;
class Transformational;
class Group3D;
class Camera3D;
class SkyBox;

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
    // сдесь настраивается плоскость отсечения камеры
    void resizeGL(int w, int h);

    // выз каждый раз при перерисовке содержимого окна(при изменении размеров окна, если дать явную команду
    // перерисовке с помощью update())
    void paintGL();

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void timerEvent(QTimerEvent *event);
    void keyPressEvent(QKeyEvent *event);

    void initShaders();
    void initCube(float);



private:
    QMatrix4x4 m_projectionMatrix;
    QOpenGLShaderProgram m_program;
    QOpenGLShaderProgram m_programSkyBox;
   // QOpenGLTexture *m_texture;

    // для работы куба два буфера
   // QOpenGLBuffer m_arrayBuffer; // по умолчанию вершинный(поэтому явно инициализировать ненужно)
   // QOpenGLBuffer m_indexBuffer; // сразу требует инициализации в конструкторе(указываем что он индексный)

    QVector2D m_mousePosition;

   // SimpleObject3D m_objects;
    QVector<SimpleObject3D *> m_objects;
    QVector<Transformational *> m_TransformObjects;
    QVector<Group3D *> m_groups;

    QBasicTimer m_timer;
    float angleObject;
    float angleGroup1;
    float angleGroup2;
    float angleMain;

    Camera3D *m_camera;
    SkyBox *m_skybox;
};

#endif // WIDGET_H
