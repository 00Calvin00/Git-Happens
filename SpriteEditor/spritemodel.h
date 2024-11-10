#ifndef MODEL_H
#define MODEL_H

#include <QPixmap>
#include <QObject>

#include "canvas.h"

class Model : public QObject
{
    Q_OBJECT
private:
    QList<QPixmap*> pixmapList;
    int currentIndex = 0;  // Track the "current" pixmap
    int canvasSize = 64;
public:
    Model();
    ~Model();
    void updateCanvasData(); // Example: method to interact with Canvas if needed
    void SelectFrame(int i);
    void DuplicateFrame(QPixmap importedMap);
    void AddFrame();
    void DeleteFrame();
    void SizeChange(int newSize);
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
