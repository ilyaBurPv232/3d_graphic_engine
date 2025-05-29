#include "physicalworld.h"
#include "physicalprimitives.h"
#include "collisiondetector.h"
#include "collisionresolution.h"
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
    clearForceFields();
    clearConstraints();
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
    for (PhysicalObject* obj : objects) {
        if (!obj->isStatic()) {
            obj->applyForce(gravity * static_cast<float>(obj->getMass()));
        }
    }

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

void PhysicalWorld::gravityUpdate() {
    for (PhysicalObject* object : objects){
        object->setAcceleration(getGravity());
        object->update(1 / 144.0f);

        QVector<PhysicalObject*> test;
        test.append(object);
        test.append(groundPlane);



    }
}

void PhysicalWorld::updateObjects(float deltaTime) {

    for (PhysicalObject* obj : objects) {
        if (!obj->isStatic()) {
            obj->applyForce(gravity * static_cast<float>(obj->getMass()));
        }
    }

    for (PhysicalObject* obj : objects) {
        obj->update(deltaTime);
    }

    for (RopeConstraint* constraint : m_ropeConstraints) {
        constraint->update(deltaTime);
    }

    for (SpringConstraint* constraint : m_springConstraints) {
        constraint->update(deltaTime);
    }

    for (RigidConstraint* constraint : m_rigidConstraints) {
        constraint->update(deltaTime);
    }

    CollisionDetector detector;
    bool hasCollisions = false;
    QVector<CollisionInfo> collisions = detector.detectCollisions(objects, hasCollisions);

    if (hasCollisions) {
        CollisionResolution resolver;
        resolver.resolveCollisions(collisions);
    }

    QVector<PhysicalObject*> allObjects = objects;
    if (groundPlane) {
        allObjects.append(groundPlane);
        bool groundCollisions = false;
        QVector<CollisionInfo> groundCols = detector.detectCollisions(allObjects, groundCollisions);

        if (groundCollisions) {
            CollisionResolution groundResolver;
            groundResolver.resolveCollisions(groundCols);
        }
    }

    for (ForceField* field : forceFields) {
        for (PhysicalObject* obj : objects) {
            field->applyForce(obj);
        }

        if (groundPlane) {
            field->applyForce(groundPlane);
        }
    }
}

void PhysicalWorld::addForceField(ForceField* field) {
    if (field && !forceFields.contains(field)) {
        forceFields.append(field);
    }
}

void PhysicalWorld::removeForceField(ForceField* field) {
    forceFields.removeOne(field);
}

void PhysicalWorld::clearForceFields() {
    qDeleteAll(forceFields);
    forceFields.clear();
}

const QVector<ForceField*>& PhysicalWorld::getForceFields() const {
    return forceFields;
}

void PhysicalWorld::addRopeConstraint(RopeConstraint* constraint) {
    if (constraint && !m_ropeConstraints.contains(constraint)) {
        m_ropeConstraints.append(constraint);
    }
}

void PhysicalWorld::addSpringConstraint(SpringConstraint* constraint) {
    if (constraint && !m_springConstraints.contains(constraint)) {
        m_springConstraints.append(constraint);
    }
}

void PhysicalWorld::addRigidConstraint(RigidConstraint* constraint) {
    if (constraint && !m_rigidConstraints.contains(constraint)) {
        m_rigidConstraints.append(constraint);
    }
}

void PhysicalWorld::removeRopeConstraint(RopeConstraint* constraint) {
    if (constraint) {
        m_ropeConstraints.removeOne(constraint);
    }
}

void PhysicalWorld::removeSpringConstraint(SpringConstraint* constraint) {
    if (constraint) {
        m_springConstraints.removeOne(constraint);
    }
}

void PhysicalWorld::removeRigidConstraint(RigidConstraint* constraint) {
    if (constraint) {
        m_rigidConstraints.removeOne(constraint);
    }
}

void PhysicalWorld::clearConstraints() {
    qDeleteAll(m_ropeConstraints);
    qDeleteAll(m_springConstraints);
    qDeleteAll(m_rigidConstraints);
    m_ropeConstraints.clear();
    m_springConstraints.clear();
    m_rigidConstraints.clear();
}

