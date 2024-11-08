#include <QDebug>

#include "spritemodel.h"



Model::Model()
{

}

void Model::AddFrame() {
    if (pixmapList.size() != 0) {
        currentIndex++;
    }
    QPixmap newMap = *new QPixmap(canvasSize, canvasSize);
    newMap.fill(Qt::white);
    if (pixmapList.size() != currentIndex && pixmapList.size() != 0) {
        pixmapList.insert(currentIndex, newMap); // Insert inserts before i, turning i into the new object
    }
    else {
        pixmapList.append(newMap); // Else we are at the end, so simply append the newMap, since we already increased the index
    }
    emit SendFrameListChanged(currentIndex, newMap);
    emit SendUpdateAnimation(pixmapList);

}


void Model::DuplicateFrame(QPixmap importedMap) {
    if (pixmapList.size() != 0) {
        currentIndex++;
    }
    if (pixmapList.size() != currentIndex && pixmapList.size() != 0) {
        pixmapList.insert(currentIndex, importedMap); // Insert inserts before i, turning i into the new object
    }
    else {
        pixmapList.append(importedMap); // Else we are at the end, so simply append the newMap, since we already increased the index
    }
    emit SendFrameListChanged(currentIndex, importedMap);
    emit SendUpdateAnimation(pixmapList);
}



void Model::DeleteFrame() {
    if (pixmapList.size() > 1) {
        pixmapList.remove(currentIndex);
        currentIndex--;
    }
    else {
        // ERROR POPUP
    }
    QPixmap newMap = pixmapList.at(currentIndex);
    emit SendFrameListChanged(currentIndex, newMap);
    emit SendUpdateAnimation(pixmapList);
}

void Model::SelectFrame(int i) {
    currentIndex = i;
    QPixmap newMap = pixmapList.at(i);
    emit SendFrameListChanged(currentIndex, newMap);
}

void Model::SizeChange(int newSize) {
    canvasSize = newSize;
}
