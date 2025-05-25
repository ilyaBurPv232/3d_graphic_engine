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

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit OpenGLWidget(QWidget* parent = nullptr);
    ~OpenGLWidget();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

    void updateFPS();

private:
    QOpenGLShaderProgram *program;
    Scene scene;
    CameraController* cameraController;

    QTimer *fpsTimer;
    int frameCount;
    qint64 lastTime;

    QElapsedTimer frameTimer;
    float deltaTime = 0.0f;

signals:
    void fpsUpdated(int fps);

};

#endif // OPENGLWIDGET_H
