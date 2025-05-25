#include "shadermanager.h"
#include <QDebug>
#include <QFile>
#include <QOpenGLShader>

ShaderManager &ShaderManager::instance()
{
    static ShaderManager instance;
    return instance;
}

bool ShaderManager::loadShaderProgram(const QString &name,
                                      const QString &vertexShaderPath,
                                      const QString &fragmentShaderPath)
{
    if (m_shaderPrograms.contains(name)) {
        return true;
    }

    QOpenGLShaderProgram *program = new QOpenGLShaderProgram();

    if (!program->addShaderFromSourceFile(QOpenGLShader::Vertex, vertexShaderPath)) {
        qWarning() << "Failed to compile vertex shader" << vertexShaderPath << ":"
                   << program->log();
        delete program;
        return false;
    }

    if (!program->addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentShaderPath)) {
        qWarning() << "Failed to compile fragment shader" << fragmentShaderPath << ":"
                   << program->log();
        delete program;
        return false;
    }

    if (!program->link()) {
        qWarning() << "Failed to link shader program:" << program->log();
        delete program;
        return false;
    }

    m_shaderPrograms[name] = program;
    return true;
}

QOpenGLShaderProgram *ShaderManager::getShaderProgram(const QString &name)
{
    return m_shaderPrograms.value(name, nullptr);
}

void ShaderManager::cleanup()
{
    qDeleteAll(m_shaderPrograms);
    m_shaderPrograms.clear();
}
