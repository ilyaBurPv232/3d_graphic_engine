#include "light.h"

Light::Light(QObject* parent)
    : QObject(parent),
    m_position(1.0f, 1.0f, 1.0f),
    m_color(1.0f, 1.0f, 1.0f),
    m_ambientStrength(0.1f),
    m_specularStrength(0.5f),
    m_shininess(32)
{
}

void Light::setDirection(const QVector3D& direction)
{
    if (m_direction != direction) {
        m_direction = direction;
        emit lightChanged();
    }
}

void Light::setLightType(int type)
{
    if (m_lightType != type && type >= 0 && type <= 2) {
        m_lightType = type;
        emit lightChanged();
    }
}

void Light::setCutOff(float cutOff)
{
    if (m_cutOff != cutOff) {
        m_cutOff = cutOff;
        emit lightChanged();
    }
}

void Light::setOuterCutOff(float outerCutOff)
{
    if (m_outerCutOff != outerCutOff) {
        m_outerCutOff = outerCutOff;
        emit lightChanged();
    }
}

void Light::setLightSphereEnabled(bool enabled)
{
    if (m_lightSphereEnabled != enabled) {
        m_lightSphereEnabled = enabled;
        emit lightChanged();
    }
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
