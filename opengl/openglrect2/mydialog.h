#ifndef MYDIALOG_H
#define MYDIALOG_H

#include <QDialog>

class RectangleWindow;

class MyDialog : public QDialog
{
    Q_OBJECT
public:
    MyDialog();

private slots:
    void onChangeColors();
    void onAnimateColors();

private:
    RectangleWindow * m_rectangleWindow;
};

#endif // MYDIALOG_H
