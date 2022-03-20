#include "rectanglewindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    RectangleWindow window;
    window.resize(640, 800);
    window.show();
    return app.exec();
}
