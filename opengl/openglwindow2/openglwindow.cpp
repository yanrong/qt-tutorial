#include "openglwindow.h"

OpenGLWindow::OpenGLWindow(QWindow *parent)
    : QWindow(parent),
      m_context(nullptr)
{
    setSurfaceType(QWindow::OpenGLSurface);
}

void OpenGLWindow::initialize()
{
}

void OpenGLWindow::renderLater()
{
    requestUpdate();
}

bool OpenGLWindow::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::UpdateRequest:
        renderNow();
        return true;
    default:
        return QWindow::event(event);
    }
}

void OpenGLWindow::exposeEvent(QExposeEvent *event)
{
    Q_UNUSED(event);

    if (isExposed())
        renderNow();
}

void OpenGLWindow::renderNow()
{
    //only render if exposed
    if (!isExposed())
        return;

    bool needsInitialize = false;

    //initilaize on first call
    if (!m_context) {
        m_context = new QOpenGLContext(this);
        m_context->setFormat(requestedFormat());
        m_context->create();

        needsInitialize = true;
    }

    m_context->makeCurrent(this);

    if (needsInitialize) {
        initializeOpenGLFunctions();
        initialize(); //call user code
    }

    render(); //call user code

    m_context->swapBuffers(this);
}
