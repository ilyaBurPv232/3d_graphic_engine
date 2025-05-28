#include "cameracontroller.h"
#include "scene.h"
#include <QWheelEvent>
#include <QtMath>

CameraController::CameraController(Scene* scene, QObject* parent)
    : QObject(parent), m_scene(scene)
{
}

void CameraController::mousePressEvent(QMouseEvent* event)
{
    m_lastMousePos = event->pos();

    if (event->button() == Qt::LeftButton) {
        m_isRotating = true;
    } else if (event->button() == Qt::RightButton) {
        m_isPanning = true;
    }
}

void CameraController::mouseMoveEvent(QMouseEvent* event)
{
    QPoint delta = event->pos() - m_lastMousePos;
    m_lastMousePos = event->pos();

    if (m_isRotating) {
        m_rotationAngles.setX(m_rotationAngles.x() + delta.y() * m_rotationSpeed);
        m_rotationAngles.setY(m_rotationAngles.y() - delta.x() * m_rotationSpeed);


        m_rotationAngles.setX(fmod(m_rotationAngles.x(), 360.0f));
        m_rotationAngles.setY(fmod(m_rotationAngles.y(), 360.0f));

        updateCamera();
    }
    else if (m_isPanning) {
        float aspect = float(m_width) / float(m_height);
        float tanFov = tan(qDegreesToRadians(45.0f) / 2.0f);

        QVector3D viewDir = QVector3D(
                                qSin(qDegreesToRadians(m_rotationAngles.y())) * qCos(qDegreesToRadians(m_rotationAngles.x())),
                                qSin(qDegreesToRadians(m_rotationAngles.x())),
                                qCos(qDegreesToRadians(m_rotationAngles.y())) * qCos(qDegreesToRadians(m_rotationAngles.x()))
                                ).normalized();

        QVector3D right = QVector3D::crossProduct(viewDir, QVector3D(0, -1, 0)).normalized();
        QVector3D up = QVector3D::crossProduct(right, viewDir).normalized();

        m_center -= right * (delta.x() * m_panSpeed * m_distance * aspect * tanFov);
        m_center -= up * (delta.y() * m_panSpeed * m_distance * tanFov);

        updateCamera();
    }
}

void CameraController::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        m_isRotating = false;
    } else if (event->button() == Qt::RightButton) {
        m_isPanning = false;
    }
}

void CameraController::wheelEvent(QWheelEvent* event)
{
    float delta = event->angleDelta().y();
    m_distance *= (1.0f - delta * m_zoomSpeed);
    m_distance = qBound(0.5f, m_distance, 100.0f);

    updateCamera();
}

void CameraController::updateCamera()
{
    float cosX = qCos(qDegreesToRadians(m_rotationAngles.x()));
    float sinX = qSin(qDegreesToRadians(m_rotationAngles.x()));
    float cosY = qCos(qDegreesToRadians(m_rotationAngles.y()));
    float sinY = qSin(qDegreesToRadians(m_rotationAngles.y()));

    QVector3D position(
        m_distance * sinY * cosX,
        m_distance * sinX,
        m_distance * cosY * cosX
        );

    position += m_center;

    QVector3D up = QVector3D(
                       -sinY * sinX,
                       cosX,
                       -cosY * sinX
                       ).normalized();

    m_scene->setCameraPosition(position);
    m_scene->setCameraFront((m_center - position).normalized());
    m_scene->setCameraUp(up);

    emit cameraUpdated();
}

void CameraController::resize(int width, int height)
{
    m_width = width;
    m_height = height;
}
