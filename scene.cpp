#include "scene.h"
#include "physicalworld.h"

Scene::Scene()
{
    cameraPos = QVector3D(0, 0, 5);
    cameraFront = QVector3D(0, 0, -1);
    cameraUp = QVector3D(0, 1, 0);
}

Scene::~Scene() {
    qDeleteAll(shapes);
}

void Scene::addShape(Shape* shape) {
    shapes.append(shape);
}

void Scene::renderAll(QOpenGLShaderProgram& program) {
    program.bind();

    view.setToIdentity();
    view.lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

    program.setUniformValue("view", view);
    program.setUniformValue("projection", projection);

    for (Shape* shape : shapes) {
        shape->render(program);
    }
    program.release();
}

QMatrix4x4 Scene::getViewMatrix() const {
    return view;
}

QMatrix4x4 Scene::getProjectionMatrix() const {
    return projection;
}

void Scene::setCameraPosition(const QVector3D& position) {
    cameraPos = position;
}

void Scene::setCameraFront(const QVector3D& front) {
    cameraFront = front;
}

void Scene::setCameraUp(const QVector3D& up) {
    cameraUp = up;
}

void Scene::resize(int w, int h) {
    projection.setToIdentity();
    projection.perspective(45.0f, float(w) / float(h), 0.1f, 1000.0f);
}

void Scene::clearShapes() {
    QVector<Shape*> toDelete;
    for (Shape* shape : shapes) {
        if (shape != PhysicalWorld::instance().getGroundPlane()->getShape()) {
            toDelete.append(shape);
        }
    }
    qDeleteAll(toDelete);
    shapes.clear();
}
