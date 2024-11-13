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

    // Function to set the scale (size of the grid cells)
    void setScale(int newScale);

    //Function to turn on and off grid
    void toggleGrid();

protected:
    // Paint event to draw the grid on the background
    void paintEvent(QPaintEvent *event) override;

private:
    int scale;             // Grid cell size
    QPixmap *pixmap;       // Background pixmap to draw on (grid)
    bool gridIsOn = false; //Default to the grid being off
};

#endif // BACKGROUND_H
