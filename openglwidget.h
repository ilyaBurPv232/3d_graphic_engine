#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLTexture>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QVector3D>
#include <QKeyEvent>
#include <QTime>

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    OpenGLWidget(QWidget* parent = nullptr);
    ~OpenGLWidget();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void keyPressEvent(QKeyEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    QOpenGLShaderProgram program;
    QOpenGLBuffer vertexBuffer;
    QMatrix4x4 projection;
    QMatrix4x4 view;
    QMatrix4x4 model;

    QVector3D cameraPos;
    QVector3D cameraFront;
    QVector3D cameraUp;

    QOpenGLTexture* texture;

    float yaw = -90.0f;
    float pitch = 0.0f;
    float lastX = 400;
    float lastY = 300;
    bool firstMouse = true;

    QPoint lastMousePosition;
    bool mousePressed = false;
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
};

#endif // OPENGLWIDGET_H
