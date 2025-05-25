#include "shape.h"

Shape::Shape()
    : indexBuffer(QOpenGLBuffer::IndexBuffer)
{
    vertexBuffer.create();
    indexBuffer.create();
}

Shape::~Shape()
{
    vertexBuffer.destroy();
    indexBuffer.destroy();
}

void Shape::setPosition(const QVector3D &position)
{
    this->position = position;
    updateModelMatrix();
}

void Shape::setRotation(float angle, const QVector3D &axis)
{
    rotationQuat = QQuaternion::fromAxisAndAngle(axis, angle) * rotationQuat;
    updateModelMatrix();
}

void Shape::setScale(const QVector3D &scale)
{
    scaleFactor = scale;
    updateModelMatrix();
}

void Shape::updateModelMatrix()
{
    modelMatrix.setToIdentity();
    modelMatrix.translate(position);
    modelMatrix.rotate(rotationQuat);
    modelMatrix.scale(scaleFactor);
}
