#include "objmodel.h"
#include "texturemanager.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

ObjModel::ObjModel(const QString& objFilePath, const QString& textureName)
    : m_objFilePath(objFilePath), m_textureName(textureName)
{
    if (loadOBJ(objFilePath)) {
        setupVertices();
    } else {
        qWarning() << "Failed to load OBJ file:" << objFilePath;
    }
}

void ObjModel::initialize()
{
    if (!TextureManager::instance().contains(m_textureName)) {
        TextureManager::instance().loadTexture(":/textures/default.png", m_textureName);
    }
}

bool ObjModel::loadOBJ(const QString& path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Cannot open file:" << path;
        return false;
    }

    QTextStream in(&file);

    QVector<unsigned int> vertexIndices, uvIndices, normalIndices;
    QVector<QVector3D> temp_vertices_local;
    QVector<QVector2D> temp_uvs_local;
    QVector<QVector3D> temp_normals_local;

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(" ", Qt::SkipEmptyParts);

        if (parts.isEmpty()) continue;

        if (parts[0] == "v") {
            // Vertex position
            if (parts.size() >= 4) {
                float x = parts[1].toFloat();
                float y = parts[2].toFloat();
                float z = parts[3].toFloat();
                temp_vertices_local.append(QVector3D(x, y, z));
            }
        } else if (parts[0] == "vt") {
            // Texture coordinate
            if (parts.size() >= 3) {
                float u = parts[1].toFloat();
                float v = parts[2].toFloat();
                temp_uvs_local.append(QVector2D(u, v));
            }
        } else if (parts[0] == "vn") {
            // Vertex normal
            if (parts.size() >= 4) {
                float x = parts[1].toFloat();
                float y = parts[2].toFloat();
                float z = parts[3].toFloat();
                temp_normals_local.append(QVector3D(x, y, z));
            }
        } else if (parts[0] == "f") {
            // Face
            for (int i = 1; i < parts.size(); i++) {
                QStringList faceParts = parts[i].split("/");

                if (faceParts.size() >= 1) {
                    unsigned int vertexIndex = faceParts[0].toUInt();
                    vertexIndices.append(vertexIndex);
                }

                if (faceParts.size() >= 2 && !faceParts[1].isEmpty()) {
                    unsigned int uvIndex = faceParts[1].toUInt();
                    uvIndices.append(uvIndex);
                }

                if (faceParts.size() >= 3 && !faceParts[2].isEmpty()) {
                    unsigned int normalIndex = faceParts[2].toUInt();
                    normalIndices.append(normalIndex);
                }
            }
        }
    }

    file.close();

    // Преобразуем индексы в массивы вершин
    for (unsigned int i = 0; i < vertexIndices.size(); i++) {
        unsigned int vertexIndex = vertexIndices[i];
        unsigned int uvIndex = (uvIndices.size() > i) ? uvIndices[i] : 0;
        unsigned int normalIndex = (normalIndices.size() > i) ? normalIndices[i] : 0;

        // OBJ индексы начинаются с 1, наши массивы с 0
        QVector3D vertex = (vertexIndex - 1 < temp_vertices_local.size()) ?
                               temp_vertices_local[vertexIndex - 1] : QVector3D();

        QVector2D uv = (uvIndex - 1 < temp_uvs_local.size()) ?
                           temp_uvs_local[uvIndex - 1] : QVector2D();

        QVector3D normal = (normalIndex - 1 < temp_normals_local.size()) ?
                               temp_normals_local[normalIndex - 1] : QVector3D();

        m_vertices.append(VertexData(vertex, uv, normal));
        m_indices.append(i);
    }

    return true;
}

void ObjModel::setupVertices()
{
    vertexBuffer.create();
    vertexBuffer.bind();
    vertexBuffer.allocate(m_vertices.constData(), m_vertices.size() * sizeof(VertexData));
    vertexBuffer.release();

    indexBuffer.create();
    indexBuffer.bind();
    indexBuffer.allocate(m_indices.constData(), m_indices.size() * sizeof(GLuint));
    indexBuffer.release();
}

void ObjModel::render(QOpenGLShaderProgram& program)
{
    program.setUniformValue("model", modelMatrix);

    QOpenGLTexture* texture = TextureManager::instance().getTexture(m_textureName);
    if (texture) {
        texture->bind();
        program.setUniformValue("ourTexture", 0);
    }

    vertexBuffer.bind();
    indexBuffer.bind();

    int posLoc = program.attributeLocation("aPos");
    program.enableAttributeArray(posLoc);
    program.setAttributeBuffer(posLoc, GL_FLOAT, offsetof(VertexData, position), 3, sizeof(VertexData));

    int texLoc = program.attributeLocation("aTexCoord");
    program.enableAttributeArray(texLoc);
    program.setAttributeBuffer(texLoc, GL_FLOAT, offsetof(VertexData, texCoord), 2, sizeof(VertexData));

    int normalLoc = program.attributeLocation("aNormal");
    if (normalLoc != -1) {
        program.enableAttributeArray(normalLoc);
        program.setAttributeBuffer(normalLoc, GL_FLOAT, offsetof(VertexData, normal), 3, sizeof(VertexData));
    }

    glDrawElements(GL_TRIANGLES, indexBuffer.size(), GL_UNSIGNED_INT, 0);

    vertexBuffer.release();
    indexBuffer.release();

    if (texture) {
        texture->release();
    }
}
