#ifndef OBJMODEL_H
#define OBJMODEL_H

#include "shape.h"
#include <QVector3D>
#include <QVector2D>
#include <QVector>

class ObjModel : public Shape
{
public:
    ObjModel(const QString& objFilePath, const QString& textureName = "default");
    ~ObjModel() = default;

    void initialize() override;
    void render(QOpenGLShaderProgram& program) override;

private:
    void setupVertices();
    bool loadOBJ(const QString& path);

    QString m_objFilePath;
    QString m_textureName;

    // Временные структуры для загрузки
    QVector<QVector3D> temp_vertices;
    QVector<QVector2D> temp_uvs;
    QVector<QVector3D> temp_normals;

    QVector<VertexData> m_vertices;
    QVector<GLuint> m_indices;
};

#endif // OBJMODEL_H
