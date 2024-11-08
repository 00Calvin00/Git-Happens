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

// Paint event to render the pixmap onto the canvas
void Canvas::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    // Draw the scaled pixmap on the widget
    painter.drawPixmap(0, 0, pixmap->scaled(canvasSize * scale, canvasSize * scale));
}

// Mouse press event to handle clicks for drawing or erasing
void Canvas::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        pressed = true;

        // Call the appropriate function based on the current mode (drawing or erasing)
        if (drawing)
        {
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
        //painter.drawRect(x, y, 1, 1);
        painter.drawPoint(x,y);
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

// Function to handle the erasing action on the canvas
void Canvas::erase(QMouseEvent *event)
{
    if (pressed && erasing)  // Ensure the mouse is pressed and in erasing mode
    {
        QPainter painter(pixmap);
        painter.setPen(QPen(Qt::white, scale));  // Set the pen color to white for erasing, maybe make it transparent???

        // Calculate the grid position for the pixel to erase
        int x = event->pos().x() / scale;
        int y = event->pos().y() / scale;

        // Erase the pixel by drawing a white square over it, or make transparent??
        painter.drawRect(x, y, 1, 1);

        repaint();  // Repaint the canvas to update the display
        emit updateCanvas();  // Emit signal to notify MainWindow to update
    }
}

// Function to get the current pixmap, pixmap is the actual thing we are drawing on
QPixmap Canvas::getPixmap() const
{
    return *pixmap;
}

// Function to set a new pixmap and update the canvas
void Canvas::setPixmap(const QPixmap &newPixmap)
{
    *pixmap = newPixmap;
    repaint();
}

// Getter for canvas size
int Canvas::getCanvasSize() const
{
    return canvasSize;
}
