#include "shape.h"
#include "texturemanager.h"

Shape::Shape() {
    vertexBuffer.create();
}

Shape::~Shape() {
    vertexBuffer.destroy();
    if (texture) delete texture;
}

void Shape::setPosition(const QVector3D& position) {
    modelMatrix.setToIdentity();
    modelMatrix.translate(position);
}

void Shape::setRotation(float angle, const QVector3D& axis) {
    rotationQuat = QQuaternion::fromAxisAndAngle(axis, angle) * rotationQuat;
    modelMatrix.setToIdentity();
    modelMatrix.rotate(rotationQuat);
}

void Shape::setScale(const QVector3D& scale) {
    modelMatrix.scale(scale);
}
