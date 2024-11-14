#include "background.h"
#include <QPainter>

// Constructor initializes background properties and sets default widget size
Background::Background(QWidget *parent, int scale)
    : QWidget(parent), scale(scale)
{
    setFixedSize(512, 512); // Adjusts based on canvas size for consistency
}

// Paint event draws the grid, checkered background, and onion skinning overlay
void Background::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    // Draw grid if enabled
    if (gridOn)
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

    // Draw checkered background if enabled
    if (checkeredBackgroundOn)
    {
        painter.setOpacity(0.5);
        bool isFilled = true;
        for (int x = 0; x <= width(); x += 2 * scale)
        {
            for (int y = 0; y <= height(); y += 2 * scale)
            {
                if (isFilled)
                {
                    painter.fillRect(x, y, scale * 2, scale * 2, Qt::gray);
                }
                isFilled = !isFilled;
            }
        }
    }

    // Draw onion skinning overlay if enabled
    if (onionSkinningOn && onionSkinningPixmap)
    {
        painter.setOpacity(0.75);
        painter.drawPixmap(0, 0, *onionSkinningPixmap);
    }
}

// Sets a new pixmap for onion skinning overlay
void Background::setPixmap(const QPixmap *previousCurrentPixmap)
{
    onionSkinningPixmap = previousCurrentPixmap;
}

// Updates the grid cell scale and triggers repaint
void Background::setScale(int newScale)
{
    scale = newScale;
    update();
}

// Enables or disables the grid overlay
void Background::setGridOn(bool state)
{
    gridOn = state;
    update();
}

// Enables or disables onion skinning overlay
void Background::setOnionSkinningOn(bool state)
{
    onionSkinningOn = state;
    update();
}

// Enables or disables checkered background pattern
void Background::setCheckeredBackgroundOn(bool state)
{
    checkeredBackgroundOn = state;
    update();
}
