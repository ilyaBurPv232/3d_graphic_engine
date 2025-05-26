#ifndef SKYBOX_H
#define SKYBOX_H

#include "shape.h"


class Skybox : public Shape {
public:
    Skybox(const QString& textureName = "default");
    ~Skybox() = default;

    void initialize() override;
    void render(QOpenGLShaderProgram& program) override;
    QVector<VertexData> getVertices() const override;

    // QVector<QVector3D> getTransformedVertices() const override;
    // void getAABB(QVector3D &min, QVector3D &max) const override;

private:
    void setupVertices();
    QString textureName;
};

#endif // SKYBOX_H
