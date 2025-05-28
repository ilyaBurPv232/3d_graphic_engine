#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <QOpenGLBuffer>
#include "shape.h"

class Cube : public Shape
{
public:
    Cube(const QString &textureName = "default");
    ~Cube() = default;

    void initialize() override;
    void render(QOpenGLShaderProgram &program) override;
    QVector<VertexData> getVertices() const override;

private:
    void setupVertices();
    QString textureName;
    QVector<VertexData> vertexes;
};

class Pyramid : public Shape
{
public:
    Pyramid(const QString &textureName = "default");
    ~Pyramid() = default;

    void initialize() override;
    void render(QOpenGLShaderProgram &program) override;
    QVector<VertexData> getVertices() const override;

private:
    void setupVertices();
    QString textureName;
    QVector<VertexData> vertexes;
};

class Sphere : public Shape
{
public:
    Sphere(const QString &textureName = "default",
           float radius = 0.5f,
           int sectors = 36,
           int stacks = 18);
    ~Sphere() = default;

    void initialize() override;
    void render(QOpenGLShaderProgram &program) override;
    QVector<VertexData> getVertices() const override;
    float getRadius() const ;


private:
    void setupVertices();
    QString textureName;
    QVector<VertexData> vertexes;
    float radius;
    int sectors;
    int stacks;
};

class Cylinder : public Shape {
public:
    Cylinder(const QString& textureName = "default", float radius = 1.0f, float height = 2.0f,
             int sectors = 36, int stacks = 1);
    ~Cylinder() = default;

    void initialize() override;
    void render(QOpenGLShaderProgram& program) override;
    QVector<VertexData> getVertices() const override;
    float getRadius() const ;
    float getHeight() const;


private:
    void setupVertices();
    QString textureName;
    QVector<VertexData> vertexes;
    float radius;
    float height;
    int sectors;
    int stacks;
};

#endif // PRIMITIVES_H
