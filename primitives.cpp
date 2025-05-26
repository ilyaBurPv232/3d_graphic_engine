#include "primitives.h"
#include "texturemanager.h"

#include <QDebug>
#include <QOpenGLTexture>
#include <cmath>

Cube::Cube(const QString &textureName)
    : textureName(textureName)
{
    setupVertices();
}

void Cube::initialize()
{
    if (!TextureManager::instance().contains(textureName)) {
        TextureManager::instance().loadTexture(":/textures/grass.png", textureName);
    }
}

void Cube::setupVertices()
{
    float w_div_2 = 0.5f;

    vertexes.append(VertexData(QVector3D(-w_div_2, w_div_2, w_div_2), // верх-лево
                               QVector2D(0.0f, 1.0f),
                               QVector3D(0.0f, 0.0f, 1.0f)));
    vertexes.append(VertexData(QVector3D(-w_div_2, -w_div_2, w_div_2), // низ-лево
                               QVector2D(0.0f, 0.0f),
                               QVector3D(0.0f, 0.0f, 1.0f)));
    vertexes.append(VertexData(QVector3D(w_div_2, w_div_2, w_div_2), // верх-право
                               QVector2D(1.0f, 1.0f),
                               QVector3D(0.0f, 0.0f, 1.0f)));
    vertexes.append(VertexData(QVector3D(w_div_2, -w_div_2, w_div_2), // низ-право
                               QVector2D(1.0f, 0.0f),
                               QVector3D(0.0f, 0.0f, 1.0f)));

    // Задняя грань (Z-)
    vertexes.append(VertexData(QVector3D(w_div_2, w_div_2, -w_div_2), // верх-право
                               QVector2D(1.0f, 1.0f),
                               QVector3D(0.0f, 0.0f, -1.0f)));
    vertexes.append(VertexData(QVector3D(w_div_2, -w_div_2, -w_div_2), // низ-право
                               QVector2D(1.0f, 0.0f),
                               QVector3D(0.0f, 0.0f, -1.0f)));
    vertexes.append(VertexData(QVector3D(-w_div_2, w_div_2, -w_div_2), // верх-лево
                               QVector2D(0.0f, 1.0f),
                               QVector3D(0.0f, 0.0f, -1.0f)));
    vertexes.append(VertexData(QVector3D(-w_div_2, -w_div_2, -w_div_2), // низ-лево
                               QVector2D(0.0f, 0.0f),
                               QVector3D(0.0f, 0.0f, -1.0f)));

    // Левая грань (X-)
    vertexes.append(VertexData(QVector3D(-w_div_2, w_div_2, -w_div_2), // верх-зад
                               QVector2D(0.0f, 1.0f),
                               QVector3D(-1.0f, 0.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(-w_div_2, -w_div_2, -w_div_2), // низ-зад
                               QVector2D(0.0f, 0.0f),
                               QVector3D(-1.0f, 0.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(-w_div_2, w_div_2, w_div_2), // верх-перед
                               QVector2D(1.0f, 1.0f),
                               QVector3D(-1.0f, 0.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(-w_div_2, -w_div_2, w_div_2), // низ-перед
                               QVector2D(1.0f, 0.0f),
                               QVector3D(-1.0f, 0.0f, 0.0f)));

    // Правая грань (X+)
    vertexes.append(VertexData(QVector3D(w_div_2, w_div_2, w_div_2), // верх-перед
                               QVector2D(0.0f, 1.0f),
                               QVector3D(1.0f, 0.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(w_div_2, -w_div_2, w_div_2), // низ-перед
                               QVector2D(0.0f, 0.0f),
                               QVector3D(1.0f, 0.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(w_div_2, w_div_2, -w_div_2), // верх-зад
                               QVector2D(1.0f, 1.0f),
                               QVector3D(1.0f, 0.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(w_div_2, -w_div_2, -w_div_2), // низ-зад
                               QVector2D(1.0f, 0.0f),
                               QVector3D(1.0f, 0.0f, 0.0f)));

    // Верхняя грань (Y+)
    vertexes.append(VertexData(QVector3D(-w_div_2, w_div_2, -w_div_2), // зад-лево
                               QVector2D(0.0f, 1.0f),
                               QVector3D(0.0f, 1.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(-w_div_2, w_div_2, w_div_2), // перед-лево
                               QVector2D(0.0f, 0.0f),
                               QVector3D(0.0f, 1.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(w_div_2, w_div_2, -w_div_2), // зад-право
                               QVector2D(1.0f, 1.0f),
                               QVector3D(0.0f, 1.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(w_div_2, w_div_2, w_div_2), // перед-право
                               QVector2D(1.0f, 0.0f),
                               QVector3D(0.0f, 1.0f, 0.0f)));

    // Нижняя грань (Y-)
    vertexes.append(VertexData(QVector3D(-w_div_2, -w_div_2, w_div_2), // перед-лево
                               QVector2D(0.0f, 1.0f),
                               QVector3D(0.0f, -1.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(-w_div_2, -w_div_2, -w_div_2), // зад-лево
                               QVector2D(0.0f, 0.0f),
                               QVector3D(0.0f, -1.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(w_div_2, -w_div_2, w_div_2), // перед-право
                               QVector2D(1.0f, 1.0f),
                               QVector3D(0.0f, -1.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(w_div_2, -w_div_2, -w_div_2), // зад-право
                               QVector2D(1.0f, 0.0f),
                               QVector3D(0.0f, -1.0f, 0.0f)));

    QVector<GLuint> indexes;
    for (int i = 0; i < 24; i += 4) {
        // Первый треугольник (против часовой стрелки)
        indexes.append(i + 0);
        indexes.append(i + 1);
        indexes.append(i + 2);

        // Второй треугольник (против часовой стрелки)
        indexes.append(i + 2);
        indexes.append(i + 1);
        indexes.append(i + 3);
    }

    vertexBuffer.create();
    vertexBuffer.bind();
    vertexBuffer.allocate(vertexes.constData(), vertexes.size() * sizeof(VertexData));
    vertexBuffer.release();

    indexBuffer.create();
    indexBuffer.bind();
    indexBuffer.allocate(indexes.constData(), indexes.size() * sizeof(GLuint));
    indexBuffer.release();
}

void Cube::render(QOpenGLShaderProgram &program)
{
    program.setUniformValue("model", modelMatrix);

    QOpenGLTexture *texture = TextureManager::instance().getTexture(textureName);
    if (texture) {
        texture->bind();
        program.setUniformValue("ourTexture", 0);
    }

    vertexBuffer.bind();
    indexBuffer.bind();

    int posLoc = program.attributeLocation("aPos");
    program.enableAttributeArray(posLoc);
    program.setAttributeBuffer(posLoc,
                               GL_FLOAT,
                               offsetof(VertexData, position),
                               3,
                               sizeof(VertexData));

    int texLoc = program.attributeLocation("aTexCoord");
    program.enableAttributeArray(texLoc);
    program.setAttributeBuffer(texLoc,
                               GL_FLOAT,
                               offsetof(VertexData, texCoord),
                               2,
                               sizeof(VertexData));

    int normalLoc = program.attributeLocation("aNormal");
    if (normalLoc != -1) {
        program.enableAttributeArray(normalLoc);
        program.setAttributeBuffer(normalLoc,
                                   GL_FLOAT,
                                   offsetof(VertexData, normal),
                                   3,
                                   sizeof(VertexData));
    }

    glDrawElements(GL_TRIANGLES, indexBuffer.size(), GL_UNSIGNED_INT, 0);

    vertexBuffer.release();
    indexBuffer.release();
    if (texture)
        texture->release();
}

QVector<VertexData> Cube::getVertices() const {
    return vertexes;
}

// QVector<QVector3D> Cube::getTransformedVertices() const {
//     QVector<QVector3D> transformed;
//     for (const auto &vertex : vertexes) {
//         transformed.append(modelMatrix * vertex.position);
//     }
//     return transformed;
// }

// void Cube::getAABB(QVector3D &min, QVector3D &max) const {
//     QVector3D localMin(-0.5f, -0.5f, -0.5f);
//     QVector3D localMax(0.5f, 0.5f, 0.5f);

//     QVector3D corners[8] = {
//         QVector3D(localMin.x(), localMin.y(), localMin.z()),
//         QVector3D(localMax.x(), localMin.y(), localMin.z()),
//         QVector3D(localMin.x(), localMax.y(), localMin.z()),
//         QVector3D(localMax.x(), localMax.y(), localMin.z()),
//         QVector3D(localMin.x(), localMin.y(), localMax.z()),
//         QVector3D(localMax.x(), localMin.y(), localMax.z()),
//         QVector3D(localMin.x(), localMax.y(), localMax.z()),
//         QVector3D(localMax.x(), localMax.y(), localMax.z())
//     };

//     min = QVector3D(INFINITY, INFINITY, INFINITY);
//     max = QVector3D(-INFINITY, -INFINITY, -INFINITY);

//     for (const auto& corner : corners) {
//         QVector3D transformed = modelMatrix * corner;
//         min.setX(qMin(min.x(), transformed.x()));
//         min.setY(qMin(min.y(), transformed.y()));
//         min.setZ(qMin(min.z(), transformed.z()));
//         max.setX(qMax(max.x(), transformed.x()));
//         max.setY(qMax(max.y(), transformed.y()));
//         max.setZ(qMax(max.z(), transformed.z()));
//     }
// }

Pyramid::Pyramid(const QString &textureName)
    : textureName(textureName)
{
    setupVertices();
}

void Pyramid::initialize()
{
    if (!TextureManager::instance().contains(textureName)) {
        TextureManager::instance().loadTexture(":/textures/default.png", textureName);
    }
}

void Pyramid::setupVertices()
{

    // Основание пирамиды (4 вершины)
    vertexes.append(VertexData(QVector3D(-0.5f, -0.5f, -0.5f),
                               QVector2D(0.0f, 0.0f),
                               QVector3D(0.0f, -1.0f, 0.0f))); // 0
    vertexes.append(VertexData(QVector3D(0.5f, -0.5f, -0.5f),
                               QVector2D(1.0f, 0.0f),
                               QVector3D(0.0f, -1.0f, 0.0f))); // 1
    vertexes.append(VertexData(QVector3D(0.5f, -0.5f, 0.5f),
                               QVector2D(1.0f, 1.0f),
                               QVector3D(0.0f, -1.0f, 0.0f))); // 2
    vertexes.append(VertexData(QVector3D(-0.5f, -0.5f, 0.5f),
                               QVector2D(0.0f, 1.0f),
                               QVector3D(0.0f, -1.0f, 0.0f))); // 3

    // Вершина пирамиды (центр верхней точки)
    const QVector3D top(0.0f, 0.5f, 0.0f);

    // Передняя грань (Z+)
    QVector3D frontNormal = QVector3D::crossProduct(QVector3D(-0.5f, -0.5f, 0.5f) - top,
                                                    QVector3D(0.5f, -0.5f, 0.5f) - top)
                                .normalized();

    vertexes.append(VertexData(top, QVector2D(0.5f, 1.0f), frontNormal)); // 4
    vertexes.append(
        VertexData(QVector3D(-0.5f, -0.5f, 0.5f), QVector2D(0.0f, 0.0f), frontNormal)); // 5
    vertexes.append(
        VertexData(QVector3D(0.5f, -0.5f, 0.5f), QVector2D(1.0f, 0.0f), frontNormal)); // 6

    // Правая грань (X+)
    QVector3D rightNormal = QVector3D::crossProduct(QVector3D(0.5f, -0.5f, 0.5f) - top,
                                                    QVector3D(0.5f, -0.5f, -0.5f) - top)
                                .normalized();

    vertexes.append(VertexData(top, QVector2D(0.5f, 1.0f), rightNormal)); // 7
    vertexes.append(
        VertexData(QVector3D(0.5f, -0.5f, 0.5f), QVector2D(0.0f, 0.0f), rightNormal)); // 8
    vertexes.append(
        VertexData(QVector3D(0.5f, -0.5f, -0.5f), QVector2D(1.0f, 0.0f), rightNormal)); // 9

    // Задняя грань (Z-)
    QVector3D backNormal = QVector3D::crossProduct(QVector3D(0.5f, -0.5f, -0.5f) - top,
                                                   QVector3D(-0.5f, -0.5f, -0.5f) - top)
                               .normalized();

    vertexes.append(VertexData(top, QVector2D(0.5f, 1.0f), backNormal)); // 10
    vertexes.append(
        VertexData(QVector3D(0.5f, -0.5f, -0.5f), QVector2D(0.0f, 0.0f), backNormal)); // 11
    vertexes.append(
        VertexData(QVector3D(-0.5f, -0.5f, -0.5f), QVector2D(1.0f, 0.0f), backNormal)); // 12

    // Левая грань (X-)
    QVector3D leftNormal = QVector3D::crossProduct(QVector3D(-0.5f, -0.5f, -0.5f) - top,
                                                   QVector3D(-0.5f, -0.5f, 0.5f) - top)
                               .normalized();

    vertexes.append(VertexData(top, QVector2D(0.5f, 1.0f), leftNormal)); // 13
    vertexes.append(
        VertexData(QVector3D(-0.5f, -0.5f, -0.5f), QVector2D(0.0f, 0.0f), leftNormal)); // 14
    vertexes.append(
        VertexData(QVector3D(-0.5f, -0.5f, 0.5f), QVector2D(1.0f, 0.0f), leftNormal)); // 15

    // Индексы для треугольников
    QVector<GLuint> indexes = {// Основание (2 треугольника)
                               0,
                               1,
                               2,
                               2,
                               3,
                               0,

                               // Передняя грань
                               4,
                               5,
                               6,

                               // Правая грань
                               7,
                               8,
                               9,

                               // Задняя грань
                               10,
                               11,
                               12,

                               // Левая грань
                               13,
                               14,
                               15};

    vertexBuffer.create();
    vertexBuffer.bind();
    vertexBuffer.allocate(vertexes.constData(), vertexes.size() * sizeof(VertexData));
    vertexBuffer.release();

    indexBuffer.create();
    indexBuffer.bind();
    indexBuffer.allocate(indexes.constData(), indexes.size() * sizeof(GLuint));
    indexBuffer.release();
}

void Pyramid::render(QOpenGLShaderProgram &program)
{
    program.setUniformValue("model", modelMatrix);

    QOpenGLTexture *texture = TextureManager::instance().getTexture(textureName);
    if (texture) {
        texture->bind();
        program.setUniformValue("ourTexture", 0);
    }

    vertexBuffer.bind();
    indexBuffer.bind();

    int posLoc = program.attributeLocation("aPos");
    program.enableAttributeArray(posLoc);
    program.setAttributeBuffer(posLoc,
                               GL_FLOAT,
                               offsetof(VertexData, position),
                               3,
                               sizeof(VertexData));

    int texLoc = program.attributeLocation("aTexCoord");
    program.enableAttributeArray(texLoc);
    program.setAttributeBuffer(texLoc,
                               GL_FLOAT,
                               offsetof(VertexData, texCoord),
                               2,
                               sizeof(VertexData));

    int normalLoc = program.attributeLocation("aNormal");
    if (normalLoc != -1) {
        program.enableAttributeArray(normalLoc);
        program.setAttributeBuffer(normalLoc,
                                   GL_FLOAT,
                                   offsetof(VertexData, normal),
                                   3,
                                   sizeof(VertexData));
    }

    glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);

    vertexBuffer.release();
    indexBuffer.release();
    if (texture)
        texture->release();
}

QVector<VertexData> Pyramid::getVertices() const {
    return vertexes;
}

// QVector<QVector3D> Pyramid::getTransformedVertices() const {
//     QVector<QVector3D> transformed;
//     for (const auto &vertex : vertexes) {
//         transformed.append(modelMatrix * vertex.position);
//     }
//     return transformed;
// }

// void Pyramid::getAABB(QVector3D &min, QVector3D &max) const {
//     QVector3D base[4] = {
//         QVector3D(-0.5f, -0.5f, -0.5f),
//         QVector3D(0.5f, -0.5f, -0.5f),
//         QVector3D(0.5f, -0.5f, 0.5f),
//         QVector3D(-0.5f, -0.5f, 0.5f)
//     };
//     QVector3D top(0.0f, 0.5f, 0.0f);

//     min = QVector3D(INFINITY, INFINITY, INFINITY);
//     max = QVector3D(-INFINITY, -INFINITY, -INFINITY);

//     for (const auto& v : base) {
//         QVector3D transformed = modelMatrix * v;
//         min.setX(qMin(min.x(), transformed.x()));
//         min.setY(qMin(min.y(), transformed.y()));
//         min.setZ(qMin(min.z(), transformed.z()));
//         max.setX(qMax(max.x(), transformed.x()));
//         max.setY(qMax(max.y(), transformed.y()));
//         max.setZ(qMax(max.z(), transformed.z()));
//     }

//     QVector3D transformedTop = modelMatrix * top;
//     min.setX(qMin(min.x(), transformedTop.x()));
//     min.setY(qMin(min.y(), transformedTop.y()));
//     min.setZ(qMin(min.z(), transformedTop.z()));
//     max.setX(qMax(max.x(), transformedTop.x()));
//     max.setY(qMax(max.y(), transformedTop.y()));
//     max.setZ(qMax(max.z(), transformedTop.z()));
// }

Sphere::Sphere(const QString &textureName, float radius, int sectors, int stacks)
    : textureName(textureName)
    , radius(radius)
    , sectors(sectors)
    , stacks(stacks)
{
    setupVertices();
}

void Sphere::initialize()
{
    if (!TextureManager::instance().contains(textureName)) {
        TextureManager::instance().loadTexture(":/textures/default.png", textureName);
    }
}

void Sphere::setupVertices() {
    QVector<GLuint> indices;

    const float PI = 3.14159265358979323846f;
    float x, y, z, xy;
    float nx, ny, nz, lengthInv = 1.0f / radius;
    float s, t;

    float sectorStep = 2 * PI / sectors;
    float stackStep = PI / stacks;
    float sectorAngle, stackAngle;

    for (int i = 0; i <= stacks; ++i) {
        stackAngle = PI / 2 - i * stackStep;
        xy = radius * cosf(stackAngle);
        z = radius * sinf(stackAngle);

        for (int j = 0; j <= sectors; ++j) {
            sectorAngle = j * sectorStep;

            x = xy * cosf(sectorAngle);
            y = xy * sinf(sectorAngle);

            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;

            s = (float) j / sectors;
            t = (float) i / stacks;

            vertexes.append(VertexData(QVector3D(x, y, z), QVector2D(s, t), QVector3D(nx, ny, nz)));
        }
    }

    int k1, k2;
    for (int i = 0; i < stacks; ++i) {
        k1 = i * (sectors + 1);
        k2 = k1 + sectors + 1;

        for (int j = 0; j < sectors; ++j, ++k1, ++k2) {
            if (i != 0) {
                indices.append(k1);
                indices.append(k2);
                indices.append(k1 + 1);
            }

            if (i != (stacks - 1)) {
                indices.append(k1 + 1);
                indices.append(k2);
                indices.append(k2 + 1);
            }
        }
    }

    vertexBuffer.create();
    vertexBuffer.bind();
    vertexBuffer.allocate(vertexes.constData(), vertexes.size() * sizeof(VertexData));
    vertexBuffer.release();

    indexBuffer.create();
    indexBuffer.bind();
    indexBuffer.allocate(indices.constData(), indices.size() * sizeof(GLuint));
    indexBuffer.release();
}

void Sphere::render(QOpenGLShaderProgram &program)
{
    program.setUniformValue("model", modelMatrix);

    QOpenGLTexture *texture = TextureManager::instance().getTexture(textureName);
    if (texture) {
        texture->bind();
        program.setUniformValue("ourTexture", 0);
    }

    vertexBuffer.bind();
    indexBuffer.bind();

    int posLoc = program.attributeLocation("aPos");
    program.enableAttributeArray(posLoc);
    program.setAttributeBuffer(posLoc,
                               GL_FLOAT,
                               offsetof(VertexData, position),
                               3,
                               sizeof(VertexData));

    int texLoc = program.attributeLocation("aTexCoord");
    program.enableAttributeArray(texLoc);
    program.setAttributeBuffer(texLoc,
                               GL_FLOAT,
                               offsetof(VertexData, texCoord),
                               2,
                               sizeof(VertexData));

    int normalLoc = program.attributeLocation("aNormal");
    if (normalLoc != -1) {
        program.enableAttributeArray(normalLoc);
        program.setAttributeBuffer(normalLoc,
                                   GL_FLOAT,
                                   offsetof(VertexData, normal),
                                   3,
                                   sizeof(VertexData));
    }

    glDrawElements(GL_TRIANGLES, indexBuffer.size(), GL_UNSIGNED_INT, 0);

    vertexBuffer.release();
    indexBuffer.release();
    if (texture)
        texture->release();
}

QVector<VertexData> Sphere::getVertices() const {
    return vertexes;
}

float Sphere::getRadius() const {
    return radius;
}

// QVector<QVector3D> Sphere::getTransformedVertices() const {
//     QVector<QVector3D> transformed;
//     for (const auto &vertex : vertexes) {
//         transformed.append(modelMatrix * vertex.position);
//     }
//     return transformed;
// }

// void Sphere::getAABB(QVector3D &min, QVector3D &max) const {
//     QVector3D center = position;
//     float scaledRadius = radius * qMax(scaleFactor.x(), qMax(scaleFactor.y(), scaleFactor.z()));

//     min = center - QVector3D(scaledRadius, scaledRadius, scaledRadius);
//     max = center + QVector3D(scaledRadius, scaledRadius, scaledRadius);
// }

Cylinder::Cylinder(const QString& textureName, float radius, float height, int sectors, int stacks)
    : textureName(textureName), radius(radius), height(height), sectors(sectors), stacks(stacks)
{
    setupVertices();
}

void Cylinder::initialize()
{
    if (!TextureManager::instance().contains(textureName)) {
        TextureManager::instance().loadTexture(":/textures/default.png", textureName);
    }
}

void Cylinder::setupVertices()
{
    QVector<VertexData> vertices;
    QVector<GLuint> indices;

    const float PI = 3.14159265358979323846f;
    float sectorStep = 2 * PI / sectors;
    float stackHeight = height / stacks;

    // 1. Боковая поверхность
    for(int i = 0; i <= stacks; ++i) {
        float y = -height / 2.0f + i * stackHeight;

        for(int j = 0; j <= sectors; ++j) {
            float sectorAngle = j * sectorStep;
            float x = radius * cosf(sectorAngle);
            float z = radius * sinf(sectorAngle);

            // Нормаль направлена наружу
            QVector3D normal(cosf(sectorAngle), 0.0f, sinf(sectorAngle));

            // Текстурные координаты
            float s = (float)j / sectors;
            float t = (float)i / stacks;

            vertices.append(VertexData(
                QVector3D(x, y, z),
                QVector2D(s, t),
                normal
                ));
        }
    }

    // Индексы для боковой поверхности
    for(int i = 0; i < stacks; ++i) {
        int k1 = i * (sectors + 1);
        int k2 = k1 + sectors + 1;

        for(int j = 0; j < sectors; ++j, ++k1, ++k2) {
            // Первый треугольник
            indices.append(k1);
            indices.append(k1 + 1);
            indices.append(k2);

            // Второй треугольник
            indices.append(k2);
            indices.append(k1 + 1);
            indices.append(k2 + 1);
        }
    }

    // 2. Верхняя крышка
    int baseIndexTop = vertices.size();
    float yTop = height / 2.0f;

    // Центр верхней крышки
    vertices.append(
        VertexData(QVector3D(0.0f, yTop, 0.0f), QVector2D(0.5f, 0.5f), QVector3D(0.0f, 1.0f, 0.0f)));

    // Вершины верхней крышки
    for (int j = 0; j <= sectors; ++j) {
        float sectorAngle = j * sectorStep;
        float x = radius * cosf(sectorAngle);
        float z = radius * sinf(sectorAngle);

        vertices.append(
            VertexData(QVector3D(x, yTop, z),
                       QVector2D(0.5f + 0.5f * cosf(sectorAngle), 0.5f + 0.5f * sinf(sectorAngle)),
                       QVector3D(0.0f, 1.0f, 0.0f)));
    }

    // Индексы верхней крышки
    for(int j = 0; j < sectors; ++j) {
        indices.append(baseIndexTop);
        indices.append(baseIndexTop + j + 2);
        indices.append(baseIndexTop + j + 1);
    }

    // 3. Нижняя крышка
    int baseIndexBottom = vertices.size();
    float yBottom = -height / 2.0f;

    // Центр нижней крышки
    vertices.append(VertexData(
        QVector3D(0.0f, yBottom, 0.0f),
        QVector2D(0.5f, 0.5f),
        QVector3D(0.0f, -1.0f, 0.0f)
        ));

    // Вершины нижней крышки
    for(int j = 0; j <= sectors; ++j) {
        float sectorAngle = j * sectorStep;
        float x = radius * cosf(sectorAngle);
        float z = radius * sinf(sectorAngle);

        vertices.append(VertexData(
            QVector3D(x, yBottom, z),
            QVector2D(0.5f + 0.5f * cosf(sectorAngle),
                      0.5f + 0.5f * sinf(sectorAngle)),
            QVector3D(0.0f, -1.0f, 0.0f)
            ));
    }

    // Индексы нижней крышки (по часовой стрелке)
    for(int j = 0; j < sectors; ++j) {
        indices.append(baseIndexBottom);
        indices.append(baseIndexBottom + j + 1);
        indices.append(baseIndexBottom + j + 2);
    }

    // Заполняем буферы
    vertexBuffer.bind();
    vertexBuffer.allocate(vertices.constData(), vertices.size() * sizeof(VertexData));
    vertexBuffer.release();

    indexBuffer.bind();
    indexBuffer.allocate(indices.constData(), indices.size() * sizeof(GLuint));
    indexBuffer.release();
}

void Cylinder::render(QOpenGLShaderProgram& program)
{
    program.setUniformValue("model", modelMatrix);

    QOpenGLTexture* texture = TextureManager::instance().getTexture(textureName);
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
    if (texture) texture->release();
}

QVector<VertexData> Cylinder::getVertices() const {
    return vertexes;
}

float Cylinder::getRadius() const {
    return radius;
}

float Cylinder::getHeight() const {
    return height;
}

// void Cylinder::getAABB(QVector3D &min, QVector3D &max) const
// {
//     float scaledRadius = radius * qMax(scaleFactor.x(), scaleFactor.z());
//     float scaledHalfHeight = height * 0.5f * scaleFactor.y();

//     QVector3D localMin(-scaledRadius, -scaledHalfHeight, -scaledRadius);
//     QVector3D localMax(scaledRadius, scaledHalfHeight, scaledRadius);

//     QVector<QVector3D> keyPoints = {
//         QVector3D(0, -scaledHalfHeight, 0),          // низ центр
//         QVector3D(0, scaledHalfHeight, 0),           // верх центр
//         QVector3D(scaledRadius, -scaledHalfHeight, 0),   // низ право
//         QVector3D(-scaledRadius, -scaledHalfHeight, 0),  // низ лево
//         QVector3D(0, -scaledHalfHeight, scaledRadius),    // низ перед
//         QVector3D(0, -scaledHalfHeight, -scaledRadius),  // низ зад
//         QVector3D(scaledRadius, scaledHalfHeight, 0),    // верх право
//         QVector3D(-scaledRadius, scaledHalfHeight, 0),   // верх лево
//         QVector3D(0, scaledHalfHeight, scaledRadius),    // верх перед
//         QVector3D(0, scaledHalfHeight, -scaledRadius)    // верх зад
//     };

//     min = QVector3D(INFINITY, INFINITY, INFINITY);
//     max = QVector3D(-INFINITY, -INFINITY, -INFINITY);

//     for (const auto& point : keyPoints) {
//         QVector3D transformed = modelMatrix * point;
//         min.setX(qMin(min.x(), transformed.x()));
//         min.setY(qMin(min.y(), transformed.y()));
//         min.setZ(qMin(min.z(), transformed.z()));
//         max.setX(qMax(max.x(), transformed.x()));
//         max.setY(qMax(max.y(), transformed.y()));
//         max.setZ(qMax(max.z(), transformed.z()));
//     }
// }

// QVector<QVector3D> Cylinder::getTransformedVertices() const {
//     QVector<QVector3D> transformed;
//     for (const auto &vertex : vertexes) {
//         transformed.append(modelMatrix * vertex.position);
//     }
//     return transformed;
// }
