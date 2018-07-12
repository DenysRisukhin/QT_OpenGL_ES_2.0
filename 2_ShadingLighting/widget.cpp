#include "widget.h"

#include <QMouseEvent>
#include <QOpenGLContext>
#include <simpleobject3d.h>

#pragma comment (lib, "opengl32.lib")

Widget::Widget(QWidget *parent)
    : QOpenGLWidget(parent)//, m_texture(0), m_indexBuffer(QOpenGLBuffer::IndexBuffer)
{
}

Widget::~Widget()
{

}

void Widget::initializeGL(){
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // 1 - не прозрачный  0 - прозрачный

    glEnable(GL_DEPTH_TEST); // вкл буфера глубины
    glEnable(GL_CULL_FACE);  // вкл отсечение задних граней что бы они не ресовались

    // загрузка шейдеров и их компиляция
    initShaders();
    initCube(1.0f);
}

void Widget::resizeGL(int w, int h){
    float aspect = w / (float)h;

    // формируем матрицу проекций
    m_projectionMatrix.setToIdentity(); // 1-ая матрица
    m_projectionMatrix.perspective(45, aspect, 0.1f, 10.f); // формируем матрицу перспективы
}

void Widget::paintGL(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // очистить буфер цвета и глубины

    // модельно видовая матрица(матрица камеры)
    QMatrix4x4 viewMatrix;
    viewMatrix.setToIdentity(); // делаем ее 1-ой
    viewMatrix.translate(0.0, 0.0, -5.0); // отодвигаем камеру немного назад
    viewMatrix.rotate(m_rotation);

    //viewMatrix.rotate(30, 1.0, 0.0, 0.0);
   // viewMatrix.rotate(30, 0.0, 1.0, 0.0);

   // QMatrix4x4 modelMatrix;
  //  modelMatrix.setToIdentity();

    // биндим текстуру под номером 0 для дальнйшей отрисовке
    // этот номер должен совпадать котрый передали как Uniform значение
    //m_texture->bind(0);

    // биндим програму что бы иметь к ней доступ
    m_program.bind();

    // присваиваем значения Uniform для шейдера (эти значения нужно передать в шейдер)
    m_program.setUniformValue("u_projectionMatrix", m_projectionMatrix);
    m_program.setUniformValue("u_viewMatrix", viewMatrix);
   // m_program.setUniformValue("u_modelMatrix", modelMatrix);
  //  m_program.setUniformValue("u_texture", 0); // 0 - номер текстуры которая будет отрисовываться
    m_program.setUniformValue("u_lightPosition", QVector4D(0.0, 0.0, 0.0, 1.0));
    m_program.setUniformValue("u_lightPower", 1.5f);

    //for (int i=0;i < m_obects->size(); i++){
        m_objects[0]->draw(&m_program, context()->functions());
   // }

//    m_arrayBuffer.bind();

//    int offset = 0;

//    // получаем располжение вершинного атрибута: qt_Vertex и qt_MultiTexCoord0 координаты
//    int vertLoc = m_program.attributeLocation("a_position");

//    // вкл использование этого атрибута
//    m_program.enableAttributeArray(vertLoc);
//    m_program.setAttributeBuffer(vertLoc, GL_FLOAT, offset, 3, sizeof(VertexData));

//    offset += sizeof(QVector3D);

//    int texLoc = m_program.attributeLocation("a_texcoord");
//    m_program.enableAttributeArray(texLoc);
//    m_program.setAttributeBuffer(texLoc, GL_FLOAT, offset, 2, sizeof(VertexData));

//    offset += sizeof(QVector2D);

//    int normLoc = m_program.attributeLocation("a_normal");
//    m_program.enableAttributeArray(normLoc);
//    m_program.setAttributeBuffer(normLoc, GL_FLOAT, offset, 3, sizeof(VertexData));


//    // биндим индексный буфер
//    m_indexBuffer.bind();

//    // вызываем отрисовку
//    // GL_TRIANGLES - примитивы которыми будем рисовать
//    // m_indexBuffer.size() кол-во эл-ов для отрисовки
//    glDrawElements(GL_TRIANGLES, m_indexBuffer.size(), GL_UNSIGNED_INT, 0);

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

     m_rotation = QQuaternion::fromAxisAndAngle(axis, angle) * m_rotation; // умножаем на осуществляемые
     // повороты для корректного отображения(если не домножить то будет все дергаться)

     // далее после того как мы посчитали ноый поворот нужно обновить картинку
     update();

     // m_rotation нужно применить при отрисовке в paintGL()

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
//     // создаем и заполняем вершинный и индексный буфера
//     m_arrayBuffer.create();
//     m_arrayBuffer.bind(); // размещение проекции буфера в видеопамять
//     // загрузка буфера
//     m_arrayBuffer.allocate(vertexes.constData(), vertexes.size() * sizeof(VertexData)); // (загружаем данные ,кол-во в байтах)
//     m_arrayBuffer.release();

//     m_indexBuffer.create();
//     m_indexBuffer.bind();
//     m_indexBuffer.allocate(indexes.constData(), indexes.size() * sizeof(GLuint));
//     m_indexBuffer.release();

//     // создаем тексуру отраженную по вертикали(особенность OpenGL- результат будет корректным)
//     m_texture = new QOpenGLTexture(QImage("C:/Den/Programming/QT/OGQ_Test/cube.jpg").mirrored());

//     m_texture->setMinificationFilter(QOpenGLTexture::Nearest);

//     m_texture->setMagnificationFilter(QOpenGLTexture::Linear);

//     m_texture->setWrapMode(QOpenGLTexture::Repeat);
}
