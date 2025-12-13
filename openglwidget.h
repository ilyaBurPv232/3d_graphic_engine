#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QMouseEvent>
#include <QTimer>
#include <QDateTime>
#include "scene.h"
#include "cameracontroller.h"
#include "light.h"
#include "fpscounter.h"
#include "postprocessor.h"
#include "primitives.h"

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit OpenGLWidget(QWidget* parent = nullptr);
    ~OpenGLWidget();
    PostProcessor* getPostProcessor() const { return postProcessor; }

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;


private:
    QOpenGLShaderProgram *program;
    QOpenGLShaderProgram *skyBoxProgram;
    Scene scene;
    CameraController* cameraController;
    float animCounter;
    QElapsedTimer frameTimer;
    float deltaTime = 0.0f;
    Light* light;
    FPSCounter* fpsCounter;
    PostProcessor* postProcessor;
    QOpenGLShaderProgram* postProcessingProgram;

    bool moveForward = false;
    bool moveBackward = false;
    bool moveLeft = false;
    bool moveRight = false;
    bool moveUp = false;
    bool moveDown = false;
    float movementSpeed = 5.0f; // Скорость движения

    // Добавляем сферу как источник света
    Sphere* lightSphere;
    float lightSphereRotation = 0.0f;
    bool lightSphereVisible = true;

signals:
    void frameRendered();
    void fpsUpdated(int fps);
};

#endif // OPENGLWIDGET_H
