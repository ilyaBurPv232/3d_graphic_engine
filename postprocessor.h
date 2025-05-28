// postprocessor.h
#ifndef POSTPROCESSOR_H
#define POSTPROCESSOR_H

#include <QOpenGLFunctions>
#include <QOpenGLFramebufferObject>
#include <QOpenGLBuffer>
#include <QVector3D>
#include <QOpenGLShaderProgram>

class QOpenGLShaderProgram;

class PostProcessor : protected QOpenGLFunctions
{
public:
    PostProcessor();
    ~PostProcessor();

    void initialize(int width, int height);
    void resize(int width, int height);
    void beginRender();
    void endRender();
    void applyEffects(QOpenGLShaderProgram& program);

    void toggleEffects(bool enabled) { effectsEnabled = enabled; }
    void setVignetteEnabled(bool enabled) { useVignette = enabled; }
    void setFilmGrainEnabled(bool enabled) { useFilmGrain = enabled; }
    void setNegativeEnabled(bool enabled) { useNegative = enabled; }
    void setSepiaEnabled(bool enabled) { useSepia = enabled; }
    void setFisheyeEnabled(bool enabled) { useFisheye = enabled; }
    void setColorFilterEnabled(bool enabled) { useColorFilter = enabled; }
    void setGrayscaleEnabled(bool enabled) { useGrayscale = enabled; }

    void setVignetteIntensity(float intensity) { vignetteIntensity = intensity; }
    void setFilmGrainIntensity(float intensity) { filmGrainIntensity = intensity; }
    void setFisheyeStrength(float strength) { fisheyeStrength = strength; }
    void setColorFilterColor(const QVector3D& color) { colorFilterColor = color; }
    QVector3D getColorFilterColor() const { return colorFilterColor; }

    bool areEffectsEnabled() const { return effectsEnabled; }

private:
    void setupScreenQuad();

    QOpenGLFramebufferObject* fbo;
    QOpenGLBuffer quadVBO;
    int screenWidth;
    int screenHeight;

    bool effectsEnabled = true;
    bool useVignette = false;
    bool useFilmGrain = false;
    bool useNegative = false;
    bool useSepia = false;
    bool useFisheye = false;
    bool useColorFilter = false;
    bool useGrayscale = false;

    float vignetteIntensity = 0.5f;
    float filmGrainIntensity = 0.1f;
    float fisheyeStrength = 0.5f;
    QVector3D colorFilterColor = QVector3D(1.0f, 1.0f, 1.0f);
};

#endif // POSTPROCESSOR_H
