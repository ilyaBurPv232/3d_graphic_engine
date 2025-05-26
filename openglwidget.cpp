#include "openglwidget.h"
#include "shadermanager.h"
#include "texturemanager.h"
#include "primitives.h"
#include "skybox.h"
#include <QDebug>
#include <QMouseEvent>
#include <QWheelEvent>

OpenGLWidget::OpenGLWidget(QWidget* parent)
    : QOpenGLWidget(parent), frameCount(0), lastTime(0)
{
    setFocusPolicy(Qt::StrongFocus);
    cameraController = new CameraController(&scene, this);

    fpsTimer = new QTimer(this);
    connect(fpsTimer, &QTimer::timeout, this, &OpenGLWidget::updateFPS);
    fpsTimer->start(100);

    connect(cameraController, &CameraController::cameraUpdated, this, QOverload<>::of(&OpenGLWidget::update));

    QTimer::singleShot(0, this, QOverload<>::of(&OpenGLWidget::update));
}

OpenGLWidget::~OpenGLWidget()
{
    makeCurrent();
    TextureManager::instance().clear();
    doneCurrent();
}

void OpenGLWidget::updateFPS()
{
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    if (lastTime == 0) {
        lastTime = currentTime;
        return;
    }

    int fps = frameCount * 1000 / (currentTime - lastTime);
    frameCount = 0;
    lastTime = currentTime;

    emit fpsUpdated(fps);
}

void OpenGLWidget::initializeGL()
{
    light = new Light(this);
    connect(light, &Light::lightChanged, this, QOverload<>::of(&OpenGLWidget::update));

    initializeOpenGLFunctions();
    glClearColor(0.3f, 0.2f, 0.3f, 1.0f);
    glEnable(GL_DEPTH_TEST);


    // Загружаем обычные шейдеры
    if (!ShaderManager::instance().loadShaderProgram("default",
                                                     ":/shaders/vshader.vsh",
                                                     ":/shaders/fshader.fsh")) {
        qCritical() << "Failed to load default shaders!";
        return;
    }

    // Загружаем шейдеры для скайбокса
    if (!ShaderManager::instance().loadShaderProgram("skybox",
                                                     ":/shaders/vskybox.vsh",
                                                     ":/shaders/fskybox.fsh")) {
        qCritical() << "Failed to load skybox shaders!";
        return;
    }

    program = ShaderManager::instance().getShaderProgram("default");
    skyBoxProgram = ShaderManager::instance().getShaderProgram("skybox");


    TextureManager::instance().loadTexture(":/textures/magma.png", "magma");
    TextureManager::instance().loadTexture(":/textures/wood.png", "wood");
    TextureManager::instance().loadTexture(":/textures/cubes_gray.png", "cubes");
    TextureManager::instance().loadTexture(":/textures/water.png", "water");

    Skybox *skybox = new Skybox();
    skybox->initialize();
    skybox->setScale(QVector3D(1000, 1000, 1000));
    skybox->setRotation(180, QVector3D(0, 1, 0));
    scene.setSkybox(skybox);

    Cube *cube = new Cube("magma");
    cube->initialize();
    cube->setScale(QVector3D(1.5f, 1.5f, 1.5f));
    scene.addShape(cube);

    Pyramid *pyramid = new Pyramid("wood");
    pyramid->initialize();
    pyramid->setPosition(QVector3D(0.0f, 1.35f, 0.0f));
    pyramid->setScale(QVector3D(1.5f, 1.2f, 1.5f));
    scene.addShape(pyramid);

    Sphere *sphere = new Sphere("cubes", 0.5f);
    sphere->initialize();
    sphere->setPosition(QVector3D(-1.5f, 0.0f, 0.0f));
    sphere->setRotation(90, QVector3D(0, 1, 0));
    scene.addShape(sphere);

    Cylinder *cylinder = new Cylinder("water", 0.5f, 1.0f);
    cylinder->initialize();
    cylinder->setPosition(QVector3D(1.5f, 0.0f, 0.0f));
    cylinder->setRotation(90, QVector3D(0, 1, 0));
    scene.addShape(cylinder);

    cameraController->updateCamera();
}

void OpenGLWidget::initShaders() {


}

void OpenGLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    scene.resize(w, h);
}

void OpenGLWidget::paintGL()
{
    frameCount++;

    deltaTime = frameTimer.restart() / 1000.0f;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDepthFunc(GL_LEQUAL);
    if (skyBoxProgram) {
        skyBoxProgram->bind();
        scene.renderSkybox(*skyBoxProgram);
        skyBoxProgram->release();

    }
    glDepthFunc(GL_LESS);

    if (program) {
        program->bind();
        program->setUniformValue("lightPos", light->position());
        program->setUniformValue("viewPos", scene.getCameraPosition());
        program->setUniformValue("lightColor", light->color());
        program->setUniformValue("ambientStrength", light->ambientStrength());
        program->setUniformValue("specularStrength", light->specularStrength());
        program->setUniformValue("shininess", light->shininess());

        program->setUniformValue("objectColor", QVector3D(1.0f, 1.0f, 1.0f));

        scene.renderAll(*program);
        program->release();
    }

    QTimer::singleShot(0, this, QOverload<>::of(&OpenGLWidget::update));
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
