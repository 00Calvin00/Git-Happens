#include <QDebug>

#include "spritemodel.h"



Model::Model()
{

}

Model::~Model()
{
    qDeleteAll(pixmapList); // Delete each QPixmap pointer stored in the list
    pixmapList.clear();  // Clear the list
}

void Model::AddInitialFrame(Canvas* canvas)
{
    // Store pointer to QPixmap in the list
    //pixmapList.append(canvas->getPixmap());  // Assuming getPixmap() returns QPixmap*

    pixmapList.append(canvas->getPixmap());
    currentIndex = 0;  // Set the initial selected index
}

void Model::AddFrame() {
    if (pixmapList.size() != 0) {
        currentIndex++;
    }

    //QPixmap newMap = *new QPixmap(canvasSize, canvasSize); //heap allocation

    QPixmap* newMap = new QPixmap(canvasSize, canvasSize);


    newMap->fill(Qt::white);
    if (pixmapList.size() != currentIndex && pixmapList.size() != 0) {
        pixmapList.insert(currentIndex, newMap); // Insert inserts before i, turning i into the new object
    }
    else {
        pixmapList.append(newMap); // Else we are at the end, so simply append the newMap, since we already increased the index
    }
    emit SendFrameListChanged(currentIndex, newMap);
    emit SendUpdateAnimation(pixmapList);
}


// void Model::DuplicateFrame(QPixmap importedMap) {
//     if (pixmapList.size() != 0) {
//         currentIndex++;
//     }
//     if (pixmapList.size() != currentIndex && pixmapList.size() != 0) {
//         pixmapList.insert(currentIndex, importedMap); // Insert inserts before i, turning i into the new object
//     }
//     else {
//         pixmapList.append(importedMap); // Else we are at the end, so simply append the newMap, since we already increased the index
//     }
//     emit SendFrameListChanged(currentIndex, importedMap);
//     emit SendUpdateAnimation(pixmapList);
// }


void Model::DeleteFrame() {
    // If there’s more than one item, remove the current frame
    if (pixmapList.size() > 1)
    {
        pixmapList.remove(currentIndex); // Remove and retrieve the frame

        // Set currentIndex to last index if out of bounds
        if (currentIndex >= pixmapList.size()) {
            currentIndex = pixmapList.size() - 1;
        }
    }
    // If there’s only one item, remove it and add a new blank frame
    else {
        pixmapList.remove(currentIndex);
        AddFrame();
        currentIndex = 0;
    }

    QPixmap* newMap = pixmapList.at(currentIndex);
    emit SendFrameListChanged(currentIndex, newMap);
    emit SendUpdateAnimation(pixmapList);
}

void Model::SelectFrame(int i) {
    if (currentIndex >= 0 && currentIndex < pixmapList.size()) {
        currentIndex = i;
        QPixmap* newMap = pixmapList.at(i);
        emit SendFrameListChanged(currentIndex, newMap);
    } else {
        qDebug() << "Index out of bounds";
    }

    qDebug() << "Current frame selected at index:" << currentIndex;
    qDebug() << "Full frame list size:" << pixmapList.size();

}

void Model::SizeChange(int newSize) {
    canvasSize = newSize;
}

void Model::setPixmapList(const QList<QPixmap*>& newPixmapList)
{
    pixmapList = newPixmapList;
    currentIndex = 0;  // Reset selected to start
    emit SendUpdateAnimation(pixmapList);
}

const QList<QPixmap*> Model::getPixmapListValues()
{
    return pixmapList;
}

QList<QPixmap*>& Model::getPixmapListObjects()
{
    return pixmapList;
}

// Getter for currentIndex debugging
int Model::getCurrentIndex()
{
    return currentIndex;
}
