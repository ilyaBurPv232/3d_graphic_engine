#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class OpenGLWidget;  // Добавьте forward declaration

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void addCube();
    void addSphere();
    void addPyramid();
    void addCylinder();
    void addForceField();
    void addConstraint();

private:
    Ui::MainWindow *ui;
    OpenGLWidget *glWidget;  // Добавьте указатель на виджет
};
#endif // MAINWINDOW_H
