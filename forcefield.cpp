#include "forcefield.h"
#include <cmath>

ForceField::ForceField(ForceField::FieldType type, const QVector3D& position,
                       const QVector3D& direction, float strength, float radius)
    : type(type), position(position), direction(direction.normalized()),
    strength(strength), radius(radius) {}

void ForceField::applyForce(PhysicalObject* object) const {
    if (!object || object->isStatic()) return;

    QVector3D objectPos = object->getPosition();
    QVector3D force(0, 0, 0);

    switch (type) {
    case WIND: {
        // Постоянная сила ветра в заданном направлении
        force = direction * strength;
        break;
    }
    case MAGNETIC: {
        // Сила притяжения/отталкивания от центра поля
        QVector3D toCenter = position - objectPos;
        float distance = toCenter.length();
        if (radius > 0 && distance > radius) return;

        float falloff = radius > 0 ? (1.0f - distance / radius) : 1.0f;
        force = toCenter.normalized() * strength * falloff;
        break;
    }
    case VORTEX: {
        // Вихревая сила (перпендикулярно направлению к центру)
        QVector3D toCenter = position - objectPos;
        float distance = toCenter.length();
        if (radius > 0 && distance > radius) return;

        QVector3D tangent = QVector3D::crossProduct(direction, toCenter).normalized();
        float falloff = radius > 0 ? (1.0f - distance / radius) : 1.0f;
        force = tangent * strength * falloff;
        break;
    }
    case GRAVITY_POINT: {
        // Точечная гравитация (как черная дыра)
        QVector3D toCenter = position - objectPos;
        float distance = toCenter.length();
        if (distance < 0.1f) return; // Избегаем деления на 0

        float falloff = strength / (distance * distance);
        force = toCenter.normalized() * falloff;
        break;
    }
    case EXPLOSION: {
        // Сила взрыва (от центра)
        QVector3D fromCenter = objectPos - position;
        float distance = fromCenter.length();
        if (distance > radius) return;

        float falloff = (radius - distance) / radius;
        force = fromCenter.normalized() * strength * falloff * falloff;
        break;
    }
    case CUSTOM:
        // Пользовательская логика может быть добавлена здесь
        break;
    }

    object->applyForce(force * object->getMass());
}

// Геттеры и сеттеры
ForceField::FieldType ForceField::getType() const { return type; }
QVector3D ForceField::getPosition() const { return position; }
QVector3D ForceField::getDirection() const { return direction; }
float ForceField::getStrength() const { return strength; }
float ForceField::getRadius() const { return radius; }

void ForceField::setType(FieldType newType) { type = newType; }
void ForceField::setPosition(const QVector3D& newPosition) { position = newPosition; }
void ForceField::setDirection(const QVector3D& newDirection) { direction = newDirection.normalized(); }
void ForceField::setStrength(float newStrength) { strength = newStrength; }
void ForceField::setRadius(float newRadius) { radius = newRadius; }
