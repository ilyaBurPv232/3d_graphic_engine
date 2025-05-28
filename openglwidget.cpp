#include "openglwidget.h"
#include "shadermanager.h"
#include "texturemanager.h"
#include "physicalprimitives.h"
#include "primitives.h"
#include "skybox.h"
#include "collisiondetector.h"
#include "physicalworld.h"
#include "physicalobject.h"
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
    fpsTimer->start(100); // Обновление раз в секунду

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

    emit fpsUpdated(fps); // Нужно добавить сигнал в заголовочный файл
}

void OpenGLWidget::initializeGL()
{
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

    TextureManager::instance().loadTexture(":/textures/magma.png", "magma");
    TextureManager::instance().loadTexture(":/textures/wood.png", "wood");
    TextureManager::instance().loadTexture(":/textures/cubes_gray.png", "cubes");
    TextureManager::instance().loadTexture(":/textures/water.png", "water");
    TextureManager::instance().loadTexture(":/textures/parcet.png", "parcet");

    Skybox *skybox = new Skybox();
    skybox->initialize();
    skybox->setScale(QVector3D(500, 500, 500));
    scene.addShape(skybox);

    PhysicalWorld& world = PhysicalWorld::instance();

    scene.addShape(world.getGroundPlane()->getShape());

    PhycSphere* physCube = new PhycSphere("cubes", 1.0, 1.0);
    physCube->setPosition(QVector3D(9, 1, 0));
    physCube->setVelocity(QVector3D(-1.0f, 0, 0));
    world.addObject(physCube);
    scene.addShape(physCube->getShape());

    PhycCube* physCube1 = new PhycCube("magma", 1.0, 0.5);
    physCube1->setPosition(QVector3D(9, 0, 0));
    physCube1->setStatic(true);
    physCube1->setVelocity(QVector3D(0, 0, 0));
    world.addObject(physCube1);
    scene.addShape(physCube1->getShape());

    PhycPyramid* physPyramid = new PhycPyramid("wood", 1.0, 1.0);
    physPyramid->setPosition(QVector3D(0, 0, 0));
    //physPyramid->setStatic(true);
    physPyramid->setVelocity(QVector3D(3.0f, 0, 0));
    world.addObject(physPyramid);
    scene.addShape(physPyramid->getShape());

    PhycCylinder* phycCyl = new PhycCylinder("water", 1.0, 1.0);
    phycCyl->setPosition(QVector3D(11.0f, 10.0f, 0));
    world.addObject(phycCyl);
    scene.addShape(phycCyl->getShape());

    cameraController->updateCamera();
}

void OpenGLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    scene.resize(w, h);
}

void OpenGLWidget::paintGL()
{
    frameCount++;
    physical_counter ++;
    deltaTime = frameTimer.restart() / 1000.0f;

    // PhysicalWorld& world = PhysicalWorld::instance();
    // QVector3D cybeV = world.getObjects()[0]->getVelocity();
    // world.getObjects()[0]->update(deltaTime);
    // CollisionDetector dect;
    // bool isFlag = false;
    // auto collions = dect.detectCollisions(world.getObjects(), isFlag);
    // if (isFlag == true) {
    //     world.getObjects()[0]->setVelocity(-cybeV);
    // }
    // world.getObjects()[2]->setAcceleration(world.getGravity());
    // world.getObjects()[2]->update(1 / 144.0f);

    // QVector<PhysicalObject*> test;
    // test.append(world.getObjects()[2]);
    // test.append(world.getGroundPlane());
    // qDebug() << world.getObjects()[2]->getVelocity();

    // bool Flag = false;
    // auto col = dect.detectCollisions(test, Flag);
    // if (Flag == true) {
    //     world.getObjects()[2]->setStatic(true);
    // }

    PhysicalWorld& world = PhysicalWorld::instance();
    world.updateObjects(1 / 144.0f);


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDepthMask(GL_FALSE); // Отключаем запись глубины
    QOpenGLShaderProgram* skyboxProgram = ShaderManager::instance().getShaderProgram("skybox");
    if (skyboxProgram) {
        skyboxProgram->bind();
        scene.renderAll(*skyboxProgram);
        skyboxProgram->release();
    }
    glDepthMask(GL_TRUE); // Включаем запись глубины обратно

    // Рендерим обычные объекты
    if (program) {
        program->bind();
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
