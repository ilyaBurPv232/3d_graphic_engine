#ifndef PHYSICALWORLD_H
#define PHYSICALWORLD_H

#include <QVector3D>
#include <QVector>
#include "physicalobject.h"

class PhysicalWorld
{
public:
    // Удаляем копирование и присваивание
    PhysicalWorld(const PhysicalWorld&) = delete;
    PhysicalWorld& operator=(const PhysicalWorld&) = delete;

    // Получение экземпляра синглтона
    static PhysicalWorld& instance();

    // Основные методы
    void addObject(PhysicalObject* object);
    void removeObject(PhysicalObject* object);
    void clearObjects();
    void update(float deltaTime);

    // Геттеры/сеттеры
    QVector3D getGravity() const;
    void setGravity(const QVector3D& newGravity);

    PhysicalObject* getGroundPlane() const;
    void setGroundPlane(PhysicalObject* plane);

    const QVector<PhysicalObject*>& getObjects() const;

private:
    PhysicalWorld(); // Приватный конструктор
    ~PhysicalWorld();

    QVector<PhysicalObject*> objects;
    PhysicalObject* groundPlane = nullptr;
    QVector3D gravity = QVector3D(0, -9.8f, 0);
};

#endif // PHYSICALWORLD_H
