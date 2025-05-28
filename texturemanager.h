#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <QOpenGLTexture>
#include <QOpenGLContext>
#include <QString>
#include <QMap>

class TextureManager
{
public:
    static TextureManager& instance();

    QOpenGLTexture* loadTexture(const QString& path, const QString& name);
    QOpenGLTexture* getTexture(const QString& name) const;
    bool contains(const QString& name) const;
    void removeTexture(const QString& name);
    void clear();

private:
    TextureManager() = default;
    ~TextureManager();
    TextureManager(const TextureManager&) = delete;
    TextureManager& operator=(const TextureManager&) = delete;

    QMap<QString, QOpenGLTexture*> textures;
};

#endif // TEXTUREMANAGER_H
