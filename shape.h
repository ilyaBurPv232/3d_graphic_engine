#ifndef SHAPE_H
#define SHAPE_H

#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QVector3D>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>

class Shape
{
public:
    Shape();
    virtual ~Shape();

    virtual void initialize() = 0;
    virtual void render(QOpenGLShaderProgram& program) = 0;

    void setPosition(const QVector3D& position);
    void setRotation(float angle, const QVector3D& axis);
    void setScale(const QVector3D& scale);
    void updateModelMatrix();

protected:
    QMatrix4x4 modelMatrix;
    QOpenGLBuffer vertexBuffer;
    QOpenGLTexture* texture = nullptr;
    QQuaternion rotationQuat;

    QVector3D scaleFactor = QVector3D(1, 1, 1);  // Инициализация единичным масштабом
    QVector3D position = QVector3D(0, 0, 0);     // Инициализация нулевой позицией
};

#endif // SHAPE_H
