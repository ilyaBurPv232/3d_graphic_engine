#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include <QMap>
#include <QOpenGLShaderProgram>
#include <QString>

class ShaderManager
{
public:
    static ShaderManager &instance();

    ShaderManager(const ShaderManager &) = delete;
    ShaderManager &operator=(const ShaderManager &) = delete;

    bool loadShaderProgram(const QString &name,
                           const QString &vertexShaderPath,
                           const QString &fragmentShaderPath);

    QOpenGLShaderProgram *getShaderProgram(const QString &name);

    void cleanup();

private:
    ShaderManager() = default;
    ~ShaderManager() = default;

    QMap<QString, QOpenGLShaderProgram *> m_shaderPrograms;
};

#endif // SHADERMANAGER_H
