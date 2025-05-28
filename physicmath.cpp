#include "physicmath.h"
#include "physicalobject.h"
#include <cmath>
#include <limits>

float PhysicsMath::dotProduct(const QVector3D& a, const QVector3D& b) {
    return a.x() * b.x() + a.y() * b.y() + a.z() * b.z();
}

QVector3D PhysicsMath::project(const QVector3D& vector, const QVector3D& onto) {
    float scalar = dotProduct(vector, onto) / dotProduct(onto, onto);
    return onto * scalar;
}

bool PhysicsMath::isSeparatingAxis(const QVector3D& axis,
                                   const QVector<QVector3D>& vertsA,
                                   const QVector<QVector3D>& vertsB,
                                   float& penetrationDepth) {
    if (axis.isNull()) return false;

    float minA = std::numeric_limits<float>::max();
    float maxA = -std::numeric_limits<float>::max();
    for (const QVector3D& v : vertsA) {
        float projection = dotProduct(v, axis);
        minA = std::min(minA, projection);
        maxA = std::max(maxA, projection);
    }

    float minB = std::numeric_limits<float>::max();
    float maxB = -std::numeric_limits<float>::max();
    for (const QVector3D& v : vertsB) {
        float projection = dotProduct(v, axis);
        minB = std::min(minB, projection);
        maxB = std::max(maxB, projection);
    }

    if (maxA < minB || maxB < minA) {
        return true;
    }

    penetrationDepth = std::min(maxA - minB, maxB - minA);
    return false;
}

QVector3D PhysicsMath::calculateImpulse(PhysicalObject* objA,
                                        PhysicalObject* objB,
                                        const QVector3D& collisionNormal,
                                        const QVector3D& contactPoint,
                                        float restitution) {
    if (qFuzzyIsNull(collisionNormal.lengthSquared())) {
        qWarning() << "Zero collision normal!";
        return QVector3D(0, 0, 0);
    }

    // Рассчитываем относительную скорость
    QVector3D relativeVel = objB->getVelocity() - objA->getVelocity();
    float velAlongNormal = QVector3D::dotProduct(relativeVel, collisionNormal);

    // Объекты удаляются друг от друга - импульс не нужен
    if (velAlongNormal > 0) {
        return QVector3D(0, 0, 0);
    }

    // Учитываем, что статичный объект имеет бесконечную массу (inverseMass = 0)
    float invMassA = objA->getInverseMass();
    float invMassB = objB->getInverseMass();
    float totalInvMass = invMassA + invMassB;

    // Если оба объекта статичные
    if (totalInvMass <= 0) {
        return QVector3D(0, 0, 0);
    }

    // Модифицируем расчет импульса для статичных объектов
    float e = restitution;
    float j = -(1.0f + e) * velAlongNormal;

    // Для столкновения со статичным объектом учитываем только массу динамического объекта
    if (objA->isStatic() || objB->isStatic()) {
        j /= (objA->isStatic() ? invMassB : invMassA);
    } else {
        j /= totalInvMass;
    }

    // Уменьшаем импульс для более стабильного моделирования
    j *= 0.8f;

    return j * collisionNormal;
}
