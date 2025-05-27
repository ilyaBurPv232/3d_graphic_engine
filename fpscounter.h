#ifndef FPSCOUNTER_H
#define FPSCOUNTER_H

#include <QObject>
#include <QTimer>
#include <QLabel>
#include <QDateTime>

class FPSCounter : public QObject
{
    Q_OBJECT
public:
    explicit FPSCounter(QWidget *parentWidget = nullptr);
    ~FPSCounter();

    void setVisible(bool visible);
    void setUpdateInterval(int ms);
    void setStyleSheet(const QString &styleSheet);
    void toggleVisibility();

public slots:
    void frameRendered();

signals:
    void fpsUpdated(int fps);

private:
    void updateFPS();

    QLabel *fpsLabel;
    QTimer *fpsTimer;
    int frameCount;
    qint64 lastTime;
};

#endif // FPSCOUNTER_H
