#ifndef LIGHT_H
#define LIGHT_H

#include <QVector3D>
#include <QObject>

class Light : public QObject
{
    Q_OBJECT

public:
    explicit Light(QObject* parent = nullptr);

    QVector3D position() const { return m_position; }
    QVector3D color() const { return m_color; }
    float ambientStrength() const { return m_ambientStrength; }
    float specularStrength() const { return m_specularStrength; }
    int shininess() const { return m_shininess; }

    void setPosition(const QVector3D& position);
    void setColor(const QVector3D& color);
    void setAmbientStrength(float strength);
    void setSpecularStrength(float strength);
    void setShininess(int shininess);

signals:
    void lightChanged();

private:
    QVector3D m_position;
    QVector3D m_color;
    float m_ambientStrength;
    float m_specularStrength;
    int m_shininess;
};

#endif // LIGHT_H
