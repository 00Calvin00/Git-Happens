/* Reviewed by: Grace */

#include "canvas.h"
#include <QPainter>
#include <QDebug>

Canvas::Canvas(QWidget *parent, int scale) : QWidget(parent), scale(scale)
{
    pixmap = new QPixmap(512, 512);
    setFixedSize(512, 512);
    pixmap->fill(Qt::transparent);
    drawing = true;
    erasing = false;
}

Canvas::~Canvas()
{
    delete pixmap;
}

int Canvas::getScale() const
{
    return scale;
}

void Canvas::setScale(int newScale)
{
    scale = newScale;
    update();
}

void Canvas::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    // Disable antialiasing for precise pixel rendering
    painter.setRenderHint(QPainter::Antialiasing, false);

    // If a pixmap exists, draw it as the background image
    if (pixmap)
    {
        painter.drawPixmap(0, 0, *pixmap);
    }

    // Draw each colored cell stored in `cellColors`
    for (auto it = cellColors.begin(); it != cellColors.end(); ++it)
    {
        // Fill the rectangle at each cell position with its assigned color
        painter.fillRect(it.key().x(), it.key().y(), scale, scale, it.value());
    }
}

QPoint Canvas::mapToCell(const QPoint &position) const
{
    // Calculate the cell's top-left corner based on the current scale
    int x = (position.x() / scale) * scale;
    int y = (position.y() / scale) * scale;
    return QPoint(x, y);
}

void Canvas::drawCell(const QPoint &cell, const QColor &color)
{
    QPainter painter(pixmap);
    painter.fillRect(cell.x(), cell.y(), scale, scale, color);
    painter.end();
    update();
    emit updateCanvas();
}

void Canvas::eraseCell(const QPoint &cell)
{
    QPainter painter(pixmap);

    // Fills in the cell with transparent to erase
    painter.setCompositionMode(QPainter::CompositionMode_Clear);
    painter.fillRect(cell.x(), cell.y(), scale, scale, Qt::transparent);
    painter.end();

    // Updates the canvas data and UI
    update();
    emit updateCanvas();
}

void Canvas::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        pressed = true;

        // Map the mouse position to the nearest cell in the grid
        QPoint cellPosition = mapToCell(event->pos());

        if (drawing)
        {
            // Draw on the mapped cell if in drawing mode
            drawCell(cellPosition, penColor);
        }
        else if (erasing)
        {
            // Erase the mapped cell if in erasing mode
            eraseCell(cellPosition);
        }
    }
}

void Canvas::mouseReleaseEvent(QMouseEvent *)
{
    pressed = false;
}

void Canvas::mouseMoveEvent(QMouseEvent *event)
{
    if (pressed)
    {
        // Map the current mouse position to the nearest cell
        QPoint cellPosition = mapToCell(event->pos());

        if (drawing)
        {
            // Draw on the mapped cell if in drawing mode
            drawCell(cellPosition, penColor);
        }
        else if (erasing)
        {
            // Erase the mapped cell if in erasing mode
            eraseCell(cellPosition);
        }
    }
}

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

QPixmap* Canvas::getPixmap() const
{
    return pixmap;
}

void Canvas::setPixmap(QPixmap* newPixmap)
{
    pixmap = newPixmap;
    update();
    repaint();
}
