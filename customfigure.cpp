#include "customfigure.h"
#include "texturemanager.h"
#include <QVector2D>
#include <QVector3D>
#include <cmath>

CustomFigure::CustomFigure(const QString& textureName, int topSides, int bottomSides, int centerSides)
    : m_textureName(textureName)
    , m_topSides(topSides)
    , m_bottomSides(bottomSides)
    , m_centerSides(centerSides)
{
    setupVertices();
}

void CustomFigure::initialize()
{
    if (!TextureManager::instance().contains(m_textureName)) {
        TextureManager::instance().loadTexture(":/textures/default.png", m_textureName);
    }
}

void CustomFigure::setupVertices()
{
    m_vertices.clear();
    m_indices.clear();

    // Создаем все части фигуры
    createTopPolygon();
    createBottomPolygon();
    createSideFaces();
    createCenterFigure();

    // Заполняем буферы
    vertexBuffer.create();
    vertexBuffer.bind();
    vertexBuffer.allocate(m_vertices.constData(), m_vertices.size() * sizeof(VertexData));
    vertexBuffer.release();

    indexBuffer.create();
    indexBuffer.bind();
    indexBuffer.allocate(m_indices.constData(), m_indices.size() * sizeof(GLuint));
    indexBuffer.release();
}

void CustomFigure::createTopPolygon()
{
    const float PI = 3.14159265358979323846f;

    // Центральная вершина верхнего многоугольника
    int centerIndex = m_vertices.size();
    m_vertices.append(VertexData(
        QVector3D(0.0f, m_topHeight, 0.0f),
        QVector2D(0.5f, 0.5f),
        QVector3D(0.0f, 1.0f, 0.0f)
        ));

    // Вершины верхнего многоугольника
    QVector<int> topVertexIndices;
    for (int i = 0; i < m_topSides; ++i) {
        float angle = 2 * PI * i / m_topSides;
        float x = m_topRadius * cosf(angle);
        float z = m_topRadius * sinf(angle);

        topVertexIndices.append(m_vertices.size());
        m_vertices.append(VertexData(
            QVector3D(x, m_topHeight, z),
            QVector2D(0.5f + 0.5f * cosf(angle), 0.5f + 0.5f * sinf(angle)),
            QVector3D(0.0f, 1.0f, 0.0f)
            ));
    }

    // Треугольники верхней грани
    for (int i = 0; i < m_topSides; ++i) {
        int next = (i + 1) % m_topSides;
        m_indices.append(centerIndex);
        m_indices.append(topVertexIndices[i]);
        m_indices.append(topVertexIndices[next]);
    }
}

void CustomFigure::createBottomPolygon()
{
    const float PI = 3.14159265358979323846f;

    // Центральная вершина нижнего многоугольника
    int centerIndex = m_vertices.size();
    m_vertices.append(VertexData(
        QVector3D(0.0f, m_bottomHeight, 0.0f),
        QVector2D(0.5f, 0.5f),
        QVector3D(0.0f, -1.0f, 0.0f)
        ));

    // Вершины нижнего многоугольника
    QVector<int> bottomVertexIndices;
    for (int i = 0; i < m_bottomSides; ++i) {
        float angle = 2 * PI * i / m_bottomSides;
        float x = m_bottomRadius * cosf(angle);
        float z = m_bottomRadius * sinf(angle);

        bottomVertexIndices.append(m_vertices.size());
        m_vertices.append(VertexData(
            QVector3D(x, m_bottomHeight, z),
            QVector2D(0.5f + 0.5f * cosf(angle), 0.5f + 0.5f * sinf(angle)),
            QVector3D(0.0f, -1.0f, 0.0f)
            ));
    }

    // Треугольники нижней грани
    for (int i = 0; i < m_bottomSides; ++i) {
        int next = (i + 1) % m_bottomSides;
        m_indices.append(centerIndex);
        m_indices.append(bottomVertexIndices[i]);
        m_indices.append(bottomVertexIndices[next]);
    }
}

void CustomFigure::createSideFaces()
{
    const float PI = 3.14159265358979323846f;

    // Создаем вершины верхнего и нижнего многоугольников (без центров)
    QVector<QVector3D> topVertices;
    QVector<QVector3D> bottomVertices;

    for (int i = 0; i < m_topSides; ++i) {
        float angle = 2 * PI * i / m_topSides;
        topVertices.append(QVector3D(
            m_topRadius * cosf(angle),
            m_topHeight,
            m_topRadius * sinf(angle)
            ));
    }

    for (int i = 0; i < m_bottomSides; ++i) {
        float angle = 2 * PI * i / m_bottomSides;
        bottomVertices.append(QVector3D(
            m_bottomRadius * cosf(angle),
            m_bottomHeight,
            m_bottomRadius * sinf(angle)
            ));
    }

    // Создаем боковые грани (четырехугольники, разбитые на треугольники)
    for (int i = 0; i < m_topSides; ++i) {
        int nextTop = (i + 1) % m_topSides;

        // Находим соответствующие индексы на нижнем многоугольнике
        float ratio = (float)m_bottomSides / m_topSides;
        int bottomIdx1 = (int)(i * ratio) % m_bottomSides;
        int bottomIdx2 = (int)(nextTop * ratio) % m_bottomSides;

        if (bottomIdx1 == bottomIdx2) {
            bottomIdx2 = (bottomIdx1 + 1) % m_bottomSides;
        }

        QVector3D top1 = topVertices[i];
        QVector3D top2 = topVertices[nextTop];
        QVector3D bottom1 = bottomVertices[bottomIdx1];
        QVector3D bottom2 = bottomVertices[bottomIdx2];

        // Вычисляем нормали для боковых граней
        QVector3D normal1 = QVector3D::crossProduct(bottom1 - top1, bottom2 - top1).normalized();
        QVector3D normal2 = QVector3D::crossProduct(bottom2 - top1, top2 - top1).normalized();

        // Первый треугольник
        int baseIndex = m_vertices.size();
        m_vertices.append(VertexData(top1, QVector2D(0.0f, 0.0f), normal1));
        m_vertices.append(VertexData(bottom1, QVector2D(0.0f, 1.0f), normal1));
        m_vertices.append(VertexData(bottom2, QVector2D(1.0f, 1.0f), normal1));

        m_indices.append(baseIndex);
        m_indices.append(baseIndex + 1);
        m_indices.append(baseIndex + 2);

        // Второй треугольник
        baseIndex = m_vertices.size();
        m_vertices.append(VertexData(top1, QVector2D(0.0f, 0.0f), normal2));
        m_vertices.append(VertexData(bottom2, QVector2D(1.0f, 1.0f), normal2));
        m_vertices.append(VertexData(top2, QVector2D(1.0f, 0.0f), normal2));

        m_indices.append(baseIndex);
        m_indices.append(baseIndex + 1);
        m_indices.append(baseIndex + 2);
    }
}

void CustomFigure::createCenterFigure()
{
    const float PI = 3.14159265358979323846f;

    // Центральная верхняя вершина
    int centerTopIndex = m_vertices.size();
    m_vertices.append(VertexData(
        QVector3D(0.0f, m_centerTopHeight, 0.0f),
        QVector2D(0.5f, 0.5f),
        QVector3D(0.0f, 1.0f, 0.0f)
        ));

    // Вершины верхнего центрального многоугольника
    QVector<int> centerTopVertexIndices;
    for (int i = 0; i < m_centerSides; ++i) {
        float angle = 2 * PI * i / m_centerSides;
        float x = m_centerRadius * cosf(angle);
        float z = m_centerRadius * sinf(angle);

        centerTopVertexIndices.append(m_vertices.size());
        m_vertices.append(VertexData(
            QVector3D(x, m_centerTopHeight, z),
            QVector2D(0.5f + 0.5f * cosf(angle), 0.5f + 0.5f * sinf(angle)),
            QVector3D(0.0f, 1.0f, 0.0f)
            ));
    }

    // Треугольники верхней центральной грани
    for (int i = 0; i < m_centerSides; ++i) {
        int next = (i + 1) % m_centerSides;
        m_indices.append(centerTopIndex);
        m_indices.append(centerTopVertexIndices[i]);
        m_indices.append(centerTopVertexIndices[next]);
    }

    // Центральная нижняя вершина
    int centerBottomIndex = m_vertices.size();
    m_vertices.append(VertexData(
        QVector3D(0.0f, m_centerBottomHeight - 0.5f, 0.0f),
        QVector2D(0.5f, 0.5f),
        QVector3D(0.0f, -1.0f, 0.0f)
        ));

    // Вершины нижнего центрального многоугольника
    QVector<int> centerBottomVertexIndices;
    for (int i = 0; i < m_centerSides; ++i) {
        float angle = 2 * PI * i / m_centerSides;
        float x = m_centerRadius * cosf(angle);
        float z = m_centerRadius * sinf(angle);

        centerBottomVertexIndices.append(m_vertices.size());
        m_vertices.append(VertexData(
            QVector3D(x, m_centerBottomHeight, z),
            QVector2D(0.5f + 0.5f * cosf(angle), 0.5f + 0.5f * sinf(angle)),
            QVector3D(0.0f, -1.0f, 0.0f)
            ));
    }

    // Треугольники нижней центральной грани
    for (int i = 0; i < m_centerSides; ++i) {
        int next = (i + 1) % m_centerSides;
        m_indices.append(centerBottomIndex);
        m_indices.append(centerBottomVertexIndices[i]);
        m_indices.append(centerBottomVertexIndices[next]);
    }

    // Боковые грани центральной фигуры
    for (int i = 0; i < m_centerSides; ++i) {
        int next = (i + 1) % m_centerSides;

        int topIdx = centerTopVertexIndices[i];
        int topNext = centerTopVertexIndices[next];
        int bottomIdx = centerBottomVertexIndices[i];
        int bottomNext = centerBottomVertexIndices[next];

        // Вычисляем нормали для боковых граней
        QVector3D normal1 = QVector3D::crossProduct(
                                m_vertices[bottomIdx].position - m_vertices[topIdx].position,
                                m_vertices[bottomNext].position - m_vertices[topIdx].position
                                ).normalized();

        QVector3D normal2 = QVector3D::crossProduct(
                                m_vertices[bottomNext].position - m_vertices[topIdx].position,
                                m_vertices[topNext].position - m_vertices[topIdx].position
                                ).normalized();

        // Первый треугольник
        int baseIndex = m_vertices.size();
        m_vertices.append(VertexData(
            m_vertices[topIdx].position,
            QVector2D(0.0f, 0.0f),
            normal1
            ));
        m_vertices.append(VertexData(
            m_vertices[bottomIdx].position,
            QVector2D(0.0f, 1.0f),
            normal1
            ));
        m_vertices.append(VertexData(
            m_vertices[bottomNext].position,
            QVector2D(1.0f, 1.0f),
            normal1
            ));

        m_indices.append(baseIndex);
        m_indices.append(baseIndex + 1);
        m_indices.append(baseIndex + 2);

        // Второй треугольник
        baseIndex = m_vertices.size();
        m_vertices.append(VertexData(
            m_vertices[topIdx].position,
            QVector2D(0.0f, 0.0f),
            normal2
            ));
        m_vertices.append(VertexData(
            m_vertices[bottomNext].position,
            QVector2D(1.0f, 1.0f),
            normal2
            ));
        m_vertices.append(VertexData(
            m_vertices[topNext].position,
            QVector2D(1.0f, 0.0f),
            normal2
            ));

        m_indices.append(baseIndex);
        m_indices.append(baseIndex + 1);
        m_indices.append(baseIndex + 2);
    }
}

void CustomFigure::render(QOpenGLShaderProgram& program)
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

void CustomFigure::setTopSides(int sides)
{
    if (sides >= 3 && sides != m_topSides) {
        m_topSides = sides;
        setupVertices(); // Перестраиваем геометрию
    }
}

void CustomFigure::setBottomSides(int sides)
{
    if (sides >= 3 && sides != m_bottomSides) {
        m_bottomSides = sides;
        setupVertices();
    }
}

void CustomFigure::setCenterSides(int sides)
{
    if (sides >= 3 && sides != m_centerSides) {
        m_centerSides = sides;
        setupVertices();
    }
}
