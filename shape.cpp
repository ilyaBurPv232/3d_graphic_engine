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

QQuaternion Shape::getRotation() {
    return rotationQuat;
}

QMatrix4x4 Shape::getModelMatrix() const {
    return modelMatrix;
}

QVector3D Shape::getScale() const {
    return scaleFactor;
}

QVector3D Shape::getPosition() const {
    return position;
}


