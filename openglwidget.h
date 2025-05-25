#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QDateTime>
#include <QMouseEvent>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include <QTimer>
#include "cameracontroller.h"
#include "scene.h"

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit OpenGLWidget(QWidget *parent = nullptr);
    ~OpenGLWidget();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

    void updateFPS();

private:
    QOpenGLShaderProgram *program;
    Scene scene;
    CameraController *cameraController;

    QTimer *fpsTimer;
    int frameCount;
    qint64 lastTime;

signals:
    void fpsUpdated(int fps);
};

#endif // OPENGLWIDGET_H
