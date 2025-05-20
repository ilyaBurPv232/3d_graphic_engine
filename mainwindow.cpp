#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "openglwidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("3D Cube with Camera");

    OpenGLWidget *glWidget = new OpenGLWidget(this);
    ui->verticalLayout->addWidget(glWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}
