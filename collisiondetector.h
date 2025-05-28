// collisiondetector.h
#ifndef COLLISIONDETECTOR_H
#define COLLISIONDETECTOR_H

#include <QVector>
#include "physicalobject.h"

struct CollisionInfo {
    PhysicalObject* objA;
    PhysicalObject* objB;
    QVector3D normal;
    QVector3D contactPoint;
    float penetration;
};

class CollisionDetector {
public:
    QVector<CollisionInfo> detectCollisions(const QVector<PhysicalObject*>& bodies, bool& isCollision);

private:
    bool aabbCollision(const QVector3D &minA, const QVector3D &maxA,
                       const QVector3D &minB, const QVector3D &maxB,
                       QVector3D& collisionNormal, float& penetrationDepth);
};

#endif // COLLISIONDETECTOR_H

