#include "openglwidget.h"
#include "shadermanager.h"
#include "texturemanager.h"
#include "primitives.h"
#include <QDebug>
#include <QMouseEvent>
#include <QWheelEvent>

OpenGLWidget::OpenGLWidget(QWidget* parent)
    : QOpenGLWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);
    cameraController = new CameraController(&scene, this);

    connect(cameraController, &CameraController::cameraUpdated, this, QOverload<>::of(&OpenGLWidget::update));
}

OpenGLWidget::~OpenGLWidget()
{
    makeCurrent();
    TextureManager::instance().clear();
    doneCurrent();
}

void OpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    if (!ShaderManager::instance().loadShaderProgram("default",
                                                     ":/shaders/vshader.vsh",
                                                     ":/shaders/fshader.fsh")) {
        qCritical() << "Failed to load shaders!";
        return;
    }

    program = ShaderManager::instance().getShaderProgram("default");

    TextureManager::instance().loadTexture(":/textures/magma.png", "magma");
    TextureManager::instance().loadTexture(":/textures/wood.png", "wood");
    TextureManager::instance().loadTexture(":/textures/cubes_gray.png", "cubes");

    Cube* cube = new Cube("magma");
    cube->initialize();
    cube->setScale(QVector3D (1.5f,1.5f,1.5f));
    scene.addShape(cube);


    Pyramid* pyramid = new Pyramid("wood");
    pyramid->initialize();
    pyramid->setPosition(QVector3D(0.0f, 1.35f, 0.0f));
    pyramid->setScale(QVector3D(1.5f, 1.2f, 1.5f));
    scene.addShape(pyramid);

    cameraController->updateCamera();
}

void OpenGLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    scene.resize(w, h);
}

void OpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (program) {
        program->bind();
        scene.renderAll(*program);
        program->release();
    }
}

void OpenGLWidget::mousePressEvent(QMouseEvent* event)
{
    cameraController->mousePressEvent(event);
    event->accept();
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent* event)
{
    cameraController->mouseMoveEvent(event);
    event->accept();
}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent* event)
{
    cameraController->mouseReleaseEvent(event);
    event->accept();
}

void OpenGLWidget::wheelEvent(QWheelEvent* event)
{
    cameraController->wheelEvent(event);
    event->accept();
}
