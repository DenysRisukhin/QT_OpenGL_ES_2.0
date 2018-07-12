#include "widget.h"

#include <QMouseEvent>
#include <QOpenGLContext>
#include <simpleobject3d.h>
#include <group3d.h>
#include <QtMath>
#include <camera3d.h>
#include <QKeyEvent>
#include <skybox.h>


#pragma comment (lib, "opengl32.lib")

Widget::Widget(QWidget *parent)
    : QOpenGLWidget(parent)//, m_texture(0), m_indexBuffer(QOpenGLBuffer::IndexBuffer)
{
    m_camera = new Camera3D;
    m_camera->translate(QVector3D(0.0f, 0.0f, -5.0f));
}

Widget::~Widget()
{
    delete m_camera;

    for (int i = 0; i < m_objects.size(); ++i)
        delete m_objects[i];

    for (int i = 0; i < m_TransformObjects.size(); ++i)
        delete m_TransformObjects[i];

    for (int i = 0; i < m_groups.size(); ++i)
        delete m_groups[i];
}

void Widget::initializeGL(){
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // 1 - не прозрачный  0 - прозрачный

    glEnable(GL_DEPTH_TEST); // вкл буфера глубины
    glEnable(GL_CULL_FACE);  // вкл отсечение задних граней что бы они не ресовались

    // загрузка шейдеров и их компиляция
    initShaders();

    float step = 2;

    m_groups.append(new Group3D);

    for(float x = -step; x <= step; x += step){
        for(float y = -step; y <= step; y += step){
            for(float z = -step; z <= step; z += step){
                initCube(1.0f);
                m_objects[m_objects.size() - 1]->translate(QVector3D(x, y, z));
                m_groups[m_groups.size() - 1]->addObject(m_objects[m_objects.size() - 1]);
            }
        }
    }
    m_groups[0]->translate(QVector3D(-4.0f, 0.0f, 0.0f));

    m_groups.append(new Group3D);
    for(float x = -step; x <= step; x += step){
        for(float y = -step; y <= step; y += step){
            for(float z = -step; z <= step; z += step){
                initCube(1.0f);
                m_objects[m_objects.size() - 1]->translate(QVector3D(x, y, z));
                m_groups[m_groups.size() - 1]->addObject(m_objects[m_objects.size() - 1]);
            }
        }
    }
    m_groups[1]->translate(QVector3D(4.0f, 0.0f, 0.0f));

    m_groups.append(new Group3D);
    m_groups[2]->addObject( m_groups[0]);
    m_groups[2]->addObject( m_groups[1]);

    m_TransformObjects.append( m_groups[2]);

    m_groups[0]->addObject(m_camera);

    m_skybox = new SkyBox(100,  QImage("C:/Den/Programming/QT/OGQ_Test/skybox.jpg"));

    m_timer.start(30, this);
}


void Widget::resizeGL(int w, int h){
    float aspect = w / (float)h;

    // формируем матрицу проекций
    m_projectionMatrix.setToIdentity(); // 1-ая матрица

    // настраивается плоскость отсечения камеры
    m_projectionMatrix.perspective(45, aspect, 0.01f, 1000.0f); // формируем матрицу перспективы
}

void Widget::paintGL(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // очистить буфер цвета и глубины

    m_programSkyBox.bind();

    // присваиваем значения Uniform для шейдера (эти значения нужно передать в шейдер)
    m_programSkyBox.setUniformValue("u_projectionMatrix", m_projectionMatrix);

    m_camera->draw(&m_programSkyBox);
    m_skybox->draw(&m_programSkyBox, context()->functions());
    m_programSkyBox.release();

    // биндим програму что бы иметь к ней доступ
    m_program.bind();

    // присваиваем значения Uniform для шейдера (эти значения нужно передать в шейдер)
    m_program.setUniformValue("u_projectionMatrix", m_projectionMatrix);
    m_program.setUniformValue("u_lightPosition", QVector4D(0.0, 0.0, 0.0, 1.0));
    m_program.setUniformValue("u_lightPower", 1.0f);

    m_camera->draw(&m_program);

    for (int i=0;i < m_TransformObjects.size(); i++){
        m_TransformObjects[i]->draw(&m_program, context()->functions());
    }
    m_program.release();
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    //event->pos() // возвр глобальные координаты указателя мыши относительно левого верхнего угла экрана
    //event->localPos(); // вернет локальные коорд указателя мыши относ верхн лев угла

    if(event->buttons() == Qt::LeftButton){
         m_mousePosition = QVector2D(event->localPos());
    }

    event->accept(); // подтверждает срабатывание события
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
     if(event->buttons() != Qt::LeftButton) return;

     QVector2D diff = QVector2D(event->localPos()) - m_mousePosition; // вектор направления
     m_mousePosition = QVector2D(event->localPos());

     float angle = diff.length() / 2.0; // для поворота (а деление на 2 - что бы поворот небыл быстрым)

     QVector3D axis = QVector3D(diff.y(), diff.x(), 0.0); // вектор по которому осущ поворот

     m_camera->rotate(QQuaternion::fromAxisAndAngle(axis, angle)); // умножаем на осуществляемые
     // повороты для корректного отображения(если не домножить то будет все дергаться)

     // далее после того как мы посчитали ноый поворот нужно обновить картинку
     update();

     // m_rotation нужно применить при отрисовке в paintGL()

}

// delta - на сколько повернули колесико
void Widget::wheelEvent(QWheelEvent *event)
{
    if (event->delta() > 0){
        m_camera->translate(QVector3D(0.0f, 0.0f, 0.25f));
    } else if (event->delta() < 0){
         m_camera->translate(QVector3D(0.0f, 0.0f, -0.25f));
    }
    // перерисовываем сцену
    update();
}

void Widget::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);

    for(int i = 0; i < m_objects.size(); ++i){
        if(i % 2 == 0){
            m_objects[i]->rotate(QQuaternion::fromAxisAndAngle(1.0f, 0.0f, 0.0f, qSin(angleObject)));
            m_objects[i]->rotate(QQuaternion::fromAxisAndAngle(0.0f, 1.0f, 0.0f, qCos(angleObject)));
        } else {
            m_objects[i]->rotate(QQuaternion::fromAxisAndAngle(0.0f, 1.0f, 0.0f, qSin(angleObject)));
            m_objects[i]->rotate(QQuaternion::fromAxisAndAngle(1.0f, 0.0f, 0.0f, qCos(angleObject)));
        }
    }

    m_groups[0]->rotate(QQuaternion::fromAxisAndAngle(0.0f, 0.0f, 1.0f, qSin(angleGroup1)));
    m_groups[0]->rotate(QQuaternion::fromAxisAndAngle(0.0f, 1.0f, 0.0f, -qSin(angleGroup1)));

    m_groups[1]->rotate(QQuaternion::fromAxisAndAngle(0.0f, 0.0f, 0.0f, qCos(angleGroup2)));
    m_groups[1]->rotate(QQuaternion::fromAxisAndAngle(0.0f, 1.0f, 0.0f, -qCos(angleGroup2)));

    m_groups[2]->rotate(QQuaternion::fromAxisAndAngle(1.0f, 0.0f, 0.0f, qSin(angleMain)));
    m_groups[2]->rotate(QQuaternion::fromAxisAndAngle(0.0f, 1.0f, 0.0f, qCos(angleMain)));

    angleObject += M_PI / 180.0f;
    angleGroup1 += M_PI / 360.0f;
    angleGroup2 += M_PI / 360.0f;
    angleMain += M_PI / 720.0f;

    update();
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()){
    case Qt::Key_Left:
        m_groups[0]->delObject(m_camera);
        m_groups[1]->addObject(m_camera);
        break;
    case Qt::Key_Right:
        m_groups[1]->delObject(m_camera);
        m_groups[0]->addObject(m_camera);
        break;
    case Qt::Key_Down:
        m_groups[1]->delObject(m_camera);
        m_groups[0]->delObject(m_camera);
        break;
    case Qt::Key_Up:
        m_groups[1]->delObject(m_camera);
        m_groups[0]->delObject(m_camera);
        QMatrix4x4 tmp;
        tmp.setToIdentity();
        m_camera->setGlobalTransform(tmp);
        break;
    }
    update();
}

void Widget::initShaders(){
   if (!m_program.addShaderFromSourceFile(QOpenGLShader::Vertex, "C:/Den/Programming/QT/OGQ_Test/vshader.vsh")) // ":/vshader.vsh"))
           close();

   if (!m_program.addShaderFromSourceFile(QOpenGLShader::Fragment, "C:/Den/Programming/QT/OGQ_Test/fshader.fsh"))
           close();

   // объединяем все шейдеры в 1 и прокидуем varing переменные
   if(!m_program.link())
       close();

   // биндить нужно перед отрисовкой

   if (!m_programSkyBox.addShaderFromSourceFile(QOpenGLShader::Vertex, "C:/Den/Programming/QT/OGQ_Test/skybox.vsh"))
           close();

   if (!m_programSkyBox.addShaderFromSourceFile(QOpenGLShader::Fragment, "C:/Den/Programming/QT/OGQ_Test/skybox.fsh"))
           close();

   // объединяем все шейдеры в 1 и прокидуем varing переменные
   if(!m_programSkyBox.link())
       close();
}

void Widget::initCube(float width){

    // width - ширина любой гани куба(размер ребра)

    // куб отцентрированный поэтому width / 2.0
    float width_div_2 = width / 2.0f;

    QVector<VertexData> vertexes;
    vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, width_div_2), QVector2D(0.0, 1.0), QVector3D(0.0, 0.0, 1.0)));
    vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, width_div_2), QVector2D(0.0, 0.0), QVector3D(0.0, 0.0, 1.0)));
    vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, width_div_2), QVector2D(1.0, 1.0), QVector3D(0.0, 0.0, 1.0)));
    vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, width_div_2), QVector2D(1.0, 0.0), QVector3D(0.0, 0.0, 1.0)));

    vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, width_div_2), QVector2D(0.0, 1.0), QVector3D(1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, width_div_2), QVector2D(0.0, 0.0), QVector3D(1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, -width_div_2), QVector2D(1.0, 1.0), QVector3D(1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, -width_div_2), QVector2D(1.0, 0.0), QVector3D(1.0, 0.0, 0.0)));

     vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, width_div_2), QVector2D(0.0, 1.0), QVector3D(0.0, 1.0, 0.0)));
     vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, -width_div_2), QVector2D(0.0, 0.0), QVector3D(0.0, 1.0, 0.0)));
     vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, width_div_2), QVector2D(1.0, 1.0), QVector3D(0.0, 1.0, 0.0)));
     vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, -width_div_2), QVector2D(1.0, 0.0), QVector3D(0.0, 1.0, 0.0)));

     vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, -width_div_2), QVector2D(0.0, 1.0), QVector3D(0.0, 0.0, -1.0)));
     vertexes.append(VertexData(QVector3D(width_div_2,-width_div_2, -width_div_2), QVector2D(0.0, 0.0), QVector3D(0.0, 0.0, -1.0)));
     vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, -width_div_2), QVector2D(1.0, 1.0), QVector3D(0.0, 0.0, -1.0)));
     vertexes.append(VertexData(QVector3D(-width_div_2,- width_div_2, -width_div_2), QVector2D(1.0, 0.0), QVector3D(0.0, 0.0, -1.0)));

     vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, width_div_2), QVector2D(0.0, 1.0), QVector3D(-1.0, 0.0, 0.0)));
     vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, -width_div_2), QVector2D(0.0, 0.0), QVector3D(-1.0, 0.0, 0.0)));
     vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, width_div_2), QVector2D(1.0, 1.0), QVector3D(-1.0, 0.0, 0.0)));
     vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, -width_div_2), QVector2D(1.0, 0.0), QVector3D(-1.0, 0.0, 0.0)));

     vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, width_div_2), QVector2D(0.0, 1.0), QVector3D(0.0, -1.0, 0.0)));
     vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, -width_div_2), QVector2D(0.0, 0.0), QVector3D(0.0, -1.0, 0.0)));
     vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, width_div_2), QVector2D(1.0, 1.0), QVector3D(0.0, -1.0, 0.0)));
     vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, -width_div_2), QVector2D(1.0, 0.0), QVector3D(0.0, -1.0, 0.0)));

     QVector<GLuint> indexes;
     // каждая плоскость сформирована одинаково и добавление будет одинаково
     for(int i=0; i<24; i+=4){
         indexes.append(i + 0);
         indexes.append(i + 1);
         indexes.append(i + 2);
         indexes.append(i + 2);
         indexes.append(i + 1);
         indexes.append(i + 3);
     }

    m_objects.append(new SimpleObject3D(vertexes, indexes, QImage("C:/Den/Programming/QT/OGQ_Test/cube.jpg")));
}
