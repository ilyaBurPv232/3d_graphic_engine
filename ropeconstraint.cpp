#include "ropeconstraint.h"
#include <QVector3D>
#include <QtMath>

RopeConstraint::RopeConstraint(PhysicalObject* objA, PhysicalObject* objB, float maxLength)
    : m_objA(objA), m_objB(objB), m_maxLength(maxLength)
{}

void RopeConstraint::update(float deltaTime)
{
    if (!m_objA || !m_objB) return;

    QVector3D posA = m_objA->getPosition();
    QVector3D posB = m_objB->getPosition();
    QVector3D delta = posB - posA;
    float currentLength = delta.length();

    if (currentLength > m_maxLength)
    {
        QVector3D correction = delta.normalized() * (currentLength - m_maxLength);
        float totalInverseMass = m_objA->getInverseMass() + m_objB->getInverseMass();

        if (totalInverseMass > 0)
        {
            float ratioA = m_objA->getInverseMass() / totalInverseMass;
            float ratioB = m_objB->getInverseMass() / totalInverseMass;

            if (!m_objA->isStatic())
                m_objA->setPosition(posA + correction * ratioA);

            if (!m_objB->isStatic())
                m_objB->setPosition(posB - correction * ratioB);
        }
    }
}

float RopeConstraint::getCurrentLength() const
{
    if (!m_objA || !m_objB) return 0.0f;
    return (m_objB->getPosition() - m_objA->getPosition()).length();
}

float RopeConstraint::getMaxLength() const
{
    return m_maxLength;
}

void RopeConstraint::setMaxLength(float length)
{
    m_maxLength = qMax(0.0f, length);
}
