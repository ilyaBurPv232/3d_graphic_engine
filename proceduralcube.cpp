#include "proceduralcube.h"
#include "shadermanager.h"
#include "texturemanager.h"
#include <QOpenGLShaderProgram>
#include <QDateTime>

ProceduralCube::ProceduralCube(const QString& textureName)
    : Cube(textureName)
{
}

void ProceduralCube::initialize()
{
    // Загружаем текстуру как обычный куб
    if (!TextureManager::instance().contains(textureName)) {
        TextureManager::instance().loadTexture(":/textures/default.png", textureName);
    }

    // Загружаем специальный шейдер для процедурного куба
    if (!ShaderManager::instance().getShaderProgram("procedural")) {
        ShaderManager::instance().loadShaderProgram("procedural",
                                                    ":/shaders/vshader_procedural.vsh",
                                                    ":/shaders/fshader_procedural.fsh");
    }
}

void ProceduralCube::render(QOpenGLShaderProgram& program)
{
    // Используем специальный шейдер для процедурного куба
    QOpenGLShaderProgram* proceduralProgram = ShaderManager::instance().getShaderProgram("procedural");
    if (!proceduralProgram) {
        // Если шейдер не загружен, используем стандартный рендер куба
        Cube::render(program);
        return;
    }

    // Обновляем время для анимации
    m_time = QDateTime::currentMSecsSinceEpoch() / 1000.0f;

    // Переключаемся на процедурный шейдер
    bool wasBound = program.isLinked();
    if (wasBound) {
        program.release();
    }

    proceduralProgram->bind();

    // Устанавливаем матрицу модели
    proceduralProgram->setUniformValue("model", modelMatrix);

    // Привязываем текстуру
    QOpenGLTexture* texture = TextureManager::instance().getTexture(textureName);
    if (texture) {
        texture->bind();
        proceduralProgram->setUniformValue("ourTexture", 0);
    }

    // Устанавливаем параметры процедурного эффекта
    proceduralProgram->setUniformValue("time", m_time);
    proceduralProgram->setUniformValue("letterScale", m_letterScale);
    proceduralProgram->setUniformValue("letterIntensity", m_letterIntensity);
    proceduralProgram->setUniformValue("letterColorB", m_letterColorB);
    proceduralProgram->setUniformValue("letterColorP", m_letterColorP);

    // Привязываем буферы вершин
    vertexBuffer.bind();
    indexBuffer.bind();

    // Устанавливаем атрибуты вершин
    int posLoc = proceduralProgram->attributeLocation("aPos");
    proceduralProgram->enableAttributeArray(posLoc);
    proceduralProgram->setAttributeBuffer(posLoc, GL_FLOAT, offsetof(VertexData, position), 3, sizeof(VertexData));

    int texLoc = proceduralProgram->attributeLocation("aTexCoord");
    proceduralProgram->enableAttributeArray(texLoc);
    proceduralProgram->setAttributeBuffer(texLoc, GL_FLOAT, offsetof(VertexData, texCoord), 2, sizeof(VertexData));

    int normalLoc = proceduralProgram->attributeLocation("aNormal");
    if (normalLoc != -1) {
        proceduralProgram->enableAttributeArray(normalLoc);
        proceduralProgram->setAttributeBuffer(normalLoc, GL_FLOAT, offsetof(VertexData, normal), 3, sizeof(VertexData));
    }

    // Рисуем
    glDrawElements(GL_TRIANGLES, indexBuffer.size(), GL_UNSIGNED_INT, 0);

    // Отключаем атрибуты
    proceduralProgram->disableAttributeArray(posLoc);
    proceduralProgram->disableAttributeArray(texLoc);
    if (normalLoc != -1) {
        proceduralProgram->disableAttributeArray(normalLoc);
    }

    // Освобождаем буферы
    vertexBuffer.release();
    indexBuffer.release();

    // Освобождаем текстуру
    if (texture) {
        texture->release();
    }

    proceduralProgram->release();

    // Возвращаемся к исходному шейдеру
    if (wasBound) {
        program.bind();
    }
}
