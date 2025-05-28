#ifndef PHYSICALPRIMITIVES_H
#define PHYSICALPRIMITIVES_H

#include "physicalobject.h"


// Физический куб
class PhycCube : public PhysicalObject {
public:
    PhycCube(const QString& textureName = "default", double mass = 1.0, double elasticity = 0.5);
    void getAABB(QVector3D& min, QVector3D& max) const override;
};

// Физическая пирамида
class PhycPyramid : public PhysicalObject {
public:
    PhycPyramid(const QString& textureName = "default", double mass = 1.0, double elasticity = 0.5);
    void updateMinMax(QVector3D& min, QVector3D& max, const QVector3D& point);
    void getAABB(QVector3D& min, QVector3D& max) const override;
};

// Физическая сфера
class PhycSphere : public PhysicalObject {
public:
    PhycSphere(const QString& textureName = "default", double mass = 1.0,
               double elasticity = 0.5);
    void getAABB(QVector3D& min, QVector3D& max) const override;
};

// Физический цилиндр
class PhycCylinder : public PhysicalObject {
public:
    PhycCylinder(const QString& textureName = "default", double mass = 1.0,
                 double elasticity = 0.5);
    void getAABB(QVector3D& min, QVector3D& max) const override;
};

#endif // PHYSICALPRIMITIVES_H
