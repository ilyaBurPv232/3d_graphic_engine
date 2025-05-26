// collisiondetector.h
#ifndef COLLISIONDETECTOR_H
#define COLLISIONDETECTOR_H

#include <QVector>
#include "physicalprimitives.h"
#include "physicalobject.h"

struct CollisionInfo {
    PhysicalObject* bodyA;
    PhysicalObject* bodyB;
    QVector3D collisionNormal;  // Добавим нормаль столкновения
    float penetrationDepth;     // Глубина проникновения
};

class CollisionDetector {
public:
    QVector<CollisionInfo> detectCollisions(const QVector<PhysicalObject*>& bodies);

private:
    bool aabbCollision(const QVector3D &minA, const QVector3D &maxA,
                       const QVector3D &minB, const QVector3D &maxB,
                       QVector3D& collisionNormal, float& penetrationDepth);
};

#endif // COLLISIONDETECTOR_H
