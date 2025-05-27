#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "openglwidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , glWidget(nullptr)
{
    ui->setupUi(this);
    setWindowTitle("3D Cube with Camera");

    // Создаем OpenGLWidget
    glWidget = new OpenGLWidget(ui->tab);
    QVBoxLayout *tabLayout = new QVBoxLayout(ui->tab);
    tabLayout->addWidget(glWidget);
    ui->tab->setLayout(tabLayout);

    ui->tabWidget->setTabText(0, "3D View");
}

MainWindow::~MainWindow()
{
    delete ui;
    // OpenGLWidget удалится автоматически, так как он является дочерним виджетом
}
