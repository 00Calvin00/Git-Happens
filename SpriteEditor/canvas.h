/* Reviewed by: Grace */

#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <QPixmap>
#include <QPainter>
#include <QMouseEvent>
#include <QSet>
#include <QHash>

/**
 * @class Canvas
 * @brief A widget that allows the user to draw and erase on a canvas.
 *
 * The `Canvas` class provides an interactive drawing surface that can display
 * scalable pixels, making it suitable for pixel art applications. Users can toggle
 * between drawing and erasing modes and can adjust the canvas scale factor.
 *
 * Key features:
 * - Draw or erase pixels using the mouse
 * - Scalable pixel rendering for flexible visual adjustment
 * - Emits `updateCanvas()` signal on modification for UI updates
 * - Allows integration with main UI components via QWidget
 */

class Canvas : public QWidget
{
    Q_OBJECT

public:    
    /**
     * @brief Constructor for initializing the Canvas.
     *
     * Sets up the canvas with a specified size and scale factor. The canvas starts in
     * drawing mode, and the background is initialized as white.
     *
     * @param parent The parent widget.
     * @param scale The scale factor for rendering, adjusting pixel size.
     */
    explicit Canvas(QWidget *parent, int scale = 8);

    /**
     * @brief Destructor for Canvas.
     *
     * Deletes allocated resources (e.g., pixmap).
     */
    ~Canvas();

    /**
     * @brief Sets a new pixmap to replace the current canvas content.
     *
     * Used to load or reset a saved state of the canvas. Triggers a UI update.
     *
     * @param newPixmap The new pixmap to display.
     */
    void setPixmap(QPixmap* newPixmap);

    /**
     * @brief Retrieves the current pixmap of the canvas.
     *
     * @return Pointer to the pixmap storing the canvas's drawing data.
     */
    QPixmap* getPixmap() const;

    /**
     * @brief Gets the current canvas size.
     *
     * @return The canvas size in pixels.
     */
    int getCanvasSize() const;

    /**
     * @brief Returns the current scale factor for the canvas.
     *
     * @return The scale factor.
     */
    int getScale() const;

    /**
     * @brief Sets a new scale factor for the canvas.
     *
     * Triggers an update to reflect the new scale visually.
     *
     * @param newScale The updated scale factor.
     */
    void setScale(int newScale);

    QColor penColor; ///< Current pen color for drawing.

signals:
    /**
     * @brief Signal emitted when the canvas content is modified.
     *
     * Used to notify connected UI elements to update and display the latest canvas state.
     */
    void updateCanvas();

public slots:
    /**
     * @brief Activates erasing mode, allowing the user to remove pixels.
     */
    void eraseActivated();

    /**
     * @brief Activates drawing mode, allowing the user to add pixels.
     */
    void drawActivated();

protected:
    /**
     * @brief Handles repainting the canvas.
     *
     * Uses QPainter to draw the current pixmap and any stored cell colors.
     *
     * @param event Paint event data.
     */
    void paintEvent(QPaintEvent *event) override;

    /**
     * @brief Handles mouse press events for drawing or erasing.
     *
     * Initiates a drawing/erasing action based on current mode.
     *
     * @param event Mouse event data.
     */
    void mousePressEvent(QMouseEvent *event) override;

    /**
     * @brief Handles mouse release events, resetting drawing/erasing state.
     *
     * @param event Mouse event data.
     */
    void mouseReleaseEvent(QMouseEvent *event) override;

    /**
     * @brief Handles mouse movement events to continue drawing or erasing.
     *
     * Continues action based on mode while the mouse is pressed.
     *
     * @param event Mouse event data.
     */
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    /**
     * @brief Maps a screen position to the nearest grid cell.
     *
     * Computes the grid cell coordinates corresponding to a screen position
     * based on the current scale.
     *
     * @param position Screen position.
     * @return Top-left corner of the nearest grid cell.
     */
    QPoint mapToCell(const QPoint &position) const;

    /**
     * @brief Draws a cell on the canvas at the specified position.
     *
     * Fills the cell with the current pen color.
     *
     * @param cell The target grid cell position.
     * @param color Color to draw.
     */
    void drawCell(const QPoint &cell, const QColor &color);

    /**
     * @brief Erases a cell on the canvas at the specified position.
     *
     * Clears the cell's color to white.
     *
     * @param cell The target grid cell position.
     */
    void eraseCell(const QPoint &cell);

    QHash<QPoint, QColor> cellColors;
    QPixmap *pixmap;
    bool pressed;
    bool drawing;
    bool erasing;
    int scale;
};

#endif // CANVAS_H
