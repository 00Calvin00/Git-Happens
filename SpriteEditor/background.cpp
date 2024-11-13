#include "background.h"
#include <QPainter>

Background::Background(QWidget *parent, int scale)
    : QWidget(parent), scale(scale)
{
    setFixedSize(512, 512); // Adjust based on canvas size
}

void Background::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    if(gridIsOn)
    {
        painter.setPen(Qt::lightGray);
        for (int x = 0; x <= width(); x += scale)
        {
            painter.drawLine(x, 0, x, height());
        }
        for (int y = 0; y <= height(); y += scale)
        {
            painter.drawLine(0, y, width(), y);
        }
    }
}

void Background::setScale(int newScale)
{
    scale = newScale;
    update();
}

void Background::toggleGrid()
{
    gridIsOn = !gridIsOn;
}
