#include "rectanglewindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    RectangleWindow window;
    window.resize(600, 400);
    window.show();
    return app.exec();
}
