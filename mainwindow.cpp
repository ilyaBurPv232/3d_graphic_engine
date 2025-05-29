#include "physicalworld.h"
#include "mainwindow.h"
#include "openglwidget.h"
#include "ui_mainwindow.h"
#include "physicalprimitives.h"
#include "forcefield.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Создаем и добавляем OpenGL виджет
    glWidget = new OpenGLWidget(this);
    ui->tab->layout()->addWidget(glWidget);

    // Подключаем сигнал FPS к метке
    connect(glWidget, &OpenGLWidget::fpsUpdated, this, [this](int fps) {
        ui->fps_counter->setText(QString("FPS: %1").arg(fps));
    });

    // Настраиваем видимость групп параметров
    ui->cubeGroupBox->setVisible(false);
    ui->sphereGroupBox->setVisible(false);
    ui->pyramidGroupBox->setVisible(false);
    ui->cylinderGroupBox->setVisible(false);

    // Подключаем чекбоксы объектов к отображению соответствующих групп
    connect(ui->cubeCheckBox, &QCheckBox::toggled, ui->cubeGroupBox, &QGroupBox::setVisible);
    connect(ui->sphereCheckBox, &QCheckBox::toggled, ui->sphereGroupBox, &QGroupBox::setVisible);
    connect(ui->pyramidCheckBox, &QCheckBox::toggled, ui->pyramidGroupBox, &QGroupBox::setVisible);
    connect(ui->cylinderCheckBox, &QCheckBox::toggled, ui->cylinderGroupBox, &QGroupBox::setVisible);

    // Обработка кнопки Apply
    connect(ui->applyButton, &QPushButton::clicked, this, [this]() {
        PhysicalWorld& world = PhysicalWorld::instance();
        world.clearObjects();
        world.clearConstraints();
        world.clearForceFields();

        if (ui->cubeCheckBox->isChecked()) {
            addCube();
        }
        if (ui->sphereCheckBox->isChecked()) {
            addSphere();
        }
        if (ui->pyramidCheckBox->isChecked()) {
            addPyramid();
        }
        if (ui->cylinderCheckBox->isChecked()) {
            addCylinder();
        }

        if (!ui->forceFieldStrength->text().isEmpty()) {
            addForceField();
        }

        // Добавляем constraint если заданы параметры
        if (!ui->constraintDistance->text().isEmpty()) {
            addConstraint();
        }

        // Обновляем сцену
        glWidget->updateScene();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addCube()
{
    try {
        QString texture = ui->cubeTexture->currentText();
        float mass = ui->cubeMass->text().toFloat();
        float elasticity = ui->cubeElasticity->text().toFloat();
        float friction = ui->cubeFriction->text().toFloat();
        bool isStatic = ui->cubeStatic->isChecked();

        QVector3D position(
            ui->cubePositionX->text().toFloat(),
            ui->cubePositionY->text().toFloat(),
            ui->cubePositionZ->text().toFloat()
            );

        QVector3D velocity(
            ui->cubeVelocityX->text().toFloat(),
            ui->cubeVelocityY->text().toFloat(),
            ui->cubeVelocityZ->text().toFloat()
            );

        QVector3D scale(
            ui->cubeScaleX->text().toFloat(),
            ui->cubeScaleY->text().toFloat(),
            ui->cubeScaleZ->text().toFloat()
            );

        PhycCube* cube = new PhycCube(texture, mass, elasticity);
        cube->setPosition(position);
        cube->setVelocity(velocity);
        cube->getShape()->setScale(scale);
        cube->setStatic(isStatic);
        cube->setFrictionCoefficient(friction);

        PhysicalWorld::instance().addObject(cube);
    } catch (...) {
        QMessageBox::warning(this, "Error", "Invalid cube parameters");
    }
}

void MainWindow::addSphere()
{
    try {
        QString texture = ui->sphereTexture->currentText();
        float mass = ui->sphereMass->text().toFloat();
        float elasticity = ui->sphereElasticity->text().toFloat();
        float friction = ui->sphereFriction->text().toFloat();
        bool isStatic = ui->sphereStatic->isChecked();

        QVector3D position(
            ui->spherePositionX->text().toFloat(),
            ui->spherePositionY->text().toFloat(),
            ui->spherePositionZ->text().toFloat()
            );

        QVector3D velocity(
            ui->sphereVelocityX->text().toFloat(),
            ui->sphereVelocityY->text().toFloat(),
            ui->sphereVelocityZ->text().toFloat()
            );

        QVector3D scale(
            ui->sphereScaleX->text().toFloat(),
            ui->sphereScaleY->text().toFloat(),
            ui->sphereScaleZ->text().toFloat()
            );

        PhycSphere* sphere = new PhycSphere(texture, mass, elasticity);
        sphere->setPosition(position);
        sphere->setVelocity(velocity);
        sphere->getShape()->setScale(scale);
        sphere->setStatic(isStatic);
        sphere->setFrictionCoefficient(friction);

        PhysicalWorld::instance().addObject(sphere);
    } catch (...) {
        QMessageBox::warning(this, "Error", "Invalid sphere parameters");
    }
}

void MainWindow::addPyramid()
{
    try {
        QString texture = ui->pyramidTexture->currentText();
        float mass = ui->pyramidMass->text().toFloat();
        float elasticity = ui->pyramidElasticity->text().toFloat();
        float friction = ui->pyramidFriction->text().toFloat();
        bool isStatic = ui->pyramidStatic->isChecked();

        QVector3D position(
            ui->pyramidPositionX->text().toFloat(),
            ui->pyramidPositionY->text().toFloat(),
            ui->pyramidPositionZ->text().toFloat()
            );

        QVector3D velocity(
            ui->pyramidVelocityX->text().toFloat(),
            ui->pyramidVelocityY->text().toFloat(),
            ui->pyramidVelocityZ->text().toFloat()
            );

        QVector3D scale(
            ui->pyramidScaleX->text().toFloat(),
            ui->pyramidScaleY->text().toFloat(),
            ui->pyramidScaleZ->text().toFloat()
            );

        PhycPyramid* pyramid = new PhycPyramid(texture, mass, elasticity);
        pyramid->setPosition(position);
        pyramid->setVelocity(velocity);
        pyramid->getShape()->setScale(scale);
        pyramid->setStatic(isStatic);
        pyramid->setFrictionCoefficient(friction);

        PhysicalWorld::instance().addObject(pyramid);
    } catch (...) {
        QMessageBox::warning(this, "Error", "Invalid pyramid parameters");
    }
}

void MainWindow::addCylinder()
{
    try {
        QString texture = ui->cylinderTexture->currentText();
        float mass = ui->cylinderMass->text().toFloat();
        float elasticity = ui->cylinderElasticity->text().toFloat();
        float friction = ui->cylinderFriction->text().toFloat();
        bool isStatic = ui->cylinderStatic->isChecked();

        QVector3D position(
            ui->cylinderPositionX->text().toFloat(),
            ui->cylinderPositionY->text().toFloat(),
            ui->cylinderPositionZ->text().toFloat()
            );

        QVector3D velocity(
            ui->cylinderVelocityX->text().toFloat(),
            ui->cylinderVelocityY->text().toFloat(),
            ui->cylinderVelocityZ->text().toFloat()
            );

        QVector3D scale(
            ui->cylinderScaleX->text().toFloat(),
            ui->cylinderScaleY->text().toFloat(),
            ui->cylinderScaleZ->text().toFloat()
            );

        PhycCylinder* cylinder = new PhycCylinder(texture, mass, elasticity);
        cylinder->setPosition(position);
        cylinder->setVelocity(velocity);
        cylinder->getShape()->setScale(scale);
        cylinder->setStatic(isStatic);
        cylinder->setFrictionCoefficient(friction);

        PhysicalWorld::instance().addObject(cylinder);
    } catch (...) {
        QMessageBox::warning(this, "Error", "Invalid cylinder parameters");
    }
}

void MainWindow::addForceField()
{
    try {
        QString typeStr = ui->forceFieldType->currentText();
        ForceField::FieldType type = ForceField::WIND;

        if (typeStr == "WIND") type = ForceField::WIND;
        else if (typeStr == "MAGNETIC") type = ForceField::MAGNETIC;
        else if (typeStr == "VORTEX") type = ForceField::VORTEX;
        else if (typeStr == "GRAVITY_POINT") type = ForceField::GRAVITY_POINT;
        else if (typeStr == "EXPLOSION") type = ForceField::EXPLOSION;

        QVector3D position(
            ui->forceFieldPositionX->text().toFloat(),
            ui->forceFieldPositionY->text().toFloat(),
            ui->forceFieldPositionZ->text().toFloat()
            );

        QVector3D direction(
            ui->forceFieldDirectionX->text().toFloat(),
            ui->forceFieldDirectionY->text().toFloat(),
            ui->forceFieldDirectionZ->text().toFloat()
            );

        float strength = ui->forceFieldStrength->text().toFloat();
        float radius = ui->forceFieldRadius->text().toFloat();

        ForceField* field = new ForceField(type, position, direction, strength, radius);
        PhysicalWorld::instance().addForceField(field);
    } catch (...) {
        QMessageBox::warning(this, "Error", "Invalid force field parameters");
    }
}

void MainWindow::addConstraint()
{
    try {
        QString typeStr = ui->constraintType->currentText();
        QString objAStr = ui->constraintObjectA->currentText();
        QString objBStr = ui->constraintObjectB->currentText();

        QVector<PhysicalObject*> objects = PhysicalWorld::instance().getObjects();
        if (objects.size() < 2) {
            QMessageBox::warning(this, "Error", "Need at least 2 objects to create constraint");
            return;
        }

        // В реальном приложении нужно более сложное сопоставление объектов
        PhysicalObject* objA = objects[0]; // Упрощенный выбор объектов
        PhysicalObject* objB = objects[1]; // В реальном приложении нужно хранить указатели

        if (typeStr == "Rope") {
            float maxLength = ui->constraintDistance->text().toFloat();
            RopeConstraint* rope = new RopeConstraint(objA, objB, maxLength);
            PhysicalWorld::instance().addRopeConstraint(rope);
        }
        else if (typeStr == "Spring") {
            float restLength = ui->constraintDistance->text().toFloat();
            float stiffness = ui->constraintStiffness->text().toFloat();
            float damping = ui->constraintDamping->text().isEmpty() ? 0.1f : ui->constraintDamping->text().toFloat();
            SpringConstraint* spring = new SpringConstraint(objA, objB, restLength, stiffness, damping);
            PhysicalWorld::instance().addSpringConstraint(spring);
        }
        else if (typeStr == "Rigid") {
            float distance = ui->constraintDistance->text().toFloat();
            RigidConstraint* rigid = new RigidConstraint(objA, objB, distance);
            PhysicalWorld::instance().addRigidConstraint(rigid);
        }
    } catch (...) {
        QMessageBox::warning(this, "Error", "Invalid constraint parameters");
    }
}
