#ifndef COLLISIONRESOLUTION_H
#define COLLISIONRESOLUTION_H

#include "physicalobject.h"
#include <QVector>
#include "collisiondetector.h"

class CollisionResolution {
public:
    CollisionResolution();

    void resolveCollisions(const QVector<CollisionInfo>& collisions);
    void resolveCollision(const CollisionInfo& collision);

private:
    void applyImpulse(PhysicalObject* objA, PhysicalObject* objB,
                      const QVector3D& normal, const QVector3D& contactPoint,
                      float restitution);
    void positionalCorrection(PhysicalObject* objA, PhysicalObject* objB,
                              const QVector3D& normal, float penetration);
};

#endif // COLLISIONRESOLUTION_H
