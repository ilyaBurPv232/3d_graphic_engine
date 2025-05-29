#ifndef RIGIDCONSTRAINT_H
#define RIGIDCONSTRAINT_H

#include "physicalobject.h"

class RigidConstraint
{
public:
    RigidConstraint(PhysicalObject* objA, PhysicalObject* objB, float distance);

    void update(float deltaTime);
    float getDistance() const;
    void setDistance(float distance);

    PhysicalObject* getObjectA() const { return m_objA; }
    PhysicalObject* getObjectB() const { return m_objB; }

private:
    PhysicalObject* m_objA;
    PhysicalObject* m_objB;
    float m_distance;
};

#endif // RIGIDCONSTRAINT_H
