// mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class OpenGLWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onVignetteToggled(bool checked);
    void onFilmGrainToggled(bool checked);
    void onNegativeToggled(bool checked);
    void onSepiaToggled(bool checked);
    void onFisheyeToggled(bool checked);
    void onColorFilterToggled(bool checked);
    void onGrayscaleToggled(bool checked);

    void onVignetteIntensityChanged(int value);
    void onFilmGrainIntensityChanged(int value);
    void onFisheyeStrengthChanged(int value);

    void onColorFilterRedChanged(int value);
    void onColorFilterGreenChanged(int value);
    void onColorFilterBlueChanged(int value);

private:
    Ui::MainWindow *ui;
    OpenGLWidget *glWidget;
};

#endif // MAINWINDOW_H
