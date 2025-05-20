#include "openglwidget.h"
#include "shadermanager.h"
#include "cube.h"
#include <QDebug>
#include <QMouseEvent>
#include <QWheelEvent>

OpenGLWidget::OpenGLWidget(QWidget* parent)
    : QOpenGLWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);
    cameraController = new CameraController(&scene, this);

    // Добавляем соединение сигнала и слота
    connect(cameraController, &CameraController::cameraUpdated, this, QOverload<>::of(&OpenGLWidget::update));
}

OpenGLWidget::~OpenGLWidget()
{
    makeCurrent();
    // Освобождение ресурсов, если необходимо
    doneCurrent();
}

void OpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    // Загрузка шейдеров через ShaderManager
    if (!ShaderManager::instance().loadShaderProgram("default",
                                                     ":/shaders/vshader.vsh",
                                                     ":/shaders/fshader.fsh")) {
        qCritical() << "Failed to load shaders!";
        return;
    }

    // Просто сохраняем указатель, без копирования
    program = ShaderManager::instance().getShaderProgram("default");

    // Остальной код инициализации...
    TextureManager::instance().loadTexture(":/textures/water.png", "water");
    Cube* cube = new Cube("water");
    cube->initialize();
    scene.addShape(cube);

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
