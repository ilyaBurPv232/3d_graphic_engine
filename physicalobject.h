#ifndef PHYSICALOBJECT_H
#define PHYSICALOBJECT_H

#include <QVector3D>
#include <QVector>
#include "shape.h"

class PhysicalObject {
public:
    PhysicalObject(Shape* shape = nullptr,
                   double mass = 1.0,
                   double elasticity = 0.5);
    ~PhysicalObject();

    // Геттеры
    QVector3D getVelocity() const;
    QVector3D getAcceleration() const;
    double getMass() const;
    double getElasticity() const;
    QVector3D getPosition() const;
    Shape* getShape() const;
    bool isStatic() const;
    double getInverseMass() const;

    void setVelocity(const QVector3D& newVelocity);
    void setAcceleration(const QVector3D& newAcceleration);
    void setMass(double newMass);
    void setElasticity(double newElasticity);
    void setPosition(const QVector3D& position);
    void setStatic(bool isStatic);
    void setIsOnSurface(bool newIsOnSurface);
    void setFrictionCoefficient(double newFriction);

    void applyForce(const QVector3D& force);
    void applyImpulse(const QVector3D& impulse);
    void update(float deltaTime);
    QVector<QVector3D> getTransformedVertices() const;
    virtual void getAABB(QVector3D &min, QVector3D &max) const = 0;

    QVector3D getCenter() const ;
    float getBoundingSphereRadius() const;

protected:
    Shape* shape;
    QVector3D velocity;
    QVector3D acceleration;
    QVector3D forceAccumulator;
    double mass;
    double inverseMass;
    double elasticity;
    double frictionCoefficient;
    bool isStaticObject;
    bool isOnSurface = false;
};

#endif // PHYSICALOBJECT_H
