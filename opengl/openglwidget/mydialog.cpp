#include "mydialog.h"
#include "rectanglewindow.h"

#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>

MyDialog::MyDialog() :
#ifdef Q_OS_WIN
    QDialog(nullptr, Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint)
#else
    QDialog(nullptr, Qt::Window)
#endif
{
    m_rectangleWindow = new RectangleWindow(this);

    /* create the layout and insert widget container */
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setMargin(0);
    vbox->setSpacing(0);
    vbox->addWidget(m_rectangleWindow);

    //create buttons for switch
    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->setMargin(0);

    QPushButton *changeColorBtn = new QPushButton(tr("Change Colors"), this);
    connect(changeColorBtn, &QPushButton::clicked, this, &MyDialog::onChangeColors);
    QPushButton *changeColorAnimationBtn = new QPushButton(tr("Animate colors"), this);
    connect(changeColorAnimationBtn, &QPushButton::clicked, this, &MyDialog::onAnimateColors);
    QPushButton *closeBtn = new QPushButton(tr("Close"), this);
    connect(closeBtn, &QPushButton::clicked, this, &MyDialog::accept);

    hbox->addWidget(changeColorBtn);
    hbox->addWidget(changeColorAnimationBtn);
    hbox->addStretch();
    hbox->addWidget(closeBtn);

    vbox->addLayout(hbox);

    setLayout(vbox);
    qsrand(428); // for now use fixed seed so that we get the same colors every time
}

void MyDialog::onChangeColors()
{
    //generate the colors by randomly and change them in the OpenGL window
    for (unsigned int i = 0; i < 4; i++) {
        m_rectangleWindow->m_vertexColors[i].setRgbF(rand() * 1.0 / RAND_MAX, rand() * 1.0 / RAND_MAX, rand() * 1.0 / RAND_MAX);
    }
    // now update the scene -> this will also request an update
    m_rectangleWindow->updateScene();
}

void MyDialog::onAnimateColors()
{
    //randomize the colors and change them in the OpenGL window
    std::vector<QColor> newColor(4);
    for (unsigned int i = 0; i < 4; i++) {
        newColor[i].setRgbF(rand() * 1.0 / RAND_MAX, rand() * 1.0 / RAND_MAX, rand() * 1.0 / RAND_MAX);
    }

    //now update the scene
    m_rectangleWindow->animateColorsTo(newColor);
}
