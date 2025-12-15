// ====== C:\Users\Burcevi\Documents\3d_graphic_engine\proceduralcube.h ====== //

#ifndef PROCEDURALCUBE_H
#define PROCEDURALCUBE_H
#include "primitives.h"
#include <QOpenGLShaderProgram>
#include <QDateTime>

class ProceduralCube : public Cube
{
public:
    ProceduralCube(const QString& textureName = "default");
    ~ProceduralCube() = default;

    void initialize() override;
    void render(QOpenGLShaderProgram& program) override;

    void setLetterScale(float scale) { m_letterScale = scale; }
    void setLetterIntensity(float intensity) { m_letterIntensity = intensity; }
    void setLetterColorB(const QVector3D& color) { m_letterColorB = color; }
    void setLetterColorP(const QVector3D& color) { m_letterColorP = color; }
    void setDebugTime(bool debug) { m_debugTime = debug; } // Добавляем

    float getLetterScale() const { return m_letterScale; }
    float getLetterIntensity() const { return m_letterIntensity; }
    QVector3D getLetterColorB() const { return m_letterColorB; }
    QVector3D getLetterColorP() const { return m_letterColorP; }
    bool getDebugTime() const { return m_debugTime; } // Добавляем

private:
    float m_letterScale = 1.0f;
    float m_letterIntensity = 0.5f;
    bool m_debugTime = false; // Добавляем
    QVector3D m_letterColorB = QVector3D(50.0f/255.0f, 150.0f/255.0f, 255.0f/255.0f);
    QVector3D m_letterColorP = QVector3D(255.0f/255.0f, 100.0f/255.0f, 50.0f/255.0f);
};

#endif // PROCEDURALCUBE_H
