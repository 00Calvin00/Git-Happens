#include "canvas.h"
#include <QPainter>
#include <QDebug>

// Constructor to initialize the canvas with the given size, scale, and color
Canvas::Canvas(QWidget *parent, int canvasSize, int scale, QColor color)
    : QWidget{parent}, canvasSize(canvasSize), scale(scale), pressed(false), penColor(color)
{
    // Initialize a blank pixmap with the given canvas size
    pixmap = new QPixmap(canvasSize, canvasSize);
    pixmap->fill(Qt::white);  //Maybe make trasnsparent once we have a backgrounnd??????

    // Scale the canvas up to make the pixels visible
    resize(canvasSize * scale, canvasSize * scale);

    drawing = true;  // Default mode is drawing
    erasing = false; // Erase mode is off by default
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
    cellColors[cell] = color;

    update();
    emit updateCanvas(); // Notify main window to refresh UI
}

// Erase the cell at the specified grid position
void Canvas::eraseCell(const QPoint &cell)
{
    // Remove the color from cellColors
    cellColors.remove(cell);

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

        // Draw or erase based on the current mode
        // Call the appropriate function based on the current mode (drawing or erasing)
        if (drawing)
            draw(event);
        }
        else if (erasing)
        {
            erase(event);
        }
    }
}

// Mouse release event to reset the pressed state when the button is released
void Canvas::mouseReleaseEvent(QMouseEvent *)
{
    pressed = false;
}

// Mouse move event to handle drawing/erasing while the mouse moves
void Canvas::mouseMoveEvent(QMouseEvent *event)
{
    // Continue drawing or erasing based on the current mode
    if (drawing)
    {
        draw(event);
    }
    else if (erasing)
    {
        erase(event);
    }
}

// Slot to activate the drawing mode
void Canvas::drawActivated()
{
    drawing = true;
    erasing = false;
}

// Function to handle the drawing action on the canvas
void Canvas::draw(QMouseEvent *event)
{
    if (pressed && drawing)  // Ensure the mouse is pressed and in drawing mode
    {
        QPainter painter(pixmap);
        painter.setPen(QPen(penColor, scale));  // Set the pen color and size

        // Calculate the grid position for the pixel based on mouse position
        int x = event->pos().x() / scale;
        int y = event->pos().y() / scale;

        // Draw a pixel at the calculated position
        painter.drawRect(x, y, 1, 1);

        repaint();  // Repaint the canvas to update the display
        emit updateCanvas();  // Emit signal to notify MainWindow to update
    }
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

// Getter for canvas size
int Canvas::getCanvasSize() const
{
    return canvasSize;
}
