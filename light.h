#ifndef LIGHT_H
#define LIGHT_H

#include <QVector3D>
#include <QObject>

class Light : public QObject
{
    Q_OBJECT
public:
    explicit Light(QObject* parent = nullptr);

    // Геттеры
    QVector3D position() const { return m_position; }
    QVector3D color() const { return m_color; }
    QVector3D direction() const { return m_direction; }
    float ambientStrength() const { return m_ambientStrength; }
    float specularStrength() const { return m_specularStrength; }
    int shininess() const { return m_shininess; }
    int lightType() const { return m_lightType; }
    float cutOff() const { return m_cutOff; }
    float outerCutOff() const { return m_outerCutOff; }
    bool lightSphereEnabled() const { return m_lightSphereEnabled; }

    // Сеттеры
    void setPosition(const QVector3D& position);
    void setColor(const QVector3D& color);
    void setDirection(const QVector3D& direction);
    void setAmbientStrength(float strength);
    void setSpecularStrength(float strength);
    void setShininess(int shininess);
    void setLightType(int type);
    void setCutOff(float cutOff);
    void setOuterCutOff(float outerCutOff);
    void setLightSphereEnabled(bool enabled);

signals:
    void lightChanged();

private:
    QVector3D m_position = QVector3D(0.0f, 5.0f, 0.0f); // Над объектами
    QVector3D m_color = QVector3D(1.0f, 1.0f, 1.0f);
    QVector3D m_direction = QVector3D(0.0f, -1.0f, 0.0f); // Направлен вниз
    float m_ambientStrength = 0.2f;
    float m_specularStrength = 0.5f;
    int m_shininess = 32;
    int m_lightType = 0; // 0=точечный, 1=направленный, 2=проекторный
    float m_cutOff = qCos(qDegreesToRadians(12.5f));
    float m_outerCutOff = qCos(qDegreesToRadians(17.5f));
    bool m_lightSphereEnabled = true; // Сфера-источник включена
};

#endif // LIGHT_H
