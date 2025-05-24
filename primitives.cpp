#include "primitives.h"
#include "texturemanager.h"

#include <QDebug>
#include <QOpenGLTexture>
#include <cmath>

Cube::Cube(const QString& textureName) : textureName(textureName) {
    setupVertices();
}

void Cube::initialize() {
    if (!TextureManager::instance().contains(textureName)) {
        TextureManager::instance().loadTexture(":/textures/grass.png", textureName);
    }
}

void Cube::setupVertices() {
    float w_div_2 = 0.5f;
    QVector<VertexData> vertexes;

    vertexes.append(VertexData(QVector3D(-w_div_2, w_div_2, w_div_2),  // верх-лево
                               QVector2D(0.0f, 1.0f),
                               QVector3D(0.0f, 0.0f, 1.0f)));
    vertexes.append(VertexData(QVector3D(-w_div_2, -w_div_2, w_div_2), // низ-лево
                               QVector2D(0.0f, 0.0f),
                               QVector3D(0.0f, 0.0f, 1.0f)));
    vertexes.append(VertexData(QVector3D(w_div_2, w_div_2, w_div_2),    // верх-право
                               QVector2D(1.0f, 1.0f),
                               QVector3D(0.0f, 0.0f, 1.0f)));
    vertexes.append(VertexData(QVector3D(w_div_2, -w_div_2, w_div_2),   // низ-право
                               QVector2D(1.0f, 0.0f),
                               QVector3D(0.0f, 0.0f, 1.0f)));

    // Задняя грань (Z-)
    vertexes.append(VertexData(QVector3D(w_div_2, w_div_2, -w_div_2),   // верх-право
                               QVector2D(1.0f, 1.0f),
                               QVector3D(0.0f, 0.0f, -1.0f)));
    vertexes.append(VertexData(QVector3D(w_div_2, -w_div_2, -w_div_2),  // низ-право
                               QVector2D(1.0f, 0.0f),
                               QVector3D(0.0f, 0.0f, -1.0f)));
    vertexes.append(VertexData(QVector3D(-w_div_2, w_div_2, -w_div_2),  // верх-лево
                               QVector2D(0.0f, 1.0f),
                               QVector3D(0.0f, 0.0f, -1.0f)));
    vertexes.append(VertexData(QVector3D(-w_div_2, -w_div_2, -w_div_2), // низ-лево
                               QVector2D(0.0f, 0.0f),
                               QVector3D(0.0f, 0.0f, -1.0f)));

    // Левая грань (X-)
    vertexes.append(VertexData(QVector3D(-w_div_2, w_div_2, -w_div_2),  // верх-зад
                               QVector2D(0.0f, 1.0f),
                               QVector3D(-1.0f, 0.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(-w_div_2, -w_div_2, -w_div_2), // низ-зад
                               QVector2D(0.0f, 0.0f),
                               QVector3D(-1.0f, 0.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(-w_div_2, w_div_2, w_div_2),   // верх-перед
                               QVector2D(1.0f, 1.0f),
                               QVector3D(-1.0f, 0.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(-w_div_2, -w_div_2, w_div_2),  // низ-перед
                               QVector2D(1.0f, 0.0f),
                               QVector3D(-1.0f, 0.0f, 0.0f)));

    // Правая грань (X+)
    vertexes.append(VertexData(QVector3D(w_div_2, w_div_2, w_div_2),    // верх-перед
                               QVector2D(0.0f, 1.0f),
                               QVector3D(1.0f, 0.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(w_div_2, -w_div_2, w_div_2),   // низ-перед
                               QVector2D(0.0f, 0.0f),
                               QVector3D(1.0f, 0.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(w_div_2, w_div_2, -w_div_2),   // верх-зад
                               QVector2D(1.0f, 1.0f),
                               QVector3D(1.0f, 0.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(w_div_2, -w_div_2, -w_div_2),  // низ-зад
                               QVector2D(1.0f, 0.0f),
                               QVector3D(1.0f, 0.0f, 0.0f)));

    // Верхняя грань (Y+)
    vertexes.append(VertexData(QVector3D(-w_div_2, w_div_2, -w_div_2),  // зад-лево
                               QVector2D(0.0f, 1.0f),
                               QVector3D(0.0f, 1.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(-w_div_2, w_div_2, w_div_2),   // перед-лево
                               QVector2D(0.0f, 0.0f),
                               QVector3D(0.0f, 1.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(w_div_2, w_div_2, -w_div_2),   // зад-право
                               QVector2D(1.0f, 1.0f),
                               QVector3D(0.0f, 1.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(w_div_2, w_div_2, w_div_2),    // перед-право
                               QVector2D(1.0f, 0.0f),
                               QVector3D(0.0f, 1.0f, 0.0f)));

    // Нижняя грань (Y-)
    vertexes.append(VertexData(QVector3D(-w_div_2, -w_div_2, w_div_2),  // перед-лево
                               QVector2D(0.0f, 1.0f),
                               QVector3D(0.0f, -1.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(-w_div_2, -w_div_2, -w_div_2), // зад-лево
                               QVector2D(0.0f, 0.0f),
                               QVector3D(0.0f, -1.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(w_div_2, -w_div_2, w_div_2),   // перед-право
                               QVector2D(1.0f, 1.0f),
                               QVector3D(0.0f, -1.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(w_div_2, -w_div_2, -w_div_2),  // зад-право
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

void Cube::render(QOpenGLShaderProgram& program) {
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

Pyramid::Pyramid(const QString& textureName) : textureName(textureName)
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
    QVector<VertexData> vertexes;

    // Основание пирамиды (4 вершины)
    vertexes.append(VertexData(QVector3D(-0.5f, -0.5f, -0.5f), QVector2D(0.0f, 0.0f), QVector3D(0.0f, -1.0f, 0.0f)));  // 0
    vertexes.append(VertexData(QVector3D( 0.5f, -0.5f, -0.5f), QVector2D(1.0f, 0.0f), QVector3D(0.0f, -1.0f, 0.0f)));  // 1
    vertexes.append(VertexData(QVector3D( 0.5f, -0.5f,  0.5f), QVector2D(1.0f, 1.0f), QVector3D(0.0f, -1.0f, 0.0f)));  // 2
    vertexes.append(VertexData(QVector3D(-0.5f, -0.5f,  0.5f), QVector2D(0.0f, 1.0f), QVector3D(0.0f, -1.0f, 0.0f)));  // 3

    // Вершина пирамиды (центр верхней точки)
    const QVector3D top(0.0f, 0.5f, 0.0f);

    // Передняя грань (Z+)
    QVector3D frontNormal = QVector3D::crossProduct(
                                QVector3D(-0.5f, -0.5f, 0.5f) - top,
                                QVector3D(0.5f, -0.5f, 0.5f) - top
                                ).normalized();

    vertexes.append(VertexData(top, QVector2D(0.5f, 1.0f), frontNormal));  // 4
    vertexes.append(VertexData(QVector3D(-0.5f, -0.5f, 0.5f), QVector2D(0.0f, 0.0f), frontNormal));  // 5
    vertexes.append(VertexData(QVector3D(0.5f, -0.5f, 0.5f), QVector2D(1.0f, 0.0f), frontNormal));   // 6

    // Правая грань (X+)
    QVector3D rightNormal = QVector3D::crossProduct(
                                QVector3D(0.5f, -0.5f, 0.5f) - top,
                                QVector3D(0.5f, -0.5f, -0.5f) - top
                                ).normalized();

    vertexes.append(VertexData(top, QVector2D(0.5f, 1.0f), rightNormal));  // 7
    vertexes.append(VertexData(QVector3D(0.5f, -0.5f, 0.5f), QVector2D(0.0f, 0.0f), rightNormal));  // 8
    vertexes.append(VertexData(QVector3D(0.5f, -0.5f, -0.5f), QVector2D(1.0f, 0.0f), rightNormal)); // 9

    // Задняя грань (Z-)
    QVector3D backNormal = QVector3D::crossProduct(
                               QVector3D(0.5f, -0.5f, -0.5f) - top,
                               QVector3D(-0.5f, -0.5f, -0.5f) - top
                               ).normalized();

    vertexes.append(VertexData(top, QVector2D(0.5f, 1.0f), backNormal));   // 10
    vertexes.append(VertexData(QVector3D(0.5f, -0.5f, -0.5f), QVector2D(0.0f, 0.0f), backNormal));  // 11
    vertexes.append(VertexData(QVector3D(-0.5f, -0.5f, -0.5f), QVector2D(1.0f, 0.0f), backNormal)); // 12

    // Левая грань (X-)
    QVector3D leftNormal = QVector3D::crossProduct(
                               QVector3D(-0.5f, -0.5f, -0.5f) - top,
                               QVector3D(-0.5f, -0.5f, 0.5f) - top
                               ).normalized();

    vertexes.append(VertexData(top, QVector2D(0.5f, 1.0f), leftNormal));  // 13
    vertexes.append(VertexData(QVector3D(-0.5f, -0.5f, -0.5f), QVector2D(0.0f, 0.0f), leftNormal)); // 14
    vertexes.append(VertexData(QVector3D(-0.5f, -0.5f, 0.5f), QVector2D(1.0f, 0.0f), leftNormal));  // 15

    // Индексы для треугольников
    QVector<GLuint> indexes = {
        // Основание (2 треугольника)
        0, 1, 2,
        2, 3, 0,

        // Передняя грань
        4, 5, 6,

        // Правая грань
        7, 8, 9,

        // Задняя грань
        10, 11, 12,

        // Левая грань
        13, 14, 15
    };

    vertexBuffer.create();
    vertexBuffer.bind();
    vertexBuffer.allocate(vertexes.constData(), vertexes.size() * sizeof(VertexData));
    vertexBuffer.release();

    indexBuffer.create();
    indexBuffer.bind();
    indexBuffer.allocate(indexes.constData(), indexes.size() * sizeof(GLuint));
    indexBuffer.release();
}

void Pyramid::render(QOpenGLShaderProgram& program)
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

    glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);

    vertexBuffer.release();
    indexBuffer.release();
    if (texture) texture->release();
}


Sphere::Sphere(const QString& textureName, float radius, int sectors, int stacks)
    : textureName(textureName), radius(radius), sectors(sectors), stacks(stacks)
{
    setupVertices();
}

void Sphere::initialize()
{
    if (!TextureManager::instance().contains(textureName)) {
        TextureManager::instance().loadTexture(":/textures/default.png", textureName);
    }
}

void Sphere::setupVertices()
{
    QVector<VertexData> vertices;
    QVector<GLuint> indices;

    const float PI = 3.14159265358979323846f;
    float x, y, z, xy;
    float nx, ny, nz, lengthInv = 1.0f / radius;
    float s, t;

    float sectorStep = 2 * PI / sectors;
    float stackStep = PI / stacks;
    float sectorAngle, stackAngle;

    for(int i = 0; i <= stacks; ++i)
    {
        stackAngle = PI / 2 - i * stackStep;
        xy = radius * cosf(stackAngle);
        z = radius * sinf(stackAngle);

        for(int j = 0; j <= sectors; ++j)
        {
            sectorAngle = j * sectorStep;

            x = xy * cosf(sectorAngle);
            y = xy * sinf(sectorAngle);

            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;

            s = (float)j / sectors;
            t = (float)i / stacks;

            vertices.append(VertexData(
                QVector3D(x, y, z),
                QVector2D(s, t),
                QVector3D(nx, ny, nz)
                ));
        }
    }

    int k1, k2;
    for(int i = 0; i < stacks; ++i)
    {
        k1 = i * (sectors + 1);
        k2 = k1 + sectors + 1;

        for(int j = 0; j < sectors; ++j, ++k1, ++k2)
        {
            if(i != 0)
            {
                indices.append(k1);
                indices.append(k2);
                indices.append(k1 + 1);
            }

            if(i != (stacks-1))
            {
                indices.append(k1 + 1);
                indices.append(k2);
                indices.append(k2 + 1);
            }
        }
    }

    vertexBuffer.create();
    vertexBuffer.bind();
    vertexBuffer.allocate(vertices.constData(), vertices.size() * sizeof(VertexData));
    vertexBuffer.release();

    indexBuffer.create();
    indexBuffer.bind();
    indexBuffer.allocate(indices.constData(), indices.size() * sizeof(GLuint));
    indexBuffer.release();
}

void Sphere::render(QOpenGLShaderProgram& program)
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
