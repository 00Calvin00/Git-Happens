#ifndef MODEL_H
#define MODEL_H

#include <QPixmap>
#include <QObject>

#include "canvas.h"

class Model : public QObject
{
    Q_OBJECT
private:
    //int currentIndex = 0;  // Track the "current" pixmap
    int canvasSize = 512;
public:
    Model();
    ~Model();
    QList<QPixmap*> pixmapList;
    int currentIndex = 0;  // Track the "current" pixmap
    void updateCanvasData(); // Example: method to interact with Canvas if needed

    /**
     * @brief Updates the currentIndex to the currently selectly one, emits a sendframeListChanged signal to the model, and
     * triggers the FrameListChanged slot in the mainwindow.
     * @param int i frame to select
     */
    void SelectFrame(int i);
    void AddFrame();
    void DeleteFrame();

    /**
     * @brief Updates the currentIndex to the currently selectly one, emits a sendframeListChanged signal to the model, and
     * triggers the FrameListChanged slot in the mainwindow.
     * @param int i frame to select
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

signals:
    void SendFrameListChanged(int newIndex, QPixmap* newMap);
    void SendUpdateAnimation(QList<QPixmap*> newPixMap);
};

#endif // MODEL_H
