#ifndef SCENE_H
#define SCENE_H

#include <QVector>
#include <QMatrix4x4>
#include <QVector3D>
#include "shape.h"
#include "skybox.h"

class Scene
{
public:
    Scene();
    ~Scene();


    void addShape(Shape* shape);
    void renderAll(QOpenGLShaderProgram& program);

    void setSkybox(Skybox* skybox_new);
    void renderSkybox(QOpenGLShaderProgram& program);

    QMatrix4x4 getViewMatrix() const;
    QMatrix4x4 getProjectionMatrix() const;

    void setCameraPosition(const QVector3D& position);
    void setCameraFront(const QVector3D& front);
    void setCameraUp(const QVector3D& up);
    void resize(int w, int h);
    QVector3D getCameraPosition() const { return cameraPos; }
    QVector3D getCameraFront() const { return cameraFront; }
    QVector3D getCameraUp() const { return cameraUp; }
    const QVector<Shape*>& getShapes() const { return shapes; }




private:
    QVector<Shape*> shapes;
    Skybox *skybox;
    QMatrix4x4 projection;
    QMatrix4x4 view;

    QVector3D cameraPos;
    QVector3D cameraFront;
    QVector3D cameraUp;


};

#endif // SCENE_H
