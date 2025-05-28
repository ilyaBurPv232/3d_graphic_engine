#include "physicalobject.h"
#include "primitives.h"
#include <QDebug>

PhysicalObject::PhysicalObject(Shape* shape, double mass, double elasticity)
    : shape(shape), mass(mass), elasticity(elasticity), isStaticObject(false)
{
    velocity = QVector3D(0, 0, 0);
    acceleration = QVector3D(0, 0, 0);
    forceAccumulator = QVector3D(0, 0, 0);
    inverseMass = mass > 0 ? 1.0 / mass : 0;
}

PhysicalObject::~PhysicalObject() {

}

// Геттеры
QVector3D PhysicalObject::getVelocity() const {
    return velocity;
}

QVector3D PhysicalObject::getAcceleration() const {
    return acceleration;
}

double PhysicalObject::getMass() const {
    return mass;
}

double PhysicalObject::getElasticity() const {
    return elasticity;
}

QVector3D PhysicalObject::getPosition() const {
    return shape ? shape->getPosition() : QVector3D();
}

Shape* PhysicalObject::getShape() const {
    return shape;
}

double PhysicalObject::getInverseMass() const {
    return inverseMass;
}

bool PhysicalObject::isStatic() const {
    return isStaticObject;
}



// Сеттеры
void PhysicalObject::setVelocity(const QVector3D& newVelocity) {
    velocity = newVelocity;
}

void PhysicalObject::setAcceleration(const QVector3D& newAcceleration) {
    acceleration = newAcceleration;
}

void PhysicalObject::setMass(double newMass) {
    mass = newMass;
    inverseMass = mass > 0 ? 1.0 / mass : 0;
}

void PhysicalObject::setElasticity(double newElasticity) {
    elasticity = qBound(0.0, newElasticity, 1.0);
}

void PhysicalObject::setStatic(bool staticFlag) {
    isStaticObject = staticFlag;
    if (isStaticObject) {
        velocity = QVector3D(0, 0, 0);
        acceleration = QVector3D(0, 0, 0);
    }
}

void PhysicalObject::setPosition(const QVector3D& position) {
    this->shape->setPosition(position);
}

// Применение силы (добавляется в аккумулятор)
void PhysicalObject::applyForce(const QVector3D& force) {
    if (!isStaticObject) {
        forceAccumulator += force;
    }
}

// Применение импульса (непосредственно изменяет скорость)
void PhysicalObject::applyImpulse(const QVector3D& impulse) {
    if (!isStaticObject) {
        velocity += impulse * inverseMass;
    }
}

void PhysicalObject::update(float deltaTime) {
    if (isStaticObject || !shape) return;

    // Обновление линейной скорости и позиции
    if (forceAccumulator != QVector3D(0, 0, 0)) {
        acceleration = forceAccumulator * inverseMass;
    }
    this->velocity += acceleration * deltaTime;
    QVector3D newPosition = shape->getPosition() + velocity * deltaTime;
    setPosition(newPosition);

    // Качение сферы
    if (auto sphere = dynamic_cast<Sphere*>(shape)) {
        float radius = sphere->getRadius() * shape->getScale().x(); // Радиус с учетом масштаба
        if (!qFuzzyIsNull(radius) && !velocity.isNull()) {
            // Направление оси вращения: перпендикулярно направлению движения и "вверх" (Y)
            QVector3D rotationAxis = QVector3D::crossProduct(QVector3D(0, 1, 0), velocity.normalized()).normalized();

            // Угловая скорость (в радианах/сек): v = ω*r => ω = v/r
            float angularSpeed = velocity.length() / radius;

            // Применяем вращение (перевод из радиан в градусы)
            shape->setRotation(angularSpeed * deltaTime * 180.0f / M_PI, rotationAxis);
        }
    }

    // Качение цилиндра (лежащего на боку вдоль оси Z)
    if (auto cylinder = dynamic_cast<Cylinder*>(shape)) {
        float radius = cylinder->getRadius() * shape->getScale().y(); // Радиус с учетом масштаба
        if (!qFuzzyIsNull(radius) && !velocity.isNull()) {
            // Угловая скорость (в радианах/сек): v = ω*r => ω = v/r
            float angularSpeed = velocity.x() / radius;

            // Вращение вокруг оси Z (предполагаем, что цилиндр лежит на боку)
            shape->setRotation(angularSpeed * deltaTime * 180.0f / M_PI, QVector3D(0, 0, 1));
        }
    }

    forceAccumulator = QVector3D(0, 0, 0);
}

QVector<QVector3D> PhysicalObject::getTransformedVertices() const {
    QVector<QVector3D> transformed;
    if (!shape || shape->getVertices().empty()) { // Добавляем проверки
        qWarning() << "Shape or vertices are empty!";
        return transformed;
    }

    for (const auto &vertex : shape->getVertices()) {
        transformed.append(shape->getModelMatrix().map(vertex.position));
    }
    return transformed;
}

QVector3D PhysicalObject::getCenter() const {
    return shape ? shape->getPosition() : QVector3D();
}

float PhysicalObject::getBoundingSphereRadius() const {
    if (!shape) return 0.0f;
    QVector3D scale = shape->getScale();
    return 0.5f * qMax(scale.x(), qMax(scale.y(), scale.z()));
}
