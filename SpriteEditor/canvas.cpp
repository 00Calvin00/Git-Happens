#include "canvas.h"
#include <QPainter>
#include <QDebug>

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

    // Ensure that the pixmap is correctly drawn before filling cells
    if (pixmap) {
        painter.drawPixmap(0, 0, *pixmap);
    }

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
    // Update the pixmap with the new color at the cell position
    QPainter painter(pixmap);
    painter.fillRect(cell.x(), cell.y(), scale, scale, color);
    painter.end();  // End painter to save changes to the pixmap

    // Store the color in cellColors for later reference
    //cellColors[cell] = color;

    update();
    emit updateCanvas(); // Notify main window to refresh UI
}

// Erase the cell at the specified grid position
void Canvas::eraseCell(const QPoint &cell)
{
    // Remove the color from cellColors
    //cellColors.remove(cell);

    // Update the pixmap by filling the erased cell with white (or default color)
    QPainter painter(pixmap);
    painter.fillRect(cell.x(), cell.y(), scale, scale, Qt::white); // Assuming white is the background color
    painter.end();

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
