#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include <QObject>
#include <QVector3D>
#include <QMatrix4x4>
#include <QPoint>
#include <QMouseEvent>

class Scene;

class CameraController : public QObject
{
    Q_OBJECT

public:
    explicit CameraController(Scene* scene, QObject* parent = nullptr);

    void setDistance(float distance) { m_distance = distance; updateCamera(); }
    float distance() const { return m_distance; }

    void setRotationSpeed(float speed) { m_rotationSpeed = speed; }
    float rotationSpeed() const { return m_rotationSpeed; }

    void setPanSpeed(float speed) { m_panSpeed = speed; }
    float panSpeed() const { return m_panSpeed; }

    void setZoomSpeed(float speed) { m_zoomSpeed = speed; }
    float zoomSpeed() const { return m_zoomSpeed; }

    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent* event);

    void updateCamera();
    void resize(int width, int height);

signals:
    void cameraUpdated();

private:
    Scene* m_scene;

    float m_distance = 5.0f;
    QVector2D m_rotationAngles = QVector2D(30.0f, 0.0f); // X (pitch), Y (yaw)
    QVector3D m_center = QVector3D(0, 0, 0);

    float m_rotationSpeed = 0.5f;
    float m_panSpeed = 0.005f;
    float m_zoomSpeed = 0.001f;

    QPoint m_lastMousePos;
    bool m_isRotating = false;
    bool m_isPanning = false;

    int m_width = 1;
    int m_height = 1;
};

#endif // CAMERACONTROLLER_H
