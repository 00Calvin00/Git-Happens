#include <QDebug>

#include "spritemodel.h"

QList<QPixmap> pixmapList;
int currentIndex = 0;  // Track the "current" pixmap

Model::Model()
{

}

void Model::AddFrame() {
    currentIndex++;
    QPixmap newMap = QPixmap();
    pixmapList.insert(currentIndex, newMap); // Insert inserts before i, turning i into the new object
    emit SendFrameListChanged(currentIndex, newMap);
    emit SendUpdateAnimation(pixmapList);

}

void Model::DeleteFrame() {
    pixmapList.remove(currentIndex);
    currentIndex--;
    QPixmap newMap = pixmapList.at(currentIndex);
    emit SendFrameListChanged(currentIndex, newMap);
    emit SendUpdateAnimation(pixmapList);
}

void Model::SelectFrame(int i) {
    currentIndex = i;
    QPixmap newMap = pixmapList.at(i);
    emit SendFrameListChanged(currentIndex, newMap);
}
