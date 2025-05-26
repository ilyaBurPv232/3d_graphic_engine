#include "physicalworld.h"
#include "physicalprimitives.h"
#include <QDebug>

PhysicalWorld::PhysicalWorld()
{
    // Создаем плоскость по умолчанию (статический куб)
    groundPlane = new PhycCube("parcet", 0, 0.5);

    groundPlane->setStatic(true);
    groundPlane->setPosition(QVector3D(0, -1, 0));
    groundPlane->getShape()->setScale(QVector3D(500, 1, 500));
}

PhysicalWorld::~PhysicalWorld()
{
    clearObjects();
    delete groundPlane;
}

PhysicalWorld& PhysicalWorld::instance()
{
    static PhysicalWorld instance;
    return instance;
}

void PhysicalWorld::addObject(PhysicalObject* object)
{
    if (object && !objects.contains(object)) {
        objects.append(object);
    }
}

void PhysicalWorld::removeObject(PhysicalObject* object)
{
    objects.removeOne(object);
}

void PhysicalWorld::clearObjects()
{
    qDeleteAll(objects);
    objects.clear();
}

void PhysicalWorld::update(float deltaTime)
{
    // Применяем гравитацию ко всем объектам
    for (PhysicalObject* obj : objects) {
        if (!obj->isStatic()) {
            obj->applyForce(gravity * static_cast<float>(obj->getMass()));
        }
    }

    // Обновляем все объекты
    for (PhysicalObject* obj : objects) {
        obj->update(deltaTime);
    }

    // Здесь можно добавить обработку коллизий
}

QVector3D PhysicalWorld::getGravity() const
{
    return gravity;
}

void PhysicalWorld::setGravity(const QVector3D& newGravity)
{
    gravity = newGravity;
}

PhysicalObject* PhysicalWorld::getGroundPlane() const
{
    return groundPlane;
}

void PhysicalWorld::setGroundPlane(PhysicalObject* plane)
{
    if (groundPlane != plane) {
        delete groundPlane;
        groundPlane = plane;
    }
}

const QVector<PhysicalObject*>& PhysicalWorld::getObjects() const
{
    return objects;
}
