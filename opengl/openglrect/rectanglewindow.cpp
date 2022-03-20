#include "rectanglewindow.h"
#include <QSurfaceFormat>
#include <QDebug>
#include <QColor>
#include <vector>
#include <GL/gl.h>

RectangleWindow::RectangleWindow()
    : m_program(nullptr)
{
    QSurfaceFormat format;
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setVersion(3, 3);
    this->setFormat(format);
}

RectangleWindow::~RectangleWindow()
{
    //clean up
    //make to render contex to cleaning up resources
    makeCurrent();

    m_vao.destroy();
    m_vbo.destroy();
    m_ebo.destroy();

    delete m_program;
}

void RectangleWindow::initializeGL()
{
    // this function is called once, when the window is firs show or rendered
    // build and compile shader program

    m_program = new QOpenGLShaderProgram();

    //read the shader program source from the local files
    if (!m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/openglrect.vert")){
        qDebug() << "vertex shader errors:\n" << m_program->log();
    }
    if (!m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/openglrect.frag")){
        qDebug() << "fragment shader errors:\n" << m_program->log();
    }

    if (!m_program->link()) {
        qDebug() << "shader link error\n" << m_program->log();
    }

    //set up vertex to build geometry primitive
    GLfloat vertices[] = {
         0.8f,  0.8f, 0.0f,  // top right
         0.8f, -0.8f, 0.0f,  // bottom right
        -0.8f, -0.8f, 0.0f,  // bottom left
        -0.8f,  0.8f, 0.0f   // top left
    };

    QColor colors [] = {
        QColor(0xf6a509),
        QColor(0xcb2dde),
        QColor(0x0eeed1),
        QColor(0x068918),
    };
    //create buffer to assemble the vertex indices and vertex color
    std::vector<GLfloat> bufferData(2 * 4 * 3);

    GLfloat* buf = bufferData.data();
    //the data totoal have 4 liens, each line contain 6 element,
    //3 vertex indices and 3 vertex color
    //copy data with pattern v1v2v3-c1c2c3
    for (int i = 0; i < 4; i++, buf += 6) {
        //coordinates
        buf[0] = vertices[3 * i];
        buf[1] = vertices[3 * i + 1];
        buf[2] = vertices[3 * i + 2];
        //colors
        buf[3] = colors[i].redF();
        buf[4] = colors[i].greenF();
        buf[5] = colors[i].blueF();
    }

    m_vbo = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    m_vbo.create();
    m_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vbo.bind();
    m_vbo.allocate(bufferData.data(), bufferData.size() * sizeof(GLfloat));

    //create and bind verte array object -must be bound before the element buffer is bound
    m_vao.create();
    m_vao.bind();

    GLuint indices[] = {
        0, 1, 3, //first triangle
        1, 2, 3 //second triangle
    };
    //crate new buffer for element buffer
    m_ebo = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer); //crate an index buffer
    m_ebo.create();
    m_ebo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_ebo.bind();
    m_ebo.allocate(indices, sizeof(indices));

    //stride, we just assemble the vertex and color before, the opengl get the 6 data each time
    int stride = 6 * sizeof(GLfloat);

    //layout location 0 - vec3 hold coordinate
    m_program->enableAttributeArray(0);
    m_program->setAttributeBuffer(0, GL_FLOAT, 0, 3, stride);
    //layout location 1 - vec3 with colors
    m_program->enableAttributeArray(1);
    m_program->setAttributeBuffer(1, GL_FLOAT, 3 * sizeof(GLfloat), 3, stride);
}

void RectangleWindow::paintGL()
{
    //set the background color
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    //use our shader program
    m_program->bind();
    //bind the vertex array object, The opengl just like status machine, we
    //already set the VAO status, if we want to use it, just bind it to current contex
    m_vao.bind();
    // For old Intel drivers you may need to explicitely re-bind the index buffer, because
    // these drivers do not remember the binding-state of the index/element-buffer in the VAO
    // m_ebo.bind();
    glDrawElements(GL_TRIANGLES, 6 , GL_UNSIGNED_INT, nullptr);
    //finally relase VAO again
    m_vao.release();
}
