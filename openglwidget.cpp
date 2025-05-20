#include "openglwidget.h"
#include "cube.h"
#include <QDebug>
#include <QMouseEvent>

OpenGLWidget::OpenGLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);
}

OpenGLWidget::~OpenGLWidget()
{
}

void OpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(1.0f, 1.0f, 0.3f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vshader.vsh");
    program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fshader.fsh");
    program.link();

    // Предварительная загрузка текстур
    TextureManager::instance().loadTexture(":/textures/grass.png", "grass");

    // Создание куба с указанием текстуры
    Cube* cube = new Cube("grass");
    cube->initialize();
    scene.addShape(cube);
}

void OpenGLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    scene.resize(w, h);
}

void OpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    program.bind();
    scene.renderAll(program);
    program.release();
}

void OpenGLWidget::keyPressEvent(QKeyEvent *event)
{
    float cameraSpeed = 0.1f;
    QVector3D cameraPos = scene.getCameraPosition();
    QVector3D cameraFront = scene.getCameraFront();
    QVector3D cameraUp = scene.getCameraUp();

    if (event->key() == Qt::Key_W)
        cameraPos += cameraSpeed * cameraFront;
    if (event->key() == Qt::Key_S)
        cameraPos -= cameraSpeed * cameraFront;
    if (event->key() == Qt::Key_A)
        cameraPos -= QVector3D::crossProduct(cameraFront, cameraUp).normalized() * cameraSpeed;
    if (event->key() == Qt::Key_D)
        cameraPos += QVector3D::crossProduct(cameraFront, cameraUp).normalized() * cameraSpeed;

    scene.setCameraPosition(cameraPos);
    update();
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    float xpos = event->pos().x();
    float ypos = event->pos().y();

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    QVector3D front;
    front.setX(cos(qDegreesToRadians(yaw)) * cos(qDegreesToRadians(pitch)));
    front.setY(sin(qDegreesToRadians(pitch)));
    front.setZ(sin(qDegreesToRadians(yaw)) * cos(qDegreesToRadians(pitch)));

    scene.setCameraFront(front.normalized());
    update();
}
