#ifndef SHAPE_H
#define SHAPE_H

#include <QMatrix4x4>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QQuaternion>
#include <QVector2D>
#include <QVector3D>

struct VertexData
{
    VertexData() = default;
    VertexData(QVector3D p, QVector2D t, QVector3D n)
        : position(p)
        , texCoord(t)
        , normal(n)
    {}

    QVector3D position;
    QVector2D texCoord;
    QVector3D normal;
};

class Shape
{
public:
    Shape();
    virtual ~Shape();

    virtual void initialize() = 0;
    virtual void render(QOpenGLShaderProgram &program) = 0;

    void setPosition(const QVector3D &position);
    void setRotation(float angle, const QVector3D &axis);
    void setScale(const QVector3D &scale);
    QVector3D getPosition() const;
    virtual QVector<VertexData> getVertices() const = 0;
    QMatrix4x4 getModelMatrix() const;
    QVector3D getScale() const;

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
