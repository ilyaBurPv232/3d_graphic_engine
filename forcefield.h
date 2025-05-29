#ifndef FORCEFIELD_H
#define FORCEFIELD_H

#include <QVector3D>
#include <QVector>
#include "physicalobject.h"

class ForceField {
public:
    enum FieldType {
        WIND,           // Постоянный ветер в определенном направлении
        MAGNETIC,       // Магнитное поле (притяжение/отталкивание)
        VORTEX,         // Вихревое поле (вращательное воздействие)
        GRAVITY_POINT,  // Точечный источник гравитации
        EXPLOSION,      // Взрывная волна (радиальное воздействие)
        CUSTOM          // Пользовательское поле
    };

    ForceField(FieldType type, const QVector3D& position = QVector3D(),
               const QVector3D& direction = QVector3D(0, -1, 0),
               float strength = 1.0f, float radius = 10.0f);

    // Применить силу к объекту
    void applyForce(PhysicalObject* object) const;

    // Геттеры и сеттеры
    FieldType getType() const;
    QVector3D getPosition() const;
    QVector3D getDirection() const;
    float getStrength() const;
    float getRadius() const;

    void setType(FieldType newType);
    void setPosition(const QVector3D& newPosition);
    void setDirection(const QVector3D& newDirection);
    void setStrength(float newStrength);
    void setRadius(float newRadius);

private:
    FieldType type;
    QVector3D position;     // Центр поля (для точечных полей)
    QVector3D direction;    // Направление воздействия
    float strength;         // Сила воздействия
    float radius;           // Радиус действия (0 для бесконечного)
};

#endif // FORCEFIELD_H
