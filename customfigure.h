#ifndef CUSTOMFIGURE_H
#define CUSTOMFIGURE_H

#include "shape.h"

class CustomFigure : public Shape
{
public:
    CustomFigure(const QString& textureName = "default",
                 int topSides = 8,
                 int bottomSides = 6,
                 int centerSides = 6);
    ~CustomFigure() = default;

    void initialize() override;
    void render(QOpenGLShaderProgram& program) override;

    // Настройки фигуры
    void setTopSides(int sides);
    void setBottomSides(int sides);
    void setCenterSides(int sides);

    int getTopSides() const { return m_topSides; }
    int getBottomSides() const { return m_bottomSides; }
    int getCenterSides() const { return m_centerSides; }

private:
    void setupVertices();
    void createTopPolygon();
    void createBottomPolygon();
    void createSideFaces();
    void createCenterFigure();

    QString m_textureName;
    int m_topSides;     // Количество сторон верхнего многоугольника
    int m_bottomSides;  // Количество сторон нижнего многоугольника
    int m_centerSides;  // Количество сторон центральной фигуры

    // Геометрические параметры
    float m_topRadius = 1.0f;
    float m_topHeight = 1.5f;
    float m_bottomRadius = 1.5f;
    float m_bottomHeight = 0.0f;
    float m_centerRadius = 0.5f;
    float m_centerTopHeight = 0.0f;
    float m_centerBottomHeight = -1.5f;

    // Временные буферы для построения
    QVector<VertexData> m_vertices;
    QVector<GLuint> m_indices;
};

#endif // CUSTOMFIGURE_H
