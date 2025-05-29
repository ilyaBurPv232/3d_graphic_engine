#ifndef SPRINGCONSTRAINT_H
#define SPRINGCONSTRAINT_H

#include "physicalobject.h"

class SpringConstraint
{
public:
    SpringConstraint(PhysicalObject* objA, PhysicalObject* objB, float restLength, float stiffness, float damping = 0.1f);

    void update(float deltaTime);
    float getCurrentLength() const;
    float getRestLength() const;
    void setRestLength(float length);
    float getStiffness() const;
    void setStiffness(float stiffness);
    float getDamping() const;
    void setDamping(float damping);

    PhysicalObject* getObjectA() const { return m_objA; }
    PhysicalObject* getObjectB() const { return m_objB; }

private:
    PhysicalObject* m_objA;
    PhysicalObject* m_objB;
    float m_restLength;
    float m_stiffness;
    float m_damping;
};

#endif // SPRINGCONSTRAINT_H
