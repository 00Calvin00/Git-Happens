/* Reviewed by: Calvin */

#include <QDebug>
#include "framemanager.h"

FrameManager::FrameManager() {}

FrameManager::~FrameManager()
{
    qDeleteAll(pixmapList); // Delete each QPixmap pointer stored in the list
    pixmapList.clear();  // Clear the list
}

void FrameManager::addInitialFrame(Canvas* canvas)
{
    // Store pointer to QPixmap in the list
    pixmapList.append(canvas->getPixmap());
    currentIndex = 0;  // Set the initial selected index
}

void FrameManager::addFrame()
{
    if (pixmapList.size() != 0)
    {
        currentIndex++;
    }

    QPixmap* newMap = new QPixmap(canvasSize, canvasSize);


    newMap->fill(Qt::transparent);
    if (pixmapList.size() != currentIndex && pixmapList.size() != 0)
    {
        pixmapList.insert(currentIndex, newMap); // Insert inserts before i, turning i into the new object
    }
    else
    {
        pixmapList.append(newMap); // Else we are at the end, so simply append the newMap, since we already increased the index
    }
    emit sendFrameListChanged(currentIndex, newMap);
    emit sendUpdateAnimation(pixmapList);
}

void FrameManager::deleteFrame()
{
    // If there’s more than one item, remove the current frame
    if (pixmapList.size() > 1)
    {
        pixmapList.remove(currentIndex);

        // Set currentIndex to last index if out of bounds
        if (currentIndex >= pixmapList.size())
        {
            currentIndex = pixmapList.size() - 1;
        }
    }
    // If there’s only one item, remove it and add a new blank frame
    else
    {
        pixmapList.remove(currentIndex);
        addFrame();
        currentIndex = 0;
    }

    QPixmap* newMap = pixmapList.at(currentIndex);
    emit sendFrameListChanged(currentIndex, newMap);
    emit sendUpdateAnimation(pixmapList);
}

void FrameManager::selectFrame(int index)
{
    if (currentIndex >= 0 && currentIndex < pixmapList.size())
    {
        currentIndex = index;
        QPixmap* newMap = pixmapList.at(index);
        emit sendFrameListChanged(currentIndex, newMap);
    }
}

void FrameManager::setPixmapList(const QList<QPixmap*>& newPixmapList)
{
    pixmapList = newPixmapList;
    currentIndex = 0;  // Reset selected to start
    emit sendUpdateAnimation(pixmapList);
}
