#ifndef MODEL_H
#define MODEL_H

#include <QPixmap>
#include <QObject>

class Model : public QObject
{
    Q_OBJECT
private:
    QList<QPixmap> pixmapList;
    int currentIndex = 0;  // Track the "current" pixmap
    int canvasSize = 512;
public:
    Model();
    void updateCanvasData(); // Example: method to interact with Canvas if needed
    void SelectFrame(int i);
    void DuplicateFrame(QPixmap importedMap);
    void AddFrame();
    void DeleteFrame();
    void SizeChange(int newSize);

signals:
    void SendFrameListChanged(int newIndex, QPixmap newMap);
    void SendUpdateAnimation(QList<QPixmap> newPixMap);

};

#endif // MODEL_H
