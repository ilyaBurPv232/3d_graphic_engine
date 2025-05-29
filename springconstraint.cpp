#include "springconstraint.h"
#include <QVector3D>
#include <QtMath>

SpringConstraint::SpringConstraint(PhysicalObject* objA, PhysicalObject* objB,
                                   float restLength, float stiffness, float damping)
    : m_objA(objA), m_objB(objB), m_restLength(restLength),
    m_stiffness(stiffness), m_damping(damping)
{
}

void SpringConstraint::update(float deltaTime)
{
    if (!m_objA || !m_objB) return;

    QVector3D posA = m_objA->getPosition();
    QVector3D posB = m_objB->getPosition();
    QVector3D delta = posB - posA;
    float currentLength = delta.length();

    if (currentLength > 0)
    {
        QVector3D direction = delta / currentLength;
        float displacement = currentLength - m_restLength;

        QVector3D springForce = direction * displacement * m_stiffness;

        QVector3D relativeVelocity = m_objB->getVelocity() - m_objA->getVelocity();
        QVector3D dampingForce = direction * QVector3D::dotProduct(relativeVelocity, direction) * m_damping;

        QVector3D totalForce = springForce - dampingForce;

        if (!m_objA->isStatic())
            m_objA->applyForce(totalForce);

        if (!m_objB->isStatic())
            m_objB->applyForce(-totalForce);
    }
}

float SpringConstraint::getCurrentLength() const
{
    if (!m_objA || !m_objB) return 0.0f;
    return (m_objB->getPosition() - m_objA->getPosition()).length();
}

float SpringConstraint::getRestLength() const
{
    return m_restLength;
}

void SpringConstraint::setRestLength(float length)
{
    m_restLength = qMax(0.0f, length);
}

float SpringConstraint::getStiffness() const
{
    return m_stiffness;
}

void SpringConstraint::setStiffness(float stiffness)
{
    m_stiffness = qMax(0.0f, stiffness);
}

float SpringConstraint::getDamping() const
{
    return m_damping;
}

void SpringConstraint::setDamping(float damping)
{
    m_damping = qMax(0.0f, damping);
}
