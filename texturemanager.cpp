#include "texturemanager.h"
#include <QDebug>


TextureManager& TextureManager::instance()
{
    static TextureManager instance;
    return instance;
}

TextureManager::~TextureManager()
{
    clear();
}

QOpenGLTexture* TextureManager::loadTexture(const QString& path, const QString& name)
{
    if (textures.contains(name)) {
        qWarning() << "Texture" << name << "already loaded";
        return textures[name];
    }

    QImage image(path);
    if (image.isNull()) {
        qWarning() << "Failed to load texture:" << path;
        return nullptr;
    }

    QOpenGLTexture* texture = new QOpenGLTexture(image.flipped());
    texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    texture->setMagnificationFilter(QOpenGLTexture::Linear);
    texture->setWrapMode(QOpenGLTexture::Repeat);

    textures[name] = texture;
    return texture;
}

QOpenGLTexture* TextureManager::getTexture(const QString& name) const
{
    return textures.value(name, nullptr);
}

bool TextureManager::contains(const QString& name) const
{
    return textures.contains(name);
}

void TextureManager::removeTexture(const QString& name)
{
    if (textures.contains(name)) {
        delete textures[name];
        textures.remove(name);
    }
}

void TextureManager::clear() {
    if (QOpenGLContext::currentContext()) {
        qDeleteAll(textures);
        textures.clear();
    }
}
