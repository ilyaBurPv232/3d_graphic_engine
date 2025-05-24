#ifndef SHAPE_H
#define SHAPE_H

#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QVector3D>
#include <QVector2D>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>
#include <QQuaternion>


struct VertexData {
    VertexData() = default;
    VertexData(QVector3D p, QVector2D t, QVector3D n)
        : position(p), texCoord(t), normal(n) {}

    QVector3D position;
    QVector2D texCoord;
    QVector3D normal;
};

class Shape {
public:
    Shape();
    virtual ~Shape();

    virtual void initialize() = 0;
    virtual void render(QOpenGLShaderProgram& program) = 0;

    void setPosition(const QVector3D& position);
    void setRotation(float angle, const QVector3D& axis);
    void setScale(const QVector3D& scale);

protected:
    void updateModelMatrix();

    QMatrix4x4 modelMatrix;
    QOpenGLBuffer vertexBuffer;
    QOpenGLBuffer indexBuffer;
    QQuaternion rotationQuat;
    QVector3D position = QVector3D(0, 0, 0);
    QVector3D scaleFactor = QVector3D(1, 1, 1);
};

#endif // SHAPE_H
