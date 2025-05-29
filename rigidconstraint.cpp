#include "rigidconstraint.h"
#include <QVector3D>
#include <QtMath>

RigidConstraint::RigidConstraint(PhysicalObject* objA, PhysicalObject* objB, float distance)
    : m_objA(objA), m_objB(objB), m_distance(distance)
{
}

void RigidConstraint::update(float deltaTime)
{
    if (!m_objA || !m_objB) return;

    QVector3D posA = m_objA->getPosition();
    QVector3D posB = m_objB->getPosition();
    QVector3D delta = posB - posA;
    float currentDistance = delta.length();

    if (currentDistance > 0)
    {
        QVector3D direction = delta / currentDistance;
        float correction = currentDistance - m_distance;

        float totalInverseMass = m_objA->getInverseMass() + m_objB->getInverseMass();

        if (totalInverseMass > 0)
        {
            float ratioA = m_objA->getInverseMass() / totalInverseMass;
            float ratioB = m_objB->getInverseMass() / totalInverseMass;

            if (!m_objA->isStatic())
                m_objA->setPosition(posA + direction * correction * ratioA);

            if (!m_objB->isStatic())
                m_objB->setPosition(posB - direction * correction * ratioB);
        }
    }
}

float RigidConstraint::getDistance() const
{
    return m_distance;
}

void RigidConstraint::setDistance(float distance)
{
    m_distance = qMax(0.0f, distance);
}
