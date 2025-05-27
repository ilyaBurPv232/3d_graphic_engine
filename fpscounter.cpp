#include "fpscounter.h"

FPSCounter::FPSCounter(QWidget *parentWidget) :
    QObject(parentWidget),
    frameCount(0),
    lastTime(0)
{
    fpsLabel = new QLabel(parentWidget);
    fpsLabel->setGeometry(10, 10, 100, 20);
    fpsLabel->setStyleSheet(
        "QLabel {"
        "   color: white;"
        "   font-weight: bold;"
        "   background-color: rgba(0, 0, 0, 150);"
        "   padding: 2px;"
        "   border-radius: 3px;"
        "}"
        );
    fpsLabel->raise();
    fpsLabel->show();

    fpsTimer = new QTimer(this);
    connect(fpsTimer, &QTimer::timeout, this, &FPSCounter::updateFPS);
    fpsTimer->start(100);
}

FPSCounter::~FPSCounter()
{
    delete fpsLabel;
}

void FPSCounter::setVisible(bool visible)
{
    fpsLabel->setVisible(visible);
}

void FPSCounter::setUpdateInterval(int ms)
{
    fpsTimer->setInterval(ms);
}

void FPSCounter::setStyleSheet(const QString &styleSheet)
{
    fpsLabel->setStyleSheet(styleSheet);
}

void FPSCounter::frameRendered()
{
    frameCount++;
}

void FPSCounter::updateFPS()
{
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    if (lastTime == 0) {
        lastTime = currentTime;
        return;
    }

    int fps = frameCount * 1000 / (currentTime - lastTime);
    frameCount = 0;
    lastTime = currentTime;

    fpsLabel->setText(QString("FPS: %1").arg(fps));
    emit fpsUpdated(fps);
}
