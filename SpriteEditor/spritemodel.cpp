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
    // if (pixmapList.size() > 1) {
    //     pixmapList.remove(currentIndex);
    //     currentIndex--;
    // }
    // else {
    //     // ERROR POPUP
    // }

    if (pixmapList.size() > 1)
    {
        //QPixmap* frameToDelete = pixmapList.at(currentIndex); // Might want to actually delete if using the new keyword

        pixmapList.remove(currentIndex);  // Remove pointer from the list
        currentIndex--;
    } else
    {
        // ERROR POPUP
    }

    QPixmap* newMap = pixmapList.at(currentIndex); //does not actually copy pixmap data
    emit SendFrameListChanged(currentIndex, newMap);
    emit SendUpdateAnimation(pixmapList);
}

void Model::SelectFrame(int i) {
    currentIndex = i;
    QPixmap* newMap = pixmapList.at(i);
    emit SendFrameListChanged(currentIndex, newMap);
}

void Model::SizeChange(int newSize) {
    canvasSize = newSize;
}

void Model::setPixmapList(const QList<QPixmap*> newPixmapList)
{
    pixmapList = newPixmapList;
    currentIndex = 0;  // Reset selected to start
    emit SendUpdateAnimation(pixmapList);
}

const QList<QPixmap*> Model::getPixmapList()
{
    return pixmapList;
}
