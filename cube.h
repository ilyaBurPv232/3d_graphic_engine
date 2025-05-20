#ifndef CUBE_H
#define CUBE_H

#include "shape.h"
#include "texturemanager.h"

class Cube : public Shape
{
public:
    Cube(const QString& textureName = "default");
    ~Cube();

    void initialize() override;
    void render(QOpenGLShaderProgram& program) override;

private:
    void setupVertices();
    QString textureName;
};

#endif // CUBE_H
