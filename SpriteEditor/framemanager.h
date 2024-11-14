/* Reviewed by: Calvin */

#ifndef MODEL_H
#define MODEL_H

#include <QPixmap>
#include <QObject>
#include "canvas.h"

class FrameManager : public QObject
{
    Q_OBJECT
private:
    int canvasSize = 512;

public:
    /**
     * @brief Constructs a FrameManager instance.
     *
     * Initializes an empty list of pixmap frames for managing frames.
     */
    FrameManager();

    /**
     * @brief Destructor for the frame manager instance
     *
     * Deletes all QPixmap pointers in the list to release allocated memory.
     */
    ~FrameManager();

    /**
     * @brief Updates the currentIndex to the currently selectly one, emits a sendframeListChanged signal to the model, and
     * triggers the FrameListChanged slot in the mainwindow.
     * @param int i frame to select
     */
    void SelectFrame(int i);

    /**
     * @brief Adds a new blank frame to the pixmap list.
     */
    void AddFrame();

    /**
     * @brief Deletes the current frame from the pixmap list.
     *
     */
    void DeleteFrame();

    /**
     * @brief Adds an initial frame from a Canvas instance to the pixmap list.
     *
     * This function retrieves the initial QPixmap from the given Canvas and
     * sets it as the first frame in the list, with the current index set to zero.
     *
     * @param canvas Pointer to the Canvas from which the initial frame is retrieved.
     */
    void AddInitialFrame(Canvas* canvas);

    /**
     * @brief Retrieves the values of the current frames from the PixmapList
     * @return The current QPixmap representing the canvas.
     */
    const QList<QPixmap*> getPixmapListValues();

    /**
     * @brief Retrieves the actual current frames from the PixmapList
     * @return The current QPixmap representing the canvas.
     */
    QList<QPixmap*>& getPixmapListObjects();

    /**
     * @brief Sets the current frames in the PixmapList
     * @param A new list of Pixmaps to set the current frames to.
     */
    void setPixmapList(const QList<QPixmap*>& newPixmapList);

    /**
     * @brief Retrieves the current index value representing the currently selected frame.
     * @return int current index.
     */
    int getCurrentIndex();

    /**
     * @brief Updates canvas data with the current frame information.
     */
    void updateCanvasData(); // Example: method to interact with Canvas if needed

    QList<QPixmap*> pixmapList; // List of frames represented as QPixmaps.
    int currentIndex = 0;  // Track the "current" pixmap

signals:
    /**
     * @brief Signal emitted when the frame list is changed.
     *
     * @param newIndex The new current index after selection or modification.
     * @param newMap The QPixmap associated with the new current frame.
     */
    void SendFrameListChanged(int newIndex, QPixmap* newMap);

    /**
     * @brief Signal emitted to update the animation display with the current frame list.
     *
     * @param newPixMap The updated list of frames as QPixmaps.
     */
    void SendUpdateAnimation(QList<QPixmap*> newPixMap);
};

#endif // MODEL_H
