#include "light.h"

Light::Light(QObject* parent)
    : QObject(parent),
    m_position(-50.0f, 100.0f, 50.0f),
    m_color(1.0f, 1.0f, 1.0f),
    m_ambientStrength(0.1f),
    m_specularStrength(0.5f),
    m_shininess(32)
{
}

void Light::setPosition(const QVector3D& position)
{
    if (m_position != position) {
        m_position = position;
        emit lightChanged();
    }
}

void Light::setColor(const QVector3D& color)
{
    if (m_color != color) {
        m_color = color;
        emit lightChanged();
    }
}

void Light::setAmbientStrength(float strength)
{
    if (m_ambientStrength != strength) {
        m_ambientStrength = strength;
        emit lightChanged();
    }
}

void Light::setSpecularStrength(float strength)
{
    if (m_specularStrength != strength) {
        m_specularStrength = strength;
        emit lightChanged();
    }
}

void Light::setShininess(int shininess)
{
    if (m_shininess != shininess) {
        m_shininess = shininess;
        emit lightChanged();
    }
}
