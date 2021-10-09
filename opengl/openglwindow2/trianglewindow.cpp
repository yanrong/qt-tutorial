#include "trianglewindow.h"
#include <QDebug>

TriangleWindow::TriangleWindow() :
    m_program(nullptr)
{
}

TriangleWindow::~TriangleWindow()
{
    // since we release resources related to an OpenGL context,
    // we make this context current before cleaning up our resources
    bool success = m_context->makeCurrent(this);
    if (!success)
        qDebug() << "Cannot make OpenGL context current.";
    m_vao.destroy();
    m_vertextBufferObject.destroy();
    delete m_program;
}

void TriangleWindow::initialize()
{
    m_program = new QOpenGLShaderProgram(this);

    if(m_program->addCacheableShaderFromSourceFile(
                QOpenGLShader::Vertex, ":/shaders/pass_through.vert"))
    {
        qDebug() << "Vertex shader errors: \n" << m_program->log();
    }

    if(m_program->addCacheableShaderFromSourceFile(
                QOpenGLShader::Fragment, ":/shaders/uniform_color.frag"))
    {
        qDebug() << "Fragment shader errors: \n" << m_program->log();
    }

    if (!m_program->link())
        qDebug() << "Shader linker errors :\n" << m_program->log();

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };

    //create a new buffer for the vertices
    m_vertextBufferObject = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer); //VBO
    m_vertextBufferObject.create(); //crate underlying OpenGL object
    m_vertextBufferObject.setUsagePattern(QOpenGLBuffer::StaticDraw); // must be called before allocate

    // set it active in the context, so that we can write to it
    m_vertextBufferObject.bind();
    // int bufSize = sizeof(vertices) = 9 * sizeof(float) = 9*4 = 36 bytes
    m_vertextBufferObject.allocate(vertices, sizeof(vertices));  // copy data into buffer
    // Initialize the Vertex Array Object (VAO) to record and remember subsequent attribute assocations with
    // generated vertex buffer(s)
    m_vao.create();  // create underlying OpenGL object
    m_vao.bind(); // sets the Vertex Array Object current to the OpenGL context so it monitors attribute assignments
    // now all following enableAttributeArray(), disableAttributeArray() and setAttributeBuffer() calls are
    // "recorded" in the currently bound VBA.

    // Enable attribute array at layout location 0
    m_program->enableAttributeArray(0);
    m_program->setAttributeBuffer(0, GL_FLOAT, 0, 3);
    // This maps the data we have set in the VBO to the "position" attribute.
    // 0 - offset - means the "position" data starts at the begin of the memory array
    // 3 - size of each vertex (=vec3) - means that each position-tuple has the size of 3 floats (those are the 3 coordinates,
    //     mind: this is the size of GL_FLOAT, not the size in bytes!
}

void TriangleWindow::render()
{
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    // set the background color = clear color
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // use our shader program
    m_program->bind();
    // bind the vertex array object, which in turn binds the vertex buffer object and
    // sets the attribute buffer in the OpenGL context
    m_vao.bind();
    // now draw the triangles:
    // - GL_TRIANGLES - draw individual triangles
    // - 0 index of first triangle to draw
    // - 3 number of vertices to process
    glDrawArrays(GL_TRIANGLES, 0, 3);
    // finally release VAO again (not really necessary, just for completeness)
    m_vao.release();
}
