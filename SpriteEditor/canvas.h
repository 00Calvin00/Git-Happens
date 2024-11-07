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
    explicit Canvas(QWidget *parent = nullptr, int canvasSize = 64, int scale = 10, QColor color = QColor(0, 0, 0));
    ~Canvas();

    QPixmap getPixmap() const;  // Method to return the pixmap
    QColor color;               // Color to be drawn

signals:
    void updateCanvas();  // Signal to notify that the canvas needs to be updated

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    void draw(QMouseEvent *event);

    QPixmap *pixmap;
    int canvasSize;
    int scale;
    bool pressed;
};

#endif // CANVAS_H
