#ifndef MODEL_H
#define MODEL_H

#include <QPixmap>
#include <QObject>

class Model : public QObject
{
    Q_OBJECT
public:
    Model();
    void updateCanvasData(); // Example: method to interact with Canvas if needed
    void AddFrame();
    void DeleteFrame();
    void SelectFrame(int i);

signals:
    void SendFrameListChanged(int newIndex, QPixmap newMap);
    void SendUpdateAnimation(QList<QPixmap> newPixMap);
};

#endif // MODEL_H
