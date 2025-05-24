#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include "shape.h"
#include <QOpenGLBuffer>


class Cube : public Shape {
public:
    Cube(const QString& textureName = "default");
    ~Cube() = default;

    void initialize() override;
    void render(QOpenGLShaderProgram& program) override;

private:
    void setupVertices();
    QString textureName;
};

class Pyramid : public Shape {
public:
    Pyramid(const QString& textureName = "default");
    ~Pyramid() = default;

    void initialize() override;
    void render(QOpenGLShaderProgram& program) override;

private:
    void setupVertices();
    QString textureName;
};

#endif // PRIMITIVES_H
