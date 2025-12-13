#include "openglwidget.h"
#include "shadermanager.h"
#include "texturemanager.h"
#include "primitives.h"
#include "skybox.h"
#include "customfigure.h"
#include <QDebug>
#include <QMouseEvent>
#include <QWheelEvent>


OpenGLWidget::OpenGLWidget(QWidget* parent)
    : QOpenGLWidget(parent),
    moveForward(false), moveBackward(false),
    moveLeft(false), moveRight(false),
    moveUp(false), moveDown(false),
    movementSpeed(3.0f)
{
    setFocusPolicy(Qt::StrongFocus);
    cameraController = new CameraController(&scene, this);
    fpsCounter = new FPSCounter(this);
    connect(this, &OpenGLWidget::frameRendered, fpsCounter, &FPSCounter::frameRendered);
    connect(fpsCounter, &FPSCounter::fpsUpdated, this, &OpenGLWidget::fpsUpdated);
    connect(cameraController, &CameraController::cameraUpdated, this, QOverload<>::of(&OpenGLWidget::update));
    QTimer::singleShot(0, this, QOverload<>::of(&OpenGLWidget::update));
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
    glClearColor(0.3f, 0.2f, 0.3f, 1.0f);
    glEnable(GL_DEPTH_TEST);


    if (!ShaderManager::instance().loadShaderProgram("default",
                                                     ":/shaders/vshader.vsh",
                                                     ":/shaders/fshader.fsh")) {
        qCritical() << "Failed to load default shaders!";
        return;
    }

    if (!ShaderManager::instance().loadShaderProgram("skybox",
                                                     ":/shaders/vskybox.vsh",
                                                     ":/shaders/fskybox.fsh")) {
        qCritical() << "Failed to load skybox shaders!";
        return;
    }

    program = ShaderManager::instance().getShaderProgram("default");
    skyBoxProgram = ShaderManager::instance().getShaderProgram("skybox");

    postProcessor = new PostProcessor();
    postProcessor->initialize(width(), height());

    if (!ShaderManager::instance().loadShaderProgram("postprocessing",
                                                     ":/shaders/postprocessing.vsh",
                                                     ":/shaders/postprocessing.fsh")) {
        qCritical() << "Failed to load postprocessing shaders!";
    }
    postProcessingProgram = ShaderManager::instance().getShaderProgram("postprocessing");


    TextureManager::instance().loadTexture(":/textures/magma.png", "magma");
    TextureManager::instance().loadTexture(":/textures/wood.png", "wood");
    TextureManager::instance().loadTexture(":/textures/cubes_gray.png", "cubes");
    TextureManager::instance().loadTexture(":/textures/water.png", "water");
    TextureManager::instance().loadTexture(":/textures/grass.png", "grass");
    TextureManager::instance().loadTexture(":/textures/gorbino_face.png", "face");

    Skybox *skybox = new Skybox();
    skybox->initialize();
    skybox->setScale(QVector3D(500, 500, 500));
    skybox->setRotation(180, QVector3D(0, 1, 0));
    scene.setSkybox(skybox);


    lightSphere = new Sphere("face", 0.3f);
    lightSphere->initialize();
    lightSphere->setPosition(QVector3D(0.0f, 5.0f, 0.0f));
    lightSphere->setScale(QVector3D(0.5f, 0.5f, 0.5f));
    scene.addShape(lightSphere);

    Cube *cube = new Cube("magma");
    cube->initialize();
    cube->setScale(QVector3D(1.5f, 1.5f, 1.5f));
    scene.addShape(cube);

    Pyramid *pyramid = new Pyramid("wood");
    pyramid->initialize();
    pyramid->setPosition(QVector3D(0.0f, 1.35f, 0.0f));
    pyramid->setScale(QVector3D(1.5f, 1.2f, 1.5f));
    scene.addShape(pyramid);

    Sphere *sphere = new Sphere("face", 0.5f);
    sphere->initialize();
    sphere->setRotation(90, QVector3D(0, 1, 0));
    scene.addShape(sphere);

    Cylinder *cylinder = new Cylinder("water", 0.45f, 1.0f);
    cylinder->initialize();
    cylinder->setPosition(QVector3D(1.5f, 0.0f, 0.0f));
    cylinder->setRotation(90, QVector3D(0, 1, 0));
    scene.addShape(cylinder);

    Cube *ground = new Cube("grass");
    ground->setScale(QVector3D(200.0f, 0.1f, 200.0f));
    ground->setPosition(QVector3D(0, -0.8f, 0));
    scene.addShape(ground);

    CustomFigure* customFigure = new CustomFigure("magma", 8, 6, 6); // 8-угольник сверху, 6-угольник снизу, 6-угольник в центре
    customFigure->initialize();
    customFigure->setPosition(QVector3D(5.0f, 0.7f, 0.0f));
    customFigure->setScale(QVector3D(1.0f, 1.0f, 1.0f));
    customFigure->setRotation(180.0f, QVector3D(0,0,1));
    scene.addShape(customFigure);

    // Для изменения количества сторон можно использовать:
    customFigure->setCenterSides(10); // Изменить центральную фигуру на 10-угольник


    light = new Light(this);
    connect(light, &Light::lightChanged, this, QOverload<>::of(&OpenGLWidget::update));
    light->setColor(QVector3D(0.5f, 0.5f, 0.5f));
    light->setAmbientStrength(0.5);
    light->setPosition(QVector3D(-100.0f, 350.0f, 450.0f));
    light->setDirection(QVector3D(-0.2f, -1.0f, -0.3f));
    light->setLightType(0);

    cameraController->updateCamera();
}

void OpenGLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    scene.resize(w, h);
    cameraController->resize(w, h);
    if (postProcessor) {
        postProcessor->resize(w, h);
    }
}

void OpenGLWidget::paintGL()
{
    emit frameRendered();
    postProcessor->beginRender();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    animCounter += 0.01f;

    QVector3D lightSpherePos = lightSphere->getPosition();
    float moveStep = movementSpeed * deltaTime;

    if (moveForward) lightSpherePos.setZ(lightSpherePos.z() - moveStep);
    if (moveBackward) lightSpherePos.setZ(lightSpherePos.z() + moveStep);
    if (moveLeft) lightSpherePos.setX(lightSpherePos.x() - moveStep);
    if (moveRight) lightSpherePos.setX(lightSpherePos.x() + moveStep);
    if (moveUp) lightSpherePos.setY(lightSpherePos.y() + moveStep);
    if (moveDown) lightSpherePos.setY(lightSpherePos.y() - moveStep);

    lightSphere->setPosition(lightSpherePos);

    light->setPosition(lightSphere->getPosition());

    QVector3D toCenter = QVector3D(0, 0, 0) - lightSphere->getPosition();
    if (toCenter.length() > 0) {
        light->setDirection(toCenter.normalized());
    }

    scene.getShapes()[3]->setPosition(QVector3D(sin(animCounter) * 3, 0, cos(animCounter) * 3));
    scene.getShapes()[3]->setRotation(1, QVector3D(1, 0, 1));
    scene.getShapes()[4]->setPosition(QVector3D(sin(animCounter) * 1.9, 0, cos(animCounter) * 2));
    scene.getShapes()[4]->setRotation(-1, QVector3D(0, 1, 0));

    deltaTime = frameTimer.restart() / 1000.0f;

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
        program->setUniformValue("lightDirection", light->direction());
        program->setUniformValue("ambientStrength", light->ambientStrength());
        program->setUniformValue("specularStrength", light->specularStrength());
        program->setUniformValue("shininess", light->shininess());
        program->setUniformValue("lightType", light->lightType());
        program->setUniformValue("cutOff", light->cutOff());
        program->setUniformValue("outerCutOff", light->outerCutOff());
        program->setUniformValue("lightSphereEnabled", light->lightSphereEnabled());
        program->setUniformValue("objectColor", QVector3D(1.0f, 1.0f, 1.0f));
        scene.renderAll(*program);
        program->release();
    }

    postProcessor->endRender();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (postProcessingProgram) {
        postProcessingProgram->bind();
        postProcessor->applyEffects(*postProcessingProgram);
        postProcessingProgram->release();
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

void OpenGLWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_W:
        moveForward = true;
        break;
    case Qt::Key_S:
        moveBackward = true;
        break;
    case Qt::Key_A:
        moveLeft = true;
        break;
    case Qt::Key_D:
        moveRight = true;
        break;
    case Qt::Key_Q:
        moveUp = true;
        break;
    case Qt::Key_E:
        moveDown = true;
        break;
    case Qt::Key_R:
        lightSphere->setPosition(QVector3D(0.0f, 5.0f, 0.0f));
        break;
    case Qt::Key_Plus:
    case Qt::Key_Equal:
        movementSpeed += 0.5f;
        qDebug() << "Movement speed:" << movementSpeed;
        break;
    case Qt::Key_Minus:
        movementSpeed = qMax(0.5f, movementSpeed - 0.5f);
        qDebug() << "Movement speed:" << movementSpeed;
        break;
    }

    if (event->key() == Qt::Key_F3) {
        fpsCounter->toggleVisibility();
    }
    else if (event->key() == Qt::Key_F2) {
        bool enabled = !postProcessor->areEffectsEnabled();
        postProcessor->toggleEffects(enabled);
        update();
    }
    else if (event->key() == Qt::Key_1) {
        light->setLightType(0);
        qDebug() << "Switched to Point Light";
        update();
    }
    else if (event->key() == Qt::Key_2) {
        light->setLightType(1);
        qDebug() << "Switched to Directional Light";
        update();
    }
    else if (event->key() == Qt::Key_3) {
        light->setLightType(2);
        qDebug() << "Switched to Spot Light";
        update();
    }
    else if (event->key() == Qt::Key_L) {
        bool enabled = !light->lightSphereEnabled();
        light->setLightSphereEnabled(enabled);
        lightSphereVisible = enabled;
        qDebug() << "Light sphere" << (enabled ? "enabled" : "disabled");
        update();
    }

    QOpenGLWidget::keyPressEvent(event);
}

void OpenGLWidget::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_W:
        moveForward = false;
        break;
    case Qt::Key_S:
        moveBackward = false;
        break;
    case Qt::Key_A:
        moveLeft = false;
        break;
    case Qt::Key_D:
        moveRight = false;
        break;
    case Qt::Key_Q:
        moveUp = false;
        break;
    case Qt::Key_E:
        moveDown = false;
        break;
    }

    QOpenGLWidget::keyReleaseEvent(event);
}
