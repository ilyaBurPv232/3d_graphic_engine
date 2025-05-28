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

    // Настраиваем FPS-лейбл
    ui->fps_counter->setParent(glWidget); // Делаем glWidget родителем
    ui->fps_counter->setGeometry(10, 10, 100, 20); // Позиция и размер
    ui->fps_counter->setStyleSheet(
        "QLabel {"
        "   color: white;"
        "   font-weight: bold;"
        "   background-color: rgba(0, 0, 0, 150);"
        "   padding: 2px;"
        "   border-radius: 3px;"
        "}"
        );
    ui->fps_counter->raise(); // Поднимаем на передний план
    ui->fps_counter->show();

    connect(glWidget, &OpenGLWidget::fpsUpdated, this, [this](int fps) {
        ui->fps_counter->setText(QString("FPS: %1").arg(fps));
    });
}

MainWindow::~MainWindow()
{
    delete ui;
    // OpenGLWidget удалится автоматически, так как он является дочерним виджетом
}
