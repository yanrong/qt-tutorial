#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
{
    setSurfaceType(QWindow::OpenGLSurface);
    QSurfaceFormat format;
    format.setProfile(QSurfaceFormat::CompatibilityProfile);
    format.setVersion(2,1);
    setFormat(format);

    context = new QOpenGLContext;
    context->setFormat(format);
    context->create();
    context->makeCurrent(this);

    opengGLFunctions = context->functions();
}

MainWindow::~MainWindow()
{
}

void MainWindow::initializeGL()
{
}

void MainWindow::resizeGL(int w, int h)
{
    Q_UNUSED(w);
    Q_UNUSED(h);
}

void MainWindow::paintGL()
{
    glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_QUADS);

    glVertex2d(-0.5f, -0.5f);
    glVertex2d(0.5f, -0.5f);
    glVertex2d(0.5f, 0.5f);
    glVertex2d(-0.5f, 0.5f);

    glEnd();
    glFlush();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    paintGL();
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    paintGL();
}
