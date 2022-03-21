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

    //update the scene and request a paint update
    void updateScene();
    void animateColorsTo(const std::vector<QColor>& toColors);

    //hold the vertex colors set on next call to updateScene()
    std::vector<QColor> m_vertexColors;

private:
    /*
     * modifies the scene a bit and call updateScene afterwards, then
     * already in final state, dosen't do anything
     */
    void aminate();

    //QT wrap an OpenGL vertex array object
    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_vbo;
    QOpenGLBuffer m_ebo;

    //program for shader
    QOpenGLShaderProgram *m_program;

    std::vector<GLfloat> m_vertexBufferData;
    //store the target colors that we animate towards
    std::vector<QColor> m_toColors;
    //store the target colors that we animate from
    std::vector<QColor> m_fromeColors;

    //number of frame used for the animate
    unsigned int m_frameCount;
};

#endif // end of RECTANGLEWINDOW_H
