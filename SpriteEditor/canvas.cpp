#include "canvas.h"
#include <QPainter>
#include <QDebug>

// Constructor initializes canvas with white background and drawing mode enabled
Canvas::Canvas(QWidget *parent, int scale) : QWidget(parent), scale(scale)
{
    pixmap = new QPixmap(512, 512);
    setFixedSize(512, 512);
    pixmap->fill(Qt::transparent);
    drawing = true;
    erasing = false;
}

// Destructor to clean up allocated pixmap
Canvas::~Canvas()
{
    delete pixmap;
}

// Gets the current scale factor
int Canvas::getScale() const
{
    return scale;
}

// Sets a new scale and triggers an update to the UI
void Canvas::setScale(int newScale)
{
    scale = newScale;
    update();
}

// Paints the current pixmap and any active cell colors on the canvas
void Canvas::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing, false);

    if (pixmap)
    {
        painter.drawPixmap(0, 0, *pixmap);
    }

    for (auto it = cellColors.begin(); it != cellColors.end(); ++it)
    {
        painter.fillRect(it.key().x(), it.key().y(), scale, scale, it.value());
    }
}

// Maps a screen position to the nearest cell on the grid
QPoint Canvas::mapToCell(const QPoint &position) const
{
    int x = (position.x() / scale) * scale;
    int y = (position.y() / scale) * scale;
    return QPoint(x, y);
}

// Draws the specified cell with the current pen color
void Canvas::drawCell(const QPoint &cell, const QColor &color)
{
    QPainter painter(pixmap);
    painter.fillRect(cell.x(), cell.y(), scale, scale, color);
    painter.end();
    update();
    emit updateCanvas();
}

// Erases the specified cell by resetting it to white
void Canvas::eraseCell(const QPoint &cell)
{
    QPainter painter(pixmap);
    painter.fillRect(cell.x(), cell.y(), scale, scale, Qt::white);
    painter.end();
    update();
    emit updateCanvas();
}

// Begins drawing or erasing on mouse press
void Canvas::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        pressed = true;
        QPoint cellPosition = mapToCell(event->pos());

        if (drawing)
        {
            drawCell(cellPosition, penColor);
        } else if (erasing)
        {
            eraseCell(cellPosition);
        }
    }
}

// Stops drawing or erasing on mouse release
void Canvas::mouseReleaseEvent(QMouseEvent *)
{
    pressed = false;
}

// Continues drawing or erasing as mouse moves while pressed
void Canvas::mouseMoveEvent(QMouseEvent *event)
{
    if (pressed)
    {
        QPoint cellPosition = mapToCell(event->pos());

        if (drawing)
        {
            drawCell(cellPosition, penColor);
        } else if (erasing)
        {
            eraseCell(cellPosition);
        }
    }
}

// Activates drawing mode
void Canvas::drawActivated()
{
    drawing = true;
    erasing = false;
}

// Activates erasing mode
void Canvas::eraseActivated()
{
    drawing = false;
    erasing = true;
}

// Returns the current pixmap
QPixmap* Canvas::getPixmap() const
{
    return pixmap;
}

// Sets a new pixmap and refreshes canvas
void Canvas::setPixmap(QPixmap* newPixmap)
{
    pixmap = newPixmap;
    update();
    repaint();
}

// Returns the canvas size in pixels
int Canvas::getCanvasSize() const
{
    return canvasSize;
}
