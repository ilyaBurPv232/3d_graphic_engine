#ifndef PHYSICALWORLD_H
#define PHYSICALWORLD_H

#include <QVector3D>
#include <QVector>
#include "physicalobject.h"
#include "forcefield.h"
#include "ropeconstraint.h"
#include "springconstraint.h"
#include "rigidconstraint.h"

class PhysicalWorld
{
public:
    PhysicalWorld(const PhysicalWorld&) = delete;
    PhysicalWorld& operator=(const PhysicalWorld&) = delete;

    static PhysicalWorld& instance();

    void addObject(PhysicalObject* object);
    void removeObject(PhysicalObject* object);
    void clearObjects();
    void update(float deltaTime);

    QVector3D getGravity() const;
    void setGravity(const QVector3D& newGravity);

    PhysicalObject* getGroundPlane() const;
    void setGroundPlane(PhysicalObject* plane);

    const QVector<PhysicalObject*>& getObjects() const;

    void gravityUpdate() ;
    void updateObjects(float deltaTime);

    void addForceField(ForceField* field);
    void removeForceField(ForceField* field);
    void clearForceFields();
    const QVector<ForceField*>& getForceFields() const;

    void addRopeConstraint(RopeConstraint* constraint);
    void addSpringConstraint(SpringConstraint* constraint);
    void addRigidConstraint(RigidConstraint* constraint);

    void removeRopeConstraint(RopeConstraint* constraint);
    void removeSpringConstraint(SpringConstraint* constraint);
    void removeRigidConstraint(RigidConstraint* constraint);

    void clearConstraints();

    const QVector<RopeConstraint*>& getRopeConstraints() const;
    const QVector<SpringConstraint*>& getSpringConstraints() const;
    const QVector<RigidConstraint*>& getRigidConstraints() const;

private:
    PhysicalWorld(); // Приватный конструктор
    ~PhysicalWorld();

    QVector<ForceField*> forceFields;

    QVector<PhysicalObject*> objects;
    PhysicalObject* groundPlane = nullptr;
    QVector3D gravity = QVector3D(0, -9.8f, 0);

    QVector<RopeConstraint*> m_ropeConstraints;
    QVector<SpringConstraint*> m_springConstraints;
    QVector<RigidConstraint*> m_rigidConstraints;
};

#endif // PHYSICALWORLD_H
