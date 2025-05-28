#include "collisionresolution.h"
#include "physicmath.h"
#include <QDebug>

CollisionResolution::CollisionResolution() {}

void CollisionResolution::resolveCollisions(const QVector<CollisionInfo>& collisions) {
    for (const CollisionInfo& collision : collisions) {
        resolveCollision(collision);
    }
}

void CollisionResolution::resolveCollision(const CollisionInfo& collision) {
    if (collision.objA->isStatic() && collision.objB->isStatic()) {
        return;
    }

    float restitution = qMin(collision.objA->getElasticity(), collision.objB->getElasticity());
    applyImpulse(collision.objA, collision.objB, collision.normal, collision.contactPoint, restitution);
    positionalCorrection(collision.objA, collision.objB, collision.normal, collision.penetration);
}

void CollisionResolution::applyImpulse(PhysicalObject* objA, PhysicalObject* objB,
                                       const QVector3D& normal, const QVector3D& contactPoint,
                                       float restitution) {
    // Рассчитываем общий импульс
    QVector3D impulse = PhysicsMath::calculateImpulse(objA, objB, normal, contactPoint, restitution);

    // Применяем импульс с учетом статичности объектов
    if (!objA->isStatic() && !objB->isStatic()) {
        // Оба объекта динамические - стандартное применение импульса
        objA->applyImpulse(-impulse);
        objB->applyImpulse(impulse);
    }
    else {
        // Хотя бы один объект статичный - применяем весь импульс к динамическому объекту
        PhysicalObject* dynamicObj = objA->isStatic() ? objB : objA;
        QVector3D effectiveImpulse = objA->isStatic() ? impulse : -impulse;
        dynamicObj->applyImpulse(effectiveImpulse);
    }
}

void CollisionResolution::positionalCorrection(PhysicalObject* objA, PhysicalObject* objB,
                                               const QVector3D& normal, float penetration) {
    const float percent = 0.2f;
    const float slop = 0.01f;
    float correction = (qMax(penetration - slop, 0.0f) / (objA->getInverseMass() + objB->getInverseMass())) * percent;

    QVector3D correctionVector = normal * correction;

    // Корректируем только динамические объекты
    if (!objA->isStatic()) {
        objA->setPosition(objA->getPosition() - correctionVector * objA->getInverseMass());
    }
    if (!objB->isStatic()) {
        objB->setPosition(objB->getPosition() + correctionVector * objB->getInverseMass());
    }
}
