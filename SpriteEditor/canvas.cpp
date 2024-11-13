#include "canvas.h"
#include <QPainter>
#include <QDebug>

// Constructor to initialize the canvas with given size, scale, and color
// Canvas::Canvas(QWidget *parent, int canvasSize, int scale, QColor color)
//     : QWidget{parent}, canvasSize(canvasSize), scale(scale), pressed(false), penColor(color)
// {

// }

Canvas::Canvas(QWidget *parent, int scale) : QWidget(parent), scale(scale)
{
    pixmap = new QPixmap(512, 512);
    setFixedSize(512, 512);
    pixmap->fill(Qt::white);
    drawing = true;
    erasing = false;
}

// Destructor to clean up the allocated pixmap
Canvas::~Canvas()
{
    if (pixmap)
    {
        delete pixmap;
    }
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

void Canvas::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, false);

    // Draw filled cells with stored colors
    for (auto it = cellColors.begin(); it != cellColors.end(); ++it) {
        painter.fillRect(it.key().x(), it.key().y(), scale, scale, it.value());
    }
}

// Convert a screen position to the nearest cell position on the grid
QPoint Canvas::mapToCell(const QPoint &position) const
{
    int x = (position.x() / scale) * scale;
    int y = (position.y() / scale) * scale;
    return QPoint(x, y);
}

// Draw a cell at the specified grid position with the current pen color
void Canvas::drawCell(const QPoint &cell, const QColor &color)
{
    cellColors[cell] = color;
    update();
    emit updateCanvas(); // Notify main window to refresh UI
}

// Erase the cell at the specified grid position
void Canvas::eraseCell(const QPoint &cell)
{
    cellColors.remove(cell);
    update();
    emit updateCanvas(); // Notify main window to refresh UI
}

void Canvas::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        pressed = true;
        QPoint cellPosition = mapToCell(event->pos());

        // Draw or erase based on the current mode
        if (drawing) {
            drawCell(cellPosition, penColor);
        } else if (erasing) {
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
    if (pressed) {
        QPoint cellPosition = mapToCell(event->pos());

        // Draw or erase continuously while moving
        if (drawing) {
            drawCell(cellPosition, penColor);
        } else if (erasing) {
            eraseCell(cellPosition);
        }
    }
}

// Slot to activate the drawing mode
void Canvas::drawActivated()
{
    drawing = true;
    erasing = false;
}

// Slot to activate the erasing mode
void Canvas::eraseActivated()
{
    drawing = false;
    erasing = true;
}


//Add scale getter/setter


// Function to get the current pixmap, pixmap is the actual thing we are drawing on
QPixmap* Canvas::getPixmap() const
{
    return pixmap;
}


// Function to set a new pixmap and update the canvas
void Canvas::setPixmap(QPixmap* newPixmap)
{
    pixmap = newPixmap;
    update();
    repaint();
}

// Get the canvas size
int Canvas::getCanvasSize() const
{
    return canvasSize;
}
