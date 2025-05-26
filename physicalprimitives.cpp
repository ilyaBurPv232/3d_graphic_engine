#include "physicalprimitives.h"
#include "primitives.h"

// PhycCube
PhycCube::PhycCube(const QString& textureName, double mass, double elasticity)
    : PhysicalObject(new Cube(textureName), mass, elasticity) {
    static_cast<Cube*>(shape)->initialize();
}

void PhycCube::getAABB(QVector3D& min, QVector3D& max) const {
    QVector3D localMin(-0.5f, -0.5f, -0.5f);
    QVector3D localMax(0.5f, 0.5f, 0.5f);

    QVector3D corners[8] = {
        QVector3D(localMin.x(), localMin.y(), localMin.z()),
        QVector3D(localMax.x(), localMin.y(), localMin.z()),
        QVector3D(localMin.x(), localMax.y(), localMin.z()),
        QVector3D(localMax.x(), localMax.y(), localMin.z()),
        QVector3D(localMin.x(), localMin.y(), localMax.z()),
        QVector3D(localMax.x(), localMin.y(), localMax.z()),
        QVector3D(localMin.x(), localMax.y(), localMax.z()),
        QVector3D(localMax.x(), localMax.y(), localMax.z())
    };

    min = QVector3D(INFINITY, INFINITY, INFINITY);
    max = QVector3D(-INFINITY, -INFINITY, -INFINITY);

    for (const auto& corner : corners) {
        QVector3D transformed = shape->getModelMatrix().map(corner);
        min.setX(qMin(min.x(), transformed.x()));
        min.setY(qMin(min.y(), transformed.y()));
        min.setZ(qMin(min.z(), transformed.z()));
        max.setX(qMax(max.x(), transformed.x()));
        max.setY(qMax(max.y(), transformed.y()));
        max.setZ(qMax(max.z(), transformed.z()));
    }
}

PhycPyramid::PhycPyramid(const QString& textureName, double mass, double elasticity)
    : PhysicalObject(new Pyramid(textureName), mass, elasticity) {
    static_cast<Pyramid*>(shape)->initialize();
}

void PhycPyramid::getAABB(QVector3D& min, QVector3D& max) const {
    QVector3D base[4] = {
          QVector3D(-0.5f, -0.5f, -0.5f),
          QVector3D(0.5f, -0.5f, -0.5f),
          QVector3D(0.5f, -0.5f, 0.5f),
          QVector3D(-0.5f, -0.5f, 0.5f)
      };
    QVector3D top(0.0f, 0.5f, 0.0f);

    min = QVector3D(INFINITY, INFINITY, INFINITY);
    max = QVector3D(-INFINITY, -INFINITY, -INFINITY);

    for (const auto& v : base) {
        QVector3D transformed = shape->getModelMatrix().map(v);
        min.setX(qMin(min.x(), transformed.x()));
        min.setY(qMin(min.y(), transformed.y()));
        min.setZ(qMin(min.z(), transformed.z()));
        max.setX(qMax(max.x(), transformed.x()));
        max.setY(qMax(max.y(), transformed.y()));
        max.setZ(qMax(max.z(), transformed.z()));
    }

    QVector3D transformedTop = shape->getModelMatrix().map(top);
    min.setX(qMin(min.x(), transformedTop.x()));
    min.setY(qMin(min.y(), transformedTop.y()));
    min.setZ(qMin(min.z(), transformedTop.z()));
    max.setX(qMax(max.x(), transformedTop.x()));
    max.setY(qMax(max.y(), transformedTop.y()));
    max.setZ(qMax(max.z(), transformedTop.z()));
}

PhycSphere::PhycSphere(const QString& textureName, double mass, double elasticity)
    : PhysicalObject(new Sphere(textureName), mass, elasticity) {
    static_cast<Sphere*>(shape)->initialize();
}

void PhycSphere::getAABB(QVector3D& min, QVector3D& max) const {
    Sphere* sphere = dynamic_cast<Sphere*>(shape);
    if (!sphere) {
        qWarning() << "Ошибка: объект не является сферой!";
        return;
    }

    QVector3D center = sphere->getPosition();
    float scaledRadius = sphere->getRadius() * (qMax(sphere->getScale().x(), qMax(sphere->getScale().y(), sphere->getScale().z())));

    min = center - QVector3D(scaledRadius, scaledRadius, scaledRadius);
    max = center + QVector3D(scaledRadius, scaledRadius, scaledRadius);
}

PhycCylinder::PhycCylinder(const QString& textureName, double mass, double elasticity)
    : PhysicalObject(new Sphere(textureName), mass, elasticity) {
    static_cast<Cylinder*>(shape)->initialize();
}

void PhycCylinder::getAABB(QVector3D& min, QVector3D& max) const {
    Cylinder* cylinder = dynamic_cast<Cylinder*>(shape);
    if (!cylinder) {
        qWarning() << "Ошибка: объект не является сферой!";
        return;
    }
    // Получаем исходные параметры
    float baseRadius = cylinder->getRadius();
    float baseHeight = cylinder->getHeight();
    QVector3D scale = cylinder->getScale();

    // Масштабированные параметры
    float scaledRadius = baseRadius * qMax(scale.x(), scale.z()); // Радиус масштабируется по X и Z
    float scaledHeight = baseHeight * scale.y(); // Высота масштабируется по Y

    // Центр цилиндра
    QVector3D center = cylinder->getPosition();

    // Вычисляем AABB
    min = center - QVector3D(scaledRadius, scaledHeight / 2, scaledRadius);
    max = center + QVector3D(scaledRadius, scaledHeight / 2, scaledRadius);
}
