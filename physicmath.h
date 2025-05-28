#ifndef PHYSICMATH_H
#define PHYSICMATH_H

#include <QVector3D>
#include <QVector>
#include "physicalobject.h"

class PhysicsMath {
public:
    static float dotProduct(const QVector3D& a, const QVector3D& b);
    static QVector3D project(const QVector3D& vector, const QVector3D& onto);
    static bool isSeparatingAxis(const QVector3D& axis,
                                 const QVector<QVector3D>& vertsA,
                                 const QVector<QVector3D>& vertsB,
                                 float& penetrationDepth);
    static QVector3D calculateImpulse(PhysicalObject* objA,
                                      PhysicalObject* objB,
                                      const QVector3D& collisionNormal,
                                      const QVector3D& contactPoint,
                                      float restitution);
};

#endif // PHYSICMATH_H
