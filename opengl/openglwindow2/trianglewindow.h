#ifndef TRIANGLEWINDOW_H
#define TRIANGLEWINDOW_H

#include "openglwindow.h"
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

class TriangleWindow : public OpenGLWindow
{
public:
    TriangleWindow();
    ~TriangleWindow() Q_DECL_OVERRIDE;

    void initialize() Q_DECL_OVERRIDE;
    void render() Q_DECL_OVERRIDE;

private:
    //Wraps an OpenGL VertexArrayObject(VAO)
    QOpenGLVertexArrayObject m_vao;
    //Vertex buffer(only positions now).
    QOpenGLBuffer m_vertextBufferObject;

    //Holds the compiled shader programs
    QOpenGLShaderProgram *m_program;
};

#endif // TRIANGLEWINDOW_H
