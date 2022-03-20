#ifndef RECTANGLEWINDOW_H
#define RECTANGLEWINDOW_H

#include <QOpenGLWindow>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

/*
 * this class derive from high level QOpenGLWindow, and
 * inplement the virtual member function initializeGL()
 * and paintGL functions
 */
class RectangleWindow : public QOpenGLWindow
{
public:
    RectangleWindow();
    virtual ~RectangleWindow() Q_DECL_OVERRIDE;

    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;

private:
    //wrap an OpenGL vertex array object
    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_vbo;
    QOpenGLBuffer m_ebo;

    //program for shader
    QOpenGLShaderProgram *m_program;
};

#endif // end of RECTANGLEWINDOW_H
