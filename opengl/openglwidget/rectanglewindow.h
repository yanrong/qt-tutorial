#ifndef RECTANGLEWINDOW_H
#define RECTANGLEWINDOW_H

#include <QOpenGLWidget>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QSurfaceFormat>
#include <QOpenGLShaderProgram>

#include <vector>

class RectangleWindow : public QOpenGLWidget, protected QOpenGLFunctions
{
public:
    RectangleWindow(QWidget * parent = nullptr);
    virtual ~RectangleWindow() Q_DECL_OVERRIDE;

    //update the scene and request a paint update
    void updateScene();
    void animateColorsTo(const std::vector<QColor> & toColors);

    //hold the vertex color set on next call to updateScene()
    std::vector<QColor> m_vertexColors;
protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;

private:
    //update the scene a bit until to final state
    void animate();

    //OpenGL contex member
    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_vbo, m_ebo;
    QOpenGLShaderProgram *m_program;
    std::vector<float> m_vertexBuffer;
    std::vector<QColor> m_fromColors, m_toColors;
    unsigned int m_frameCount;
};

#endif // RECTANGLEWINDOW_H
