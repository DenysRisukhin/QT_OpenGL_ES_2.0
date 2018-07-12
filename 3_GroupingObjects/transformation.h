#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

class QQuaternion;
class QVector3D;
class QMatrix4x4;
class QOpenGLShaderProgram;
class QOpenGLFunctions;

// abstract class Transformation подразумевает обЪект который можно трансформировать
// вращать перемещать масштабировать

class Transformational{
public:

    virtual void rotate(const QQuaternion &r) = 0;

    //
    virtual void translate(const QVector3D &t) = 0;

    //
    virtual void scale(const float &s) = 0;

    // 1 метод применения к текущей локальной системе координат преобразования
    // которое выставляет эту лок систему координат в пределах глобальной системы
    // координат
    // 2 метод будет задавать позицию и поворот в этой лок системе координат которая
    // относится к этому объкту трансфрмации в пределах родительской для нее
    // глобальной системы координат
    virtual void setGlobalTransform(const QMatrix4x4 &g) = 0;

    //
    virtual void draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions) = 0;
};

#endif // TRANSFORMATION_H
