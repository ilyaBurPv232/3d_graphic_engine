#include "skybox.h"
#include "texturemanager.h"


Skybox::Skybox(const QString& textureName) : textureName(textureName) {
    setupVertices();
}

void Skybox::initialize() {
    if (!TextureManager::instance().contains(textureName)) {
        TextureManager::instance().loadTexture(":/skyboxes/default.jpg", textureName);
    }
}

void Skybox::setupVertices() {
    float w_div_2 = 0.5f;
    float border = 0.002f; // Небольшой отступ от краев текстуры
    QVector<VertexData> vertexes;

    // Позади нас
    vertexes.append(VertexData(QVector3D(-w_div_2, w_div_2, w_div_2),
                               QVector2D(1.0f + border, (2.0f / 3.0f) - border),
                               QVector3D(0.0, 0.0, -1.0)));
    vertexes.append(VertexData(QVector3D(-w_div_2, -w_div_2, w_div_2),
                               QVector2D(1.0f + border, (1.0f / 3.0f) + border),
                               QVector3D(0.0, 0.0, -1.0)));
    vertexes.append(VertexData(QVector3D(w_div_2, w_div_2, w_div_2),
                               QVector2D((3.0f / 4.0f) - border, (2.0f / 3.0f) - border),
                               QVector3D(0.0, 0.0, -1.0)));
    vertexes.append(VertexData(QVector3D(w_div_2, -w_div_2, w_div_2),
                               QVector2D((3.0f / 4.0f) - border, (1.0f / 3.0f) + border),
                               QVector3D(0.0, 0.0, -1.0)));

    // Справа от нас
    vertexes.append(VertexData(QVector3D(w_div_2, w_div_2, w_div_2),
                               QVector2D((3.0f / 4.0f) - border, (2.0f / 3.0f) - border),
                               QVector3D(-1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(w_div_2, -w_div_2, w_div_2),
                               QVector2D((3.0f / 4.0f) - border, (1.0f / 3.0f) + border),
                               QVector3D(-1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(w_div_2, w_div_2, -w_div_2),
                               QVector2D((2.0f / 4.0f) + border, (2.0f / 3.0f) - border),
                               QVector3D(-1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(w_div_2, -w_div_2, -w_div_2),
                               QVector2D((2.0f / 4.0f) + border, (1.0f / 3.0f) + border),
                               QVector3D(-1.0, 0.0, 0.0)));

    // Над нами
    vertexes.append(VertexData(QVector3D(w_div_2, w_div_2, w_div_2),
                               QVector2D((2.0f / 4.0f) - border, 1.0f - border),
                               QVector3D(0.0, -1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(w_div_2, w_div_2, -w_div_2),
                               QVector2D((2.0f / 4.0f) - border, (2.0f / 3.0f) + border),
                               QVector3D(0.0, -1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-w_div_2, w_div_2, w_div_2),
                               QVector2D((1.0f / 4.0f) + border, 1.0f - border),
                               QVector3D(0.0, -1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-w_div_2, w_div_2, -w_div_2),
                               QVector2D((1.0f / 4.0f) + border, (2.0f / 3.0f) + border),
                               QVector3D(0.0, -1.0, 0.0)));

    // Перед нами
    vertexes.append(VertexData(QVector3D(w_div_2, w_div_2, -w_div_2),
                               QVector2D((2.0f / 4.0f) - border, (2.0f / 3.0f) + border),
                               QVector3D(0.0, 0.0, 1.0)));
    vertexes.append(VertexData(QVector3D(w_div_2, -w_div_2, -w_div_2),
                               QVector2D((2.0f / 4.0f) - border, (1.0f / 3.0f) - border),
                               QVector3D(0.0, 0.0, 1.0)));
    vertexes.append(VertexData(QVector3D(-w_div_2, w_div_2, -w_div_2),
                               QVector2D((1.0f / 4.0f) + border, (2.0f / 3.0f) + border),
                               QVector3D(0.0, 0.0, 1.0)));
    vertexes.append(VertexData(QVector3D(-w_div_2, -w_div_2, -w_div_2),
                               QVector2D((1.0f / 4.0f) + border, (1.0f / 3.0f) - border),
                               QVector3D(0.0, 0.0, 1.0)));

    // Слева от нас
    vertexes.append(VertexData(QVector3D(-w_div_2, w_div_2, w_div_2),
                               QVector2D(-border, (2.0f / 3.0f) - border),
                               QVector3D(1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-w_div_2, w_div_2, -w_div_2),
                               QVector2D((1.0f / 4.0f) - border, (2.0f / 3.0f) - border),
                               QVector3D(1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-w_div_2, -w_div_2, w_div_2),
                               QVector2D(-border, (1.0f / 3.0f) + border),
                               QVector3D(1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-w_div_2, -w_div_2, -w_div_2),
                               QVector2D((1.0f / 4.0f) - border, (1.0f / 3.0f) + border),
                               QVector3D(1.0, 0.0, 0.0)));

    // Под нами
    vertexes.append(VertexData(QVector3D(-w_div_2, -w_div_2, w_div_2),
                               QVector2D((1.0f / 4.0f) + border, border),
                               QVector3D(0.0, 1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-w_div_2, -w_div_2, -w_div_2),
                               QVector2D((1.0f / 4.0f) + border, (1.0f / 3.0f) - border),
                               QVector3D(0.0, 1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(w_div_2, -w_div_2, w_div_2),
                               QVector2D((2.0f / 4.0f) - border, border),
                               QVector3D(0.0, 1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(w_div_2, -w_div_2, -w_div_2),
                               QVector2D((2.0f / 4.0f) - border, (1.0f / 3.0f) - border),
                               QVector3D(0.0, 1.0, 0.0)));

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

void Skybox::render(QOpenGLShaderProgram& program) {
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
