/* Reviewed by: Grace */

#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <QWidget>
#include <QPixmap>
#include <QPainter>

class Background : public QWidget
{
    Q_OBJECT

public:
    // Constructor to initialize the background
    Background(QWidget *parent = nullptr, int scale = 16);

    void setPixmap(const QPixmap *previousCurrentPixmap);

    // Function to set the scale (size of the grid cells)
    void setScale(int newScale);

    // Setter methods
    void setGridOn(bool on);
    void setOnionSkinningOn(bool on);
    void setCheckeredBackgroundOn(bool on);

protected:
    // Paint event to draw the grid on the background
    void paintEvent(QPaintEvent *event) override;


private:
    int scale;             // Grid cell size
    bool gridOn = false;
    bool onionSkinningOn = false;
    bool checkeredBackgroundOn = false;
    const QPixmap *onionSkinningPixmap;       // Background pixmap to draw on (grid)
};

#endif // BACKGROUND_H
