#include "mydialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MyDialog dialog;
    dialog.show();
    return app.exec();
}
