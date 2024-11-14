#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <QWidget>
#include <QPixmap>
#include <QPainter>

/**
 * @class Background
 * @brief A widget that manages the background display for the canvas, including grid, checkered pattern, and onion skinning.
 *
 * The `Background` class provides optional visual layers to enhance pixel editing, such as:
 * - A grid overlay for precise positioning
 * - A checkered background for transparency visualization
 * - An onion skinning layer for frame-by-frame animation reference
 */
class Background : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructor for initializing the background.
     *
     * Sets up the background with specified scale for grid cells.
     *
     * @param parent The parent widget.
     * @param scale Size of each grid cell.
     */
    Background(QWidget *parent = nullptr, int scale = 16);

    /**
     * @brief Sets the onion skinning pixmap overlay.
     *
     * Used to apply a previous frame as a translucent background, aiding frame-to-frame consistency.
     *
     * @param previousCurrentPixmap Pointer to the pixmap for onion skinning.
     */
    void setPixmap(const QPixmap *previousCurrentPixmap);

    /**
     * @brief Sets a new scale for grid cells.
     *
     * Adjusts the size of the grid cells and triggers a repaint.
     *
     * @param newScale The updated grid cell size.
     */
    void setScale(int newScale);

    /**
     * @brief Enables or disables the grid overlay.
     *
     * @param on Set to true to display the grid; false to hide it.
     */
    void setGridOn(bool on);

    /**
     * @brief Enables or disables the onion skinning overlay.
     *
     * @param on Set to true to display onion skinning; false to hide it.
     */
    void setOnionSkinningOn(bool on);

    /**
     * @brief Enables or disables the checkered background pattern.
     *
     * Aids in visualizing transparent areas.
     *
     * @param on Set to true to display the checkered background; false to hide it.
     */
    void setCheckeredBackgroundOn(bool on);

protected:
    /**
     * @brief Paint event to render the background elements.
     *
     * Uses QPainter to draw grid lines, checkered patterns, and the onion skinning pixmap, based on active flags.
     *
     * @param event Paint event data.
     */
    void paintEvent(QPaintEvent *event) override;

private:
    int scale;                  ///< Size of each grid cell.
    bool gridOn = false;        ///< Flag to toggle grid visibility.
    bool onionSkinningOn = false; ///< Flag to toggle onion skinning visibility.
    bool checkeredBackgroundOn = false; ///< Flag to toggle checkered background visibility.
    const QPixmap *onionSkinningPixmap; ///< Pixmap used for onion skinning overlay.
};

#endif // BACKGROUND_H
