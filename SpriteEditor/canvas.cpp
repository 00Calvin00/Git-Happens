#include "canvas.h"
#include <QPainter>

Canvas::Canvas(QWidget *parent, int canvasSize, int scale)
    : QWidget{parent}, canvasSize(canvasSize), scale(scale), pressed(false)
{
    color = Qt::black;
    pixmap = new QPixmap(canvasSize, canvasSize);  // Initialize a blank pixmap of the desired size
    pixmap->fill(Qt::white);         // Fill with a default background color (e.g., white)
    resize(canvasSize * scale, canvasSize * scale); //scale the canvas up (zoom in) so the pixels are easy to see
}

Canvas::~Canvas()
{
    if (pixmap)
    {
        delete pixmap;
    }
}

void Canvas::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(0, 0, pixmap->scaled(canvasSize * scale, canvasSize * scale)); // Draw the pixmap on the widget
}

void Canvas::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        pressed = true;
        draw(event);
    }
}

void Canvas::mouseReleaseEvent(QMouseEvent *)
{
    pressed = false;
}

void Canvas::mouseMoveEvent(QMouseEvent *event)
{
    draw(event);
}

void Canvas::draw(QMouseEvent *event)
{
    if (pressed)
    {
        QPainter painter(pixmap);
        painter.setPen(QPen(color, scale));

        // Calculate grid positions to draw on
        int x = event->pos().x() / scale; //What do we need to divide by here??
        int y = event->pos().y() / scale; //What do we need to divide by here??

        painter.drawRect(x, y, 1, 1); // Draw a filled rectangle to represent the "pixel" at the scaled size

        repaint();
        emit updateCanvas();  // Emit signal to update the display in MainWindow
    }
}

QPixmap Canvas::getPixmap() const
{
    return *pixmap;  // Return the current QPixmap that represents the drawing
}
