#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QOpenGLWindow>
#include <QOpenGLFunctions>
#include <QSurfaceFormat>
#include <QResizeEvent>
#include <QPaintEvent>
#include <GL/glu.h>

class MainWindow : public QOpenGLWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);

private:
    QOpenGLContext *context;
    QOpenGLFunctions *opengGLFunctions;
};

#endif // MAINWINDOW_H
