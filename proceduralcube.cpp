#include "proceduralcube.h"
#include "shadermanager.h"
#include "texturemanager.h"
#include <QOpenGLTexture>
#include <QDebug>

ProceduralCube::ProceduralCube(const QString& textureName)
    : Cube(textureName)
{
}

void ProceduralCube::initialize()
{
    if (!TextureManager::instance().contains(textureName)) {
        TextureManager::instance().loadTexture(":/textures/default.png", textureName);
    }

    if (!ShaderManager::instance().getShaderProgram("procedural")) {
        if (!ShaderManager::instance().loadShaderProgram("procedural",
                                                         ":/shaders/vshader_procedural.vsh",
                                                         ":/shaders/fshader_procedural.fsh")) {
            qWarning() << "Failed to load procedural shader program";
        }
    }
}

void ProceduralCube::render(QOpenGLShaderProgram& program)
{
    QOpenGLShaderProgram* proceduralProgram = ShaderManager::instance().getShaderProgram("procedural");
    if (!proceduralProgram) {
        // Если процедурный шейдер не загружен, используем стандартный рендер куба
        Cube::render(program);
        return;
    }

    // Запоминаем, был ли связан основной шейдер
    bool wasBound = program.isLinked();
    if (wasBound) {
        program.release();
    }

    // Связываем процедурный шейдер
    proceduralProgram->bind();

    // Передаем матрицу модели
    proceduralProgram->setUniformValue("model", modelMatrix);

    // Привязываем текстуру
    QOpenGLTexture* texture = TextureManager::instance().getTexture(textureName);
    if (texture) {
        texture->bind();
        proceduralProgram->setUniformValue("ourTexture", 0);
    }

    // Передаем параметры букв;
    proceduralProgram->setUniformValue("letterScale", m_letterScale);
    proceduralProgram->setUniformValue("letterIntensity", m_letterIntensity);
    proceduralProgram->setUniformValue("letterColorB", m_letterColorB);
    proceduralProgram->setUniformValue("letterColorP", m_letterColorP);


    // Настраиваем атрибуты вершин
    vertexBuffer.bind();
    indexBuffer.bind();

    int posLoc = proceduralProgram->attributeLocation("aPos");
    if (posLoc != -1) {
        proceduralProgram->enableAttributeArray(posLoc);
        proceduralProgram->setAttributeBuffer(posLoc, GL_FLOAT, offsetof(VertexData, position), 3, sizeof(VertexData));
    }

    int texLoc = proceduralProgram->attributeLocation("aTexCoord");
    if (texLoc != -1) {
        proceduralProgram->enableAttributeArray(texLoc);
        proceduralProgram->setAttributeBuffer(texLoc, GL_FLOAT, offsetof(VertexData, texCoord), 2, sizeof(VertexData));
    }

    int normalLoc = proceduralProgram->attributeLocation("aNormal");
    if (normalLoc != -1) {
        proceduralProgram->enableAttributeArray(normalLoc);
        proceduralProgram->setAttributeBuffer(normalLoc, GL_FLOAT, offsetof(VertexData, normal), 3, sizeof(VertexData));
    }

    // Отрисовываем куб
    glDrawElements(GL_TRIANGLES, indexBuffer.size(), GL_UNSIGNED_INT, 0);

    // Отключаем атрибуты
    if (posLoc != -1) {
        proceduralProgram->disableAttributeArray(posLoc);
    }
    if (texLoc != -1) {
        proceduralProgram->disableAttributeArray(texLoc);
    }
    if (normalLoc != -1) {
        proceduralProgram->disableAttributeArray(normalLoc);
    }

    // Отвязываем буферы
    vertexBuffer.release();
    indexBuffer.release();

    // Отвязываем текстуру
    if (texture) {
        texture->release();
    }

    // Отвязываем шейдер
    proceduralProgram->release();

    // Если основной шейдер был связан, восстанавливаем его
    if (wasBound) {
        program.bind();
    }
}
