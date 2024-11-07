#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <QPixmap>
#include <QPainter>
#include <QMouseEvent>

class Canvas : public QWidget
{
    Q_OBJECT

public:
    explicit Canvas(int canvasSize = 64, int scale = 10, QWidget *parent = nullptr);
    ~Canvas();

    QPixmap getPixmap() const;  // Method to return the pixmap

signals:
    void updateCanvas();  // Signal to notify that the canvas needs to be updated

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    void draw(QMouseEvent *event);

    QColor color;
    QPixmap *pixmap;
    int canvasSize;
    int scale;
    bool pressed;
};

#endif // CANVAS_H
