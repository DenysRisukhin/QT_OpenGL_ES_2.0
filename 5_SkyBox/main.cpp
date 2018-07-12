#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // убираем зернистость
    //QSurfaceFormat format;
   // format.setSamples(16);

    // устанавливаем глубину буферного буфера(в основном для Андроида)
  //  format.setDepthBufferSize(24);

    // устанавл дефолтный формат для всего openGL
   // QSurfaceFormat::setDefaultFormat(format);

    Widget w;
    w.show();

    return a.exec();
}
