#include "postprocessor.h"

PostProcessor::PostProcessor()
    : fbo(nullptr), screenWidth(0), screenHeight(0)
{
    initializeOpenGLFunctions();
    quadVBO.create();
}

PostProcessor::~PostProcessor()
{
    delete fbo;
    quadVBO.destroy();
}

void PostProcessor::initialize(int width, int height) {
    screenWidth = width;
    screenHeight = height;

    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    format.setSamples(0);

    fbo = new QOpenGLFramebufferObject(width, height, format);

    if (!fbo->isValid()) {
        qWarning() << "FBO is not valid!";
    }

    setupScreenQuad();
}

void PostProcessor::resize(int width, int height)
{
    screenWidth = width;
    screenHeight = height;
    delete fbo;

    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    format.setSamples(0);

    fbo = new QOpenGLFramebufferObject(width, height, format);
}

void PostProcessor::beginRender()
{
    if (fbo) {
        fbo->bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}

void PostProcessor::endRender()
{
    if (fbo) {
        fbo->release();
    }
}

void PostProcessor::setupScreenQuad()
{
    float quadVertices[] = {
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
        1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
        1.0f, -1.0f,  1.0f, 0.0f,
        1.0f,  1.0f,  1.0f, 1.0f
    };

    quadVBO.bind();
    quadVBO.allocate(quadVertices, sizeof(quadVertices));
    quadVBO.release();
}

void PostProcessor::applyEffects(QOpenGLShaderProgram& program)
{
    if (!fbo || !effectsEnabled) {
        glDisable(GL_DEPTH_TEST);
        program.bind();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, fbo->texture());
        program.setUniformValue("screenTexture", 0);

        program.setUniformValue("useVignette", false);
        program.setUniformValue("useFilmGrain", false);
        program.setUniformValue("useNegative", false);
        program.setUniformValue("useSepia", false);
        program.setUniformValue("useFisheye", false);
        program.setUniformValue("useColorFilter", false);
        program.setUniformValue("useGrayscale", false);

        quadVBO.bind();
        program.enableAttributeArray(0);
        program.setAttributeBuffer(0, GL_FLOAT, 0, 2, 4 * sizeof(float));
        program.enableAttributeArray(1);
        program.setAttributeBuffer(1, GL_FLOAT, 2 * sizeof(float), 2, 4 * sizeof(float));
        glDrawArrays(GL_TRIANGLES, 0, 6);
        quadVBO.release();
        program.release();
        glEnable(GL_DEPTH_TEST);
        return;
    }

    glDisable(GL_DEPTH_TEST);
    program.bind();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fbo->texture());
    program.setUniformValue("screenTexture", 0);

    program.setUniformValue("useVignette", useVignette);
    program.setUniformValue("useFilmGrain", useFilmGrain);
    program.setUniformValue("useNegative", useNegative);
    program.setUniformValue("useSepia", useSepia);
    program.setUniformValue("useFisheye", useFisheye);
    program.setUniformValue("useColorFilter", useColorFilter);
    program.setUniformValue("useGrayscale", useGrayscale);

    program.setUniformValue("vignetteIntensity", vignetteIntensity);
    program.setUniformValue("filmGrainIntensity", filmGrainIntensity);
    program.setUniformValue("fisheyeStrength", fisheyeStrength);
    program.setUniformValue("colorFilterColor", colorFilterColor);

    quadVBO.bind();
    program.enableAttributeArray(0);
    program.setAttributeBuffer(0, GL_FLOAT, 0, 2, 4 * sizeof(float));
    program.enableAttributeArray(1);
    program.setAttributeBuffer(1, GL_FLOAT, 2 * sizeof(float), 2, 4 * sizeof(float));
    glDrawArrays(GL_TRIANGLES, 0, 6);
    quadVBO.release();
    program.release();
    glEnable(GL_DEPTH_TEST);
}
