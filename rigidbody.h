#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include <QVector3D>
#include <QQuaternion>
#include "physicalobject.h"

class RigidBody {
public:
    RigidBody();

private:
    PhysicalObject* odj;

    QVector3D angularVelocity;
    QMatrix3x3 inertiaTensor;
    QMatrix3x3 inverseInertiaTensor;\

    float linearDamping = 0.99f;
    float angularDamping = 0.95f;
};


#endif // RIGIDBODY_H
