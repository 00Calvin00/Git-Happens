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

    if(gridOn)
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
    if(checkeredBackgroundOn)
    {
        painter.setOpacity(0.5);

        bool isFilled = true;
        for (int x = 0; x <= width(); x += 2*scale) // 2 * scale because we want nice big sqaures that scale
        {
            for (int y = 0; y <= height(); y += 2*scale)
            {
                if (isFilled)
                {
                    painter.fillRect(x, y, scale*2, scale*2, Qt::gray);
                }
                isFilled = !isFilled;
            }
        }
    }
    if(onionSkinningOn)
    {
        painter.setOpacity(0.40);
        painter.drawPixmap(0, 0, *onionSkinningPixmap);
    }
}


void Background::setPixmap(const QPixmap *previousCurrentPixmap)
{
    onionSkinningPixmap = previousCurrentPixmap;
}

void Background::setScale(int newScale)
{
    scale = newScale;
    update();
}

void Background::setGridOn(bool state)
{
    gridOn = state;
    update();
}

void Background::setOnionSkinningOn(bool state)
{
    onionSkinningOn = state;
    update();
}

void Background::setCheckeredBackgroundOn(bool state)
{
    checkeredBackgroundOn = state;
    update();
}
