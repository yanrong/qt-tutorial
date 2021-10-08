#ifndef TRIANGLEWINDOW_H
#define TRIANGLEWINDOW_H

#include "openglwindow.h"
#include <QOpenGLShaderProgram>
#include <QScreen>

class TriangleWindow : public OpenGLWindow
{
public:
    using OpenGLWindow::OpenGLWindow;

    void initialize() override;
    void render() override;

private:
    GLint m_posAttr = 0;
    GLint m_colAttr = 0;
    GLint m_matrixUniform = 0;

    QOpenGLShaderProgram *m_program = nullptr;
    int m_frame = 0;
};

#endif // TRIANGLEWINDOW_H
