#include "physicalobject.h"
#include <QDebug>

PhysicalObject::PhysicalObject(Shape* shape, double mass, double elasticity)
    : shape(shape), mass(mass), elasticity(elasticity), isStaticObject(false)
{
    velocity = QVector3D(0, 0, 0);
    acceleration = QVector3D(0, 0, 0);
    forceAccumulator = QVector3D(0, 0, 0);
    inverseMass = mass > 0 ? 1.0 / mass : 0;
}

PhysicalObject::~PhysicalObject()
{
    // shape удаляется где-то в другом месте
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

// Основной метод обновления физики
void PhysicalObject::update(float deltaTime) {
    if (isStaticObject || !shape) return;

    // Интегрирование движения (прямолинейное)
    acceleration = forceAccumulator * inverseMass;
    velocity += acceleration * deltaTime;

    // Применение скорости к позиции
    QVector3D newPosition = shape->getPosition() + velocity * deltaTime;
    this->setPosition(newPosition);

    // Сброс аккумулятора сил
    forceAccumulator = QVector3D(0, 0, 0);
}

QVector<QVector3D> PhysicalObject::getTransformedVertices() const{
    QVector<QVector3D> transformed;
    for (const auto &vertex : shape->getVertices()) {
        transformed.append(shape->getModelMatrix().map(vertex.position));
    }
    return transformed;
}
