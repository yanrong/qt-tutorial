#include "rectanglewindow.h"
#include <QDebug>
#include <GL/gl.h>

RectangleWindow::RectangleWindow()
{
    QSurfaceFormat format;
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setVersion(3, 3);
    this->setFormat(format);

    m_vertexColors = {QColor(0xf6a509), QColor(0xcb2dde),
                      QColor(0x0eeed1), QColor(0x068918)};
    m_frameCount = 5000;
    m_program = nullptr;
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
    if (!m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/openglrect2.vert")){
        qDebug() << "vertex shader errors:\n" << m_program->log();
    }
    if (!m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/openglrect2.frag")){
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

    //create buffer to assemble the vertex indices and vertex color
    m_vertexBufferData.resize(2 * 3 * 4);
    GLfloat* buf = m_vertexBufferData.data();

    //the data totoal have 4 lines in the buffer array, each line contain 6 element,
    //3 vertex indices and 3 vertex color, copy data with pattern v1v2v3c1c2c3
    for (int i = 0; i < 4; i++, buf += 6) {
        //coordinates
        buf[0] = vertices[3 * i];
        buf[1] = vertices[3 * i + 1];
        buf[2] = vertices[3 * i + 2];
        //colors
        buf[3] = m_vertexColors[i].redF();
        buf[4] = m_vertexColors[i].greenF();
        buf[5] = m_vertexColors[i].blueF();
    }

    m_vbo = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    m_vbo.create();
    m_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vbo.bind();
    m_vbo.allocate(m_vertexBufferData.data(), m_vertexBufferData.size() * sizeof(GLfloat));

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

    //layout location 0 - vec3 hold coordinate
    m_program->enableAttributeArray(0);
    m_program->setAttributeBuffer(0, GL_FLOAT, 0, 3, 6 * sizeof(GLfloat));
    //layout location 1 - vec3 with colors
    m_program->enableAttributeArray(1);
    m_program->setAttributeBuffer(1, GL_FLOAT, 3 * sizeof(GLfloat), 3, 6 * sizeof(GLfloat));
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

    animate();
}

void RectangleWindow::updateScene()
{
    //for update color
    //get the buffer data address and update it's value
    float *buf = m_vertexBufferData.data();
    for (int i = 0; i < 4; i++, buf += 6) {
        buf[3] = m_vertexColors[i].redF();
        buf[4] = m_vertexColors[i].greenF();
        buf[5] = m_vertexColors[i].blueF();
    }

    //set upt the current render contex
    this->makeCurrent();
    //bind the VBO and send the data to GPU's buffer
    m_vbo.bind();
    m_vbo.allocate(m_vertexBufferData.data(), m_vertexBufferData.size() * sizeof(GLfloat));

    //request an update
    update();
}

void RectangleWindow::animateColorsTo(const std::vector<QColor>& toColors)
{
    m_fromColors = m_vertexColors;
    m_toColors = toColors;
    m_frameCount = 0;

    animate();
}

void RectangleWindow::animate()
{
    const unsigned int FRMAMECOUNT = 120;
    //if already at framecount end, stop
    if (++m_frameCount > FRMAMECOUNT) {
        return ;//stop the frame rendering
    }
    //update colors
    double alpha = double(m_frameCount) / FRMAMECOUNT;

    //linear blending in HSV space
    for (unsigned int i = 0; i < m_vertexColors.size(); i++) {
        double fromH, fromS, fromV;
        m_fromColors[i].getHsvF(&fromH, &fromS, &fromV);
        double toH, toS, toV;
        m_toColors[i].getHsvF(&toH, &toS, &toV);

        m_vertexColors[i] = QColor::fromHsvF(toH * alpha + fromH * (1 - alpha),
                                             toS * alpha + fromS * (1 - alpha),
                                             toV * alpha + fromV * (1 - alpha));
    }

    updateScene();
}
