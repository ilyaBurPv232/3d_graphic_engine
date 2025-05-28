// mainwindow.cpp
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "openglwidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , glWidget(nullptr)
{
    ui->setupUi(this);
    setWindowTitle("3D engine");

    glWidget = new OpenGLWidget(ui->tab);
    QVBoxLayout *tabLayout = new QVBoxLayout(ui->tab);
    tabLayout->addWidget(glWidget);
    ui->tab->setLayout(tabLayout);

    ui->tabWidget->setTabText(0, "Сцена");
    ui->tabWidget->setTabText(1, "Пост-обработка");

    connect(ui->vignetteCheckBox, &QCheckBox::toggled, this, &MainWindow::onVignetteToggled);
    connect(ui->filmGrainCheckBox, &QCheckBox::toggled, this, &MainWindow::onFilmGrainToggled);
    connect(ui->negativeCheckBox, &QCheckBox::toggled, this, &MainWindow::onNegativeToggled);
    connect(ui->sepiaCheckBox, &QCheckBox::toggled, this, &MainWindow::onSepiaToggled);
    connect(ui->fisheyeCheckBox, &QCheckBox::toggled, this, &MainWindow::onFisheyeToggled);
    connect(ui->colorFilterCheckBox, &QCheckBox::toggled, this, &MainWindow::onColorFilterToggled);
    connect(ui->grayscaleCheckBox, &QCheckBox::toggled, this, &MainWindow::onGrayscaleToggled);

    connect(ui->vignetteIntensitySlider, &QSlider::valueChanged, this, &MainWindow::onVignetteIntensityChanged);
    connect(ui->filmGrainIntensitySlider, &QSlider::valueChanged, this, &MainWindow::onFilmGrainIntensityChanged);
    connect(ui->fisheyeStrengthSlider, &QSlider::valueChanged, this, &MainWindow::onFisheyeStrengthChanged);

    connect(ui->colorFilterRedSlider, &QSlider::valueChanged, this, &MainWindow::onColorFilterRedChanged);
    connect(ui->colorFilterGreenSlider, &QSlider::valueChanged, this, &MainWindow::onColorFilterGreenChanged);
    connect(ui->colorFilterBlueSlider, &QSlider::valueChanged, this, &MainWindow::onColorFilterBlueChanged);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onVignetteToggled(bool checked) {
    glWidget->getPostProcessor()->setVignetteEnabled(checked);
    glWidget->update();
}

void MainWindow::onFilmGrainToggled(bool checked) {
    glWidget->getPostProcessor()->setFilmGrainEnabled(checked);
    glWidget->update();
}

void MainWindow::onNegativeToggled(bool checked) {
    glWidget->getPostProcessor()->setNegativeEnabled(checked);
    glWidget->update();
}

void MainWindow::onSepiaToggled(bool checked) {
    glWidget->getPostProcessor()->setSepiaEnabled(checked);
    glWidget->update();
}

void MainWindow::onFisheyeToggled(bool checked) {
    glWidget->getPostProcessor()->setFisheyeEnabled(checked);
    glWidget->update();
}

void MainWindow::onColorFilterToggled(bool checked) {
    glWidget->getPostProcessor()->setColorFilterEnabled(checked);
    glWidget->update();
}

void MainWindow::onGrayscaleToggled(bool checked) {
    glWidget->getPostProcessor()->setGrayscaleEnabled(checked);
    glWidget->update();
}

void MainWindow::onVignetteIntensityChanged(int value) {
    float intensity = static_cast<float>(value) / 50.0f;
    glWidget->getPostProcessor()->setVignetteIntensity(intensity);
    glWidget->update();
}

void MainWindow::onFilmGrainIntensityChanged(int value) {
    float intensity = static_cast<float>(value) / 50.0f;
    glWidget->getPostProcessor()->setFilmGrainIntensity(intensity);
    glWidget->update();
}

void MainWindow::onFisheyeStrengthChanged(int value) {
    float strength = static_cast<float>(value) / 50.0f;
    glWidget->getPostProcessor()->setFisheyeStrength(strength);
    glWidget->update();
}

void MainWindow::onColorFilterRedChanged(int value) {
    QVector3D color = glWidget->getPostProcessor()->getColorFilterColor();
    color.setX(static_cast<float>(value) / 100.0f);
    glWidget->getPostProcessor()->setColorFilterColor(color);
    glWidget->update();
}

void MainWindow::onColorFilterGreenChanged(int value) {
    QVector3D color = glWidget->getPostProcessor()->getColorFilterColor();
    color.setY(static_cast<float>(value) / 100.0f);
    glWidget->getPostProcessor()->setColorFilterColor(color);
    glWidget->update();
}

void MainWindow::onColorFilterBlueChanged(int value) {
    QVector3D color = glWidget->getPostProcessor()->getColorFilterColor();
    color.setZ(static_cast<float>(value) / 100.0f);
    glWidget->getPostProcessor()->setColorFilterColor(color);
    glWidget->update();
}
