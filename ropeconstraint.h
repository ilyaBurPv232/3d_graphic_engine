#ifndef ROPECONSTRAINT_H
#define ROPECONSTRAINT_H

#include "physicalobject.h"

class RopeConstraint
{
public:
    RopeConstraint(PhysicalObject* objA, PhysicalObject* objB, float maxLength);

    void update(float deltaTime);
    float getCurrentLength() const;
    float getMaxLength() const;
    void setMaxLength(float length);

    PhysicalObject* getObjectA() const { return m_objA; }
    PhysicalObject* getObjectB() const { return m_objB; }

private:
    PhysicalObject* m_objA;
    PhysicalObject* m_objB;
    float m_maxLength;
};

#endif // ROPECONSTRAINT_H
