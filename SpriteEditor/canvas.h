#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <QPixmap>
#include <QPainter>
#include <QMouseEvent>

/**
 * @class Canvas
 * @brief A widget that allows the user to draw and erase on a canvas.
 *
 * The `Canvas` class provides a simple drawing surface where users can
 * interact with the canvas by drawing pixels with the mouse or erasing them.
 * It uses a `QPixmap` to store the drawing data, which is rendered on the widget
 * in a scaled fashion to simulate larger pixels. The class supports both drawing
 * and erasing modes, and it provides methods to retrieve and modify the pixmap
 * that represents the current canvas content.
 *
 * Users can activate drawing or erasing modes through public slots (`drawActivated()`
 * and `eraseActivated()`). The canvas emits a signal (`updateCanvas()`) whenever
 * the drawing is modified, so the UI can be updated accordingly.
 *
 * Key features:
 * - Draw pixels on the canvas with the mouse.
 * - Erase pixels from the canvas with the mouse.
 * - Scaled rendering of pixels for easy visibility.
 * - Easy integration with other UI components, such as the main window,
 *   through the `updateCanvas` signal.
 *
 * @note The `Canvas` widget is designed to be used as part of a larger application
 * where it can be embedded in a layout or handled as a standalone widget. It is
 * particularly useful for applications that require pixel-based editing or simple
 * drawing functionality.
 */

class Canvas : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructor to initialize the canvas.
     *
     * Initializes the canvas with a specified size, scale, and pen color.
     * The canvas is filled with a default background color (white),
     * and the drawing mode is set to "drawing" by default.
     *
     * @param parent The parent widget.
     * @param canvasSize The size of the canvas (in pixels).
     * @param scale The scale factor for zooming the canvas (for rendering larger pixels).
     * @param color The initial pen color to use for drawing.
     */
    explicit Canvas(QWidget *parent = nullptr, int canvasSize = 64, int scale = 10, QColor color = QColor(0, 0, 0));

    /**
     * @brief Destructor to clean up the canvas resources.
     *
     * Deletes the pixmap to free allocated memory.
     */
    ~Canvas();

    /**
     * @brief Set the pixmap for the canvas.
     *
     * Replaces the current pixmap with the provided one.
     * This method triggers a repaint of the canvas to reflect the new pixmap.
     *
     * @param newPixmap The new QPixmap to set for the canvas.
     */
    void setPixmap(const QPixmap &newPixmap);

    /**
     * @brief Get the current pixmap of the canvas.
     *
     * @return The current QPixmap representing the canvas.
     */
    QPixmap getPixmap() const;

    QColor penColor;  ///< The current pen color for drawing.

    /**
     * @brief Get the size of the canvas.
     *
     * @return The size of the canvas in pixels.
     */
    int getCanvasSize() const;

signals:
    /**
     * @brief Signal to notify that the canvas needs to be updated.
     *
     * This signal is emitted whenever the canvas content is modified,
     * so that the main window or parent widget can update the display.
     */
    void updateCanvas();

public slots:
    /**
     * @brief Slot to activate the erasing mode.
     *
     * This method switches the mode to erasing, allowing the user to erase pixels.
     */
    void eraseActivated();

    /**
     * @brief Slot to activate the drawing mode.
     *
     * This method switches the mode to drawing, allowing the user to draw pixels.
     */
    void drawActivated();

protected:
    /**
     * @brief Paint event to render the canvas.
     *
     * This method is called whenever the canvas needs to be redrawn.
     * It paints the current pixmap onto the widget.
     *
     * @param event The paint event passed to the method.
     */
    void paintEvent(QPaintEvent *event) override;

    /**
     * @brief Mouse press event to handle drawing or erasing on click.
     *
     * This method is called when the mouse button is pressed. It triggers either
     * drawing or erasing depending on the current mode.
     *
     * @param event The mouse event that contains the mouse position and button clicked.
     */
    void mousePressEvent(QMouseEvent *event) override;

    /**
     * @brief Mouse release event to reset the pressed state.
     *
     * This method is called when the mouse button is released.
     * It resets the pressed state to allow for new drawing or erasing actions.
     *
     * @param event The mouse event that contains the mouse position and button released.
     */
    void mouseReleaseEvent(QMouseEvent *event) override;

    /**
     * @brief Mouse move event to continue drawing or erasing.
     *
     * This method is called when the mouse moves. It continues drawing or erasing
     * while the mouse is pressed.
     *
     * @param event The mouse event that contains the updated mouse position.
     */
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    /**
     * @brief Draw on the canvas at the specified position.
     *
     * This method is called when drawing is activated, and it draws a "pixel" at
     * the position based on the mouse event. The position is calculated using
     * the scale factor.
     *
     * @param event The mouse event that contains the mouse position.
     */
    void draw(QMouseEvent *event);

    /**
     * @brief Erase from the canvas at the specified position.
     *
     * This method is called when erasing is activated, and it erases a "pixel" at
     * the position based on the mouse event. The position is calculated using
     * the scale factor.
     *
     * @param event The mouse event that contains the mouse position.
     */
    void erase(QMouseEvent *event);

    QPixmap *pixmap;  ///< The pixmap that stores the canvas content.
    int canvasSize;   ///< The size of the canvas in pixels.
    int scale;        ///< The scale factor to zoom the canvas.
    bool pressed;     ///< Boolean to check if the mouse is pressed.
    bool drawing;     ///< Boolean to check if drawing mode is active.
    bool erasing;     ///< Boolean to check if erasing mode is active.
};

#endif // CANVAS_H
